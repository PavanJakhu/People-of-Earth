#pragma once
#include <MegaEngine.h>
#include <glm\glm.hpp>
#include <iostream>

#include "FreeLook.h"
#include "FreeMove.h"
#include "TextLerpAlpha.h"
#include "MainMenuManager.h"
#include "MissionOneScene.h"
#include "OptionsScene.h"

class MainMenuScene : public Scene
{
public:
	MainMenuScene() : Scene("Main Menu") { }
	~MainMenuScene() { }

	virtual void init(const Viewport& window) override
	{
		AudioSource * BGM = new AudioSource("Music/Title Music.wav", AudioType::STREAM, true, 1.0f, false, true);
		//BGM->play();

		addToRoot((new GameObject("Skybox"))
			->addGameComponent(new SkyboxRenderer("Skybox/orbital/orbital.tga")));

		addToRoot((new GameObject("Camera"))
			->addGameComponent(new CameraComponent(ToRadians(60.0f), window.getAspectRatio(), 0.1f, 1000.0f))
			->addGameComponent(new FreeLook(window.getCenter()))
			->addGameComponent(BGM)
			//->addGameComponent(new FreeMove)
			);

		addToRoot((new GameObject("Splash"))
			->addGUIComponent(new GUILabel(PxVec4(0.4f, 0.6f, 0.2f, 0.05f), PxVec4(0.0f), "Press start...", 16))
			->addGameComponent(new TextLerpAlpha));

		m_logo = (new GameObject("Logo"))
			->addGUIComponent(new GUIImage(PxVec4(0.1f, 0.1f, 0.8f, 0.3f),
			PxVec4(0.0f), "Images/Logos/TPOE_LOGO_Trans.png", 2500.0f / 500.0f));
		addToRoot(m_logo);

		m_mainMenuObject = new GameObject("Main Menu");
		m_mainMenuObject->addChild((new GameObject("Play Button"))
			->addGUIComponent(new GUIButton(PxVec4(0.425f, 0.6f, 0.15f, 0.07f), PxVec4(0.0f), "Play",
			std::bind(&MainMenuScene::clickPlayButton, this, std::placeholders::_1), 16)));
		m_mainMenuObject->addChild((new GameObject("Options Button"))
			->addGUIComponent(new GUIButton(PxVec4(0.425f, 0.7f, 0.15f, 0.07f), PxVec4(0.0f), "Options",
			std::bind(&MainMenuScene::clickOptionsButton, this, std::placeholders::_1), 16)));
		m_mainMenuObject->addChild((new GameObject("Exit Button"))
			->addGUIComponent(new GUIButton(PxVec4(0.425f, 0.8f, 0.15f, 0.07f), PxVec4(0.0f), "Exit",
			std::bind(&MainMenuScene::clickExitButton, this, std::placeholders::_1), 16)));
		addToRoot(m_mainMenuObject);

		std::string formatting = "[colour='FFFFFFFF'][font='SaucerBB-16'][padding='l:5 t:0 r:5 b:0']";
		std::string objectives = "\t[colour='aaFF0000'][font='SaucerBB-20']Mission 1 : Into the Fire\n\n[colour='FFFFFFFF'][font='SaucerBB-16']The Terra 1 passenger ship\nwill not make it to Europa\nwithout your protection. It\nwon't be easy though, you'll\nhave to defeat a full squadron\nof alien fighters and a\nmother ship.\n\n[colour='FFFFFF00']Objectives:\n[vert-alignment='stretch'][colour='FFFFFFFF']1. Protect the Terra 1\n2. Destroy the enemy mother\nship\n\n\tGood Luck Soldier.";

		m_missionSelectMenuObject = new GameObject("Mission Select Menu");
		m_missionSelectMenuObject->addChild((new GameObject("Mission 1"))
			->addGUIComponent(new GUIContainer(PxVec4(0.1f, 0.1f, 0.8f, 0.8f), PxVec4(0.0f), "Mission 1"))
			->addChild((new GameObject("Mission 1 Button"))
			->addGUIComponent(new GUIButton(PxVec4(0.425f, 0.95f, 0.15f, 0.05f), PxVec4(0.0f), "Mission 1",
			std::bind(&MainMenuScene::clickMissionOne, this, std::placeholders::_1), 16)))
			->addChild((new GameObject("Mission 1 Image"))
			->addGUIComponent(new GUIImage(PxVec4(0.02f, 0.1f, 0.9f, 0.8f), PxVec4(0.0f), "Images/MissionOneImage.jpg")))
			->addChild((new GameObject("Objectives"))
			->addGUIComponent(new GUILabel(PxVec4(0.54f, 0.2f, 0.45f, 0.5f), PxVec4(0.0f), formatting + objectives))));
		m_missionSelectMenuObject->addChild((new GameObject("Mission 2"))
			->addGUIComponent(new GUIContainer(PxVec4(1.1f, 0.1f, 0.8f, 0.8f), PxVec4(0.0f), "Mission 2"))
			->addChild((new GameObject("Mission 2 Button"))
			->addGUIComponent(new GUIButton(PxVec4(0.425f, 0.95f, 0.15f, 0.05f), PxVec4(0.0f), "Mission 2",
			std::bind(&MainMenuScene::clickMissionTwo, this, std::placeholders::_1), 16)))
			->addChild((new GameObject("Mission 2 Image"))
			->addGUIComponent(new GUIImage(PxVec4(0.05f, 0.1f, 0.9f, 0.8f), PxVec4(0.0f), "Images/MissionTwoImage.png"))));
		m_missionSelectMenuObject->addChild((new GameObject("Mission 3"))
			->addGUIComponent(new GUIContainer(PxVec4(2.1f, 0.1f, 0.8f, 0.8f), PxVec4(0.0f), "Mission 3"))
			->addChild((new GameObject("Mission 3 Button"))
			->addGUIComponent(new GUIButton(PxVec4(0.425f, 0.95f, 0.15f, 0.05f), PxVec4(0.0f), "Mission 3",
			std::bind(&MainMenuScene::clickMissionThree, this, std::placeholders::_1), 16)))
			->addChild((new GameObject("Mission 3 Image"))
			->addGUIComponent(new GUIImage(PxVec4(0.05f, 0.1f, 0.9f, 0.8f), PxVec4(0.0f), "Images/MissionThreeImage.png"))));
		m_missionSelectMenuObject->addChild((new GameObject("Back Button"))
			->addGUIComponent(new GUIButton(PxVec4(0.45f, 0.95f, 0.1f, 0.05f), PxVec4(0.0f), "Back",
			std::bind(&MainMenuScene::clickBackButton, this, std::placeholders::_1))));

		addToRoot(m_missionSelectMenuObject);

		addToRoot((new GameObject("Moved button sound"))
			->addGameComponent(new AudioSource("Music/(hover_beep)185828__lloydevans09__little-thing.wav", AudioType::SOUND, false)));
		addToRoot((new GameObject("Selected button sound"))
			->addGameComponent(new AudioSource("Music/(select_beep)319200__18hiltc__pixel-game-beep.wav", AudioType::SOUND, false)));

		addToRoot((new GameObject("Main Menu Manager"))
			->addGameComponent(new MainMenuManager(3)));

		// TODO: Add sound effects for hovering
		// TODO: Add Sound effect for buttons selected
	}

private:
	GameObject* m_logo;
	GameObject* m_mainMenuObject;
	GameObject* m_missionSelectMenuObject;
	GameObject* m_missionSelectManagerObject;

	bool clickPlayButton(const GameObject& game)
	{
		m_logo->setEnabled(false);
		m_mainMenuObject->setEnabled(false);
		m_missionSelectMenuObject->setEnabled(true);
		//m_missionSelectManagerObject->getGameComponent<MissionSelectManager>()->setCheckInput(false);
		return true;
	}

	bool clickOptionsButton(const GameObject& game)
	{
		getCoreEngine()->getSceneManager()->push(new OptionsScene, Modality::Popup);
		return true;
	}

	bool clickExitButton(const GameObject& game)
	{
		getCoreEngine()->stop();
		return true;
	}

	//Mission Select
	bool clickBackButton(const GameObject& gameobject)
	{
		m_logo->setEnabled(true);
		m_mainMenuObject->setEnabled(true);
		m_missionSelectMenuObject->setEnabled(false);
		//m_missionSelectManagerObject->getGameComponent<MissionSelectManager>()->setCheckInput(false);
		return true;
	}

	bool clickMissionOne(const GameObject& gameobject)
	{
		getCoreEngine()->getSceneManager()->push(new MissionOneScene);
		return true;
	}

	bool clickMissionTwo(const GameObject& gameobject)
	{
		return true;
	}

	bool clickMissionThree(const GameObject& gameobject)
	{
		return true;
	}
};
