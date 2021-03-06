// ***********************************************************************
// Author           : Pavan Jakhu, Jesse Derochie and Christopher Maeda
// Created          : 09-15-2015
//
// Last Modified By : Jesse Derochie
// Last Modified On : 03-29-2016
// ***********************************************************************
// <copyright file="SceneManager.cpp" company="Team MegaFox">
//     Copyright (c) Team MegaFox. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "..\Components\GameComponents.h"
#include "..\Components\AudioSource.h"
#include "..\Rendering\RenderingEngine.h"
#include "..\Rendering\Camera3D.h"
#include "..\Physics\PhysicsEngine.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <assert.h>

SceneManager::SceneManager(Viewport* viewport) :
m_viewport(viewport)
{
}

SceneManager::~SceneManager()
{
	while (!m_activeList.empty())
	{
		pop();
	}
}

Scene* SceneManager::peek()
{
	Scene* result = nullptr;
	if (!m_activeList.empty())
	{
		result = m_activeList.back().first;
	}
	return result;
}

void SceneManager::push(Scene* scene, Modality modality /*= Modality::Exclusive*/)
{
	if (!m_activeList.empty())
	{
		auto go = m_activeList.back().first->getAllGameObjects();
		for (size_t i = 0; i < go.size(); i++)
		{
			//go[i]->deactivate();
			if (modality == Modality::Exclusive)
			{
				go[i]->setWasEnabled(go[i]->isEnabled());
				go[i]->setEnabled(false, false);

				AudioSource * audio = go[i]->getGameComponent<AudioSource>();
				if (audio != nullptr)
				{
					if (audio->getType() == AudioType::STREAM)
					{
						audio->setWasPlaying(audio->isPlaying());
						// pause music
						audio->stop();
					}
				}
			}
		}
	}

	if (modality == Modality::Popup)
	{
		m_activeList.back().first->notifyCoveredObjects();
	}

	m_activeList.push_back(std::make_pair(scene, modality));

	scene->init(*m_viewport);
	scene->setEngine(m_coreEngine);
	auto attachedToRoot = scene->getRoot()->getAllAttached();
	for (size_t i = attachedToRoot.size() - 1; i > 0; i--)
	{
		auto gameComponents = attachedToRoot[i]->getAllGameComponents();
		for (size_t j = 0; j < gameComponents.size(); j++)
		{
			gameComponents[j]->onStart();
		}
	}

	updateExclusiveScene();	
}

void SceneManager::pop()
{
	if (m_activeList.empty()) 
	{
		throw std::runtime_error("Attempted to pop from an empty game state stack");
	}

	bool poppingExclusive = false;
	if (m_activeList.back().second == Modality::Exclusive)
	{
		poppingExclusive = true;
	}

	delete m_activeList.back().first;
	m_activeList.pop_back();

	if (!m_activeList.empty())
	{
		updateExclusiveScene();

		if (!poppingExclusive)
		{
			m_activeList.back().first->notifyUncoveredObjects();
		}

		auto go = m_activeList.back().first->getAllGameObjects();
		for (size_t i = 0; i < go.size(); i++)
		{
			//go[i]->activate();
			if (poppingExclusive)
			{
				go[i]->setEnabled(go[i]->wasEnabled(), false);
			}
		}

		if (poppingExclusive)
		{
			go = m_activeList[m_exclusiveScene].first->getAllGameObjects();
			for (size_t i = 0; i < go.size(); i++)
			{
				AudioSource * audio = go[i]->getGameComponent<AudioSource>();
				if (audio != nullptr)
				{
					if (audio->getType() == AudioType::STREAM && audio->wasPlaying())
					{
						audio->play();
					}
				}
			}
		}
	}
}

void SceneManager::popTo(Uint8 popIndex)
{
	assert(popIndex >= 0 && popIndex < m_activeList.size());

	while (popIndex != m_activeList.size() - 1)
	{
		pop();
	}
}

Scene* SceneManager::switchScene(Scene* scene, Modality modality /*= Modality::Exclusive*/)
{
	Scene* currentScene = peek();

	if (currentScene)
	{
		pop();
	}

	push(scene, modality);

	return currentScene;
}

void SceneManager::update(float delta)
{
	m_activeList.back().first->update(delta);
}

void SceneManager::render(RenderingEngine* renderingEngine)
{
	for (size_t i = m_exclusiveScene; i < m_activeList.size(); i++)
	{
		if (m_activeList[i].second == Modality::Exclusive)
		{
			m_activeList[i].first->render(renderingEngine);
		}
	}
}

void SceneManager::processInput(const InputManager& input, float delta)
{
	m_activeList.back().first->processInput(input, delta);
}

void SceneManager::setEngine(CoreEngine* engine)
{
	m_coreEngine = engine;

	for (size_t i = 0; i < m_activeList.size(); i++)
	{
		m_activeList[i].first->setEngine(engine);
	}
}

bool SceneManager::removeGameObjectByName(const std::string& name)
{
	//Gets all gameobject in the scene
	std::vector<GameObject*> attached = peek()->getRoot()->getAllAttached();
	
	//Loop through all the gameobject
	for (size_t i = 0; i < attached.size(); i++)
	{
		//if the gameobject name is the same as the name you are looking for then
		if (attached[i]->getName() == name)
		{
			//Delete gameobject from the parent
			attached[i]->getTransform()->getParent()->getAttachedGameObject()->removeChild(attached[i]);
			return true;
		}
	}
	return false;
}

GameObject* SceneManager::getGameObjectByName(const std::string& name)
{
	GameObject* result = nullptr;

	Uint16 counter = peek()->getNameCounter(name);

	std::vector<GameObject*> attached = peek()->getRoot()->getAllAttached();
	for (size_t i = 0; i < attached.size(); i++)
	{
		for (Uint16 j = 1; j < counter + 1; j++)
		{
			if (attached[i]->getName() == name + std::to_string(j))
			{
				result = attached[i];
			}
		}
	}

	return result;
}

std::vector<GameObject*> SceneManager::getGameObjectsByName(const std::string& name)
{
	std::vector<GameObject*> returnGameObjects;

	Uint16 counter = peek()->getNameCounter(name);

	std::vector<GameObject*> attached = peek()->getRoot()->getAllAttached();

	for (size_t i = 0; i < attached.size(); i++)
	{
		for (Uint16 j = 1; j < counter + 1; j++)
		{
			if (attached[i]->getName() == name + std::to_string(j))
			{
				returnGameObjects.push_back(attached[i]);
			}
		}
	}

	return returnGameObjects;
}

void SceneManager::updateExclusiveScene()
{
	//auto go = m_activeList[m_exclusiveScene].first->getAllGameObjects();

	//for (size_t i = 0; i < go.size(); i++)
	//{
	//	Audio * audio = go[i]->getGameComponent<Audio>();
	//	if (audio != nullptr)
	//	{
	//		audio->pause();
	//	}
	//}

	for (size_t i = m_activeList.size() - 1; i >= 0; i--)
	{
		if (m_activeList[i].second == Modality::Exclusive)
		{
			m_exclusiveScene = i;
			break;
		}
	}

	auto go = m_activeList[m_exclusiveScene].first->getAllGameObjects();
	for (size_t i = 0; i < go.size(); i++)
	{
		CameraComponent* camera = go[i]->getGameComponent<CameraComponent>();
		if (camera != nullptr)
		{
			m_coreEngine->getPhysicsEngine()->setMainCamera(*camera->getCamera3D());
			m_coreEngine->getRenderingEngine()->setMainCamera(*camera->getCamera3D());
		}

		SkyboxRenderer* skybox = go[i]->getGameComponent<SkyboxRenderer>();
		if (skybox != nullptr)
		{
			m_coreEngine->getRenderingEngine()->setSkybox(*skybox->getSkybox());
		}
	}
}
