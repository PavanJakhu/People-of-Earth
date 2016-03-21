// ***********************************************************************
// Author           : Christopher Maeda
// Created          : 03-11-2016
//
// Last Modified By : Christopher Maeda
// Last Modified On : 03-21-2016
// ***********************************************************************
// <copyright file="MiniMap.h" company="Team MegaFox">
//     Copyright (c) Team MegaFox. All rights reserved.
// </copyright>
// <summary>
//</summary>
// ***********************************************************************

#pragma once
#include <MegaEngine.h>
#include <PhysX/PxPhysicsAPI.h>
#include <vector>
using namespace physx;

class MiniMap : public GameComponent
{
public: 
	MiniMap(PxVec2 miniMapCenterPosition = PxVec2(0), float miniMapRadius = 1000.0f) :
		m_miniMapCenterPosition(miniMapCenterPosition),
		m_miniMapRadius(miniMapRadius) {}

	~MiniMap() {}

	virtual void onStart()
	{
		//Adding Figher Ships
		m_playerGameObject = getGameObjectByName("Fighter Ship");
		playerPosition = PxVec3(0);
		getParent()->addChild(
			(new GameObject(m_playerGameObject->getName() + "_GUI"))
			->addGUIComponent(new GUIImage(PxVec4(1.0f, 1.0f, 0.1f, 0.1f), PxVec4(0.0f), "Images/minimap_graphics/player_marker.png"))
			);
		//Adding Passenger Ship
		m_passengerGameObject = getGameObjectByName("passengerShip");
		getParent()->addChild(
			(new GameObject(m_passengerGameObject->getName() + "_GUI"))
			->addGUIComponent(new GUIImage(PxVec4(0.0f, 0.70f, 0.1f, 0.1f), PxVec4(0.0f), "Images/minimap_graphics/passenger_ship_marker.png"))
			);
		//Adding Enemy Fighter Ship
		m_allEnemyGameObjects = getGameObjectsByName("enemyFighter");
		for (size_t i = 0; i < m_allEnemyGameObjects.size(); i++)
		{
			getParent()->addChild(
				(new GameObject(m_allEnemyGameObjects[i]->getName() + "_GUI"))
				->addGUIComponent(new GUIImage(PxVec4(0.0f, 0.70f, 0.1f, 0.1f), PxVec4(0.0f), "Images/minimap_graphics/enemy_marker.png"))
				);
		}

		////Add child for all the GUI Components
		//for (size_t i = 0; i < allGameObjects.size(); i++)
		//{
		//	if (allGameObjects[i]->getName() == "Fighter Ship")
		//	{
		//		getParent()->addChild(
		//			(new GameObject(allGameObjects[i]->getName() + "_GUI"))
		//			->addGUIComponent(new GUIImage(PxVec4(0.0f, 0.70f, 0.3f, 0.3f), PxVec4(0.0f), "Images/healthbar.png"))
		//			);
		//	}
		//	if (allGameObjects[i]->getName() == "passengerShip")
		//	{
		//		getParent()->addChild(
		//			(new GameObject(allGameObjects[i]->getName() + "_GUI"))
		//			->addGUIComponent(new GUIImage(PxVec4(0.0f, 0.70f, 0.3f, 0.3f), PxVec4(0.0f), "Images/energybar.png"))
		//			);
		//	}
		//	if (allGameObjects[i]->getName() == "enemyFighter")
		//	{
		//		getParent()->addChild(
		//			(new GameObject(allGameObjects[i]->getName() + "_GUI"))
		//			->addGUIComponent(new GUIImage(PxVec4(0.0f, 0.70f, 0.3f, 0.3f), PxVec4(0.0f), "Images/space.png"))
		//			);
		//	}
		//}
	}

	virtual void update(float delta) override
	{
		//Updates all the GUI Gameobject in the scene from the Scene GameObjects
		bool dead;
		for (size_t i = 0; i < getParent()->getAllChildren().size(); i++)
		{
			dead = true;
			if (getParent()->getAllChildren()[i]->getName() == m_playerGameObject->getName() + "_GUI")
			{
				getParent()->getAllChildren()[i]->getTransform()->setPosition(*m_playerGameObject->getTransform()->getPosition());
				getParent()->getAllChildren()[i]->getTransform()->setRotation(*m_playerGameObject->getTransform()->getRotation());
				dead = false;
			}
			if (dead)
			{
				if (getParent()->getAllChildren()[i]->getName() == m_passengerGameObject->getName() + "_GUI")
				{
					getParent()->getAllChildren()[i]->getTransform()->setPosition(*m_passengerGameObject->getTransform()->getPosition());
					getParent()->getAllChildren()[i]->getTransform()->setRotation(*m_passengerGameObject->getTransform()->getRotation());
					dead = false;
				}
			}
			if (dead)
			{
				for (size_t j = 0; j < m_allEnemyGameObjects.size(); j++)
				{
					if (getParent()->getAllChildren()[i]->getName() == m_allEnemyGameObjects[j]->getName() + "_GUI")
					{
						getParent()->getAllChildren()[i]->getTransform()->setPosition(*m_allEnemyGameObjects[j]->getTransform()->getPosition());
						getParent()->getAllChildren()[i]->getTransform()->setRotation(*m_allEnemyGameObjects[j]->getTransform()->getRotation());
						dead = false;
						break;
					}
				}
			}
			//if the gameobject is dead in the scene then
			if (dead)
			{
				delete getParent()->getAllChildren()[i];
				getParent()->getAllChildren().erase(getParent()->getAllChildren().begin() + i);
			}
		}
		setMiniMapPosition();
	}

	virtual void processInput(const InputManager& input, float delta) override
	{
		if (input.PadButtonDown(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
		{
			m_angleRotationZ += 0.025f;
			checkToResetCalculations();
		}
		if (input.PadButtonDown(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
		{
			m_angleRotationZ -= 0.025f;
			checkToResetCalculations();
		}

		if (input.GetThumbRPosition().x > 0.3f)
		{
			if ((PxAbs(m_angleRotationZ) > 1.57f && PxAbs(m_angleRotationZ) < 3.14f)
				|| (PxAbs(m_angleRotationZ) > 4.71f && PxAbs(m_angleRotationZ) < 6.28f))
			{
				m_angleRotationY -= 0.025f;
			}
			else
			{
				m_angleRotationY += 0.025f;
			}
			checkToResetCalculations();
		}
		if (input.GetThumbRPosition().x < -0.3f)
		{
			if ((PxAbs(m_angleRotationZ) > 1.57f && PxAbs(m_angleRotationZ) < 3.14f)
				|| (PxAbs(m_angleRotationZ) > 4.71f && PxAbs(m_angleRotationZ) < 6.28f))
			{
				m_angleRotationY += 0.025f;
			}
			else
			{
				m_angleRotationY -= 0.025f;
			}
			checkToResetCalculations();
		}
		if (input.GetThumbRPosition().y > 0.3f)
		{
			//m_angleRotationX -= 0.025f;
			if (PxAbs(m_angleRotationZ) > 1.57f && PxAbs(m_angleRotationZ) < 4.71f)
			{
				m_angleRotationX -= 0.025f;
			}
			else
			{
				m_angleRotationX += 0.025f;
			}
			checkToFlipMapDirection();	
			checkToResetCalculations();
		}
		if (input.GetThumbRPosition().y < -0.3f)
		{
			//m_angleRotationX += 0.025f;
			if (PxAbs(m_angleRotationZ) > 1.57f && PxAbs(m_angleRotationZ) < 4.71f)
			{
				m_angleRotationX += 0.025f;
			}
			else
			{
				m_angleRotationX -= 0.025f;
			}
			checkToFlipMapDirection();
			checkToResetCalculations();
		}
	}

	void checkToFlipMapDirection()
	{
		float absRotationXValue = PxAbs(m_angleRotationX);
		if (absRotationXValue > 1.57f && absRotationXValue < 4.71f)
		{
			m_flipDirection = true;
		}
		else if (absRotationXValue > 4.71f && absRotationXValue < 6.28f)
		{
			m_flipDirection = false;
		}
		else
		{
			m_flipDirection = false;
		}
	}

	void checkToResetCalculations()
	{
		float absRotationValue = PxAbs(m_angleRotationX);
		if (absRotationValue > 6.28f)
		{
			if (m_angleRotationX > 0)
			{
				m_angleRotationX -= 6.28f;
			}
			else
			{
				m_angleRotationX += 6.28f;
			}
		}
		absRotationValue = PxAbs(m_angleRotationY);
		if (absRotationValue > 6.28f)
		{
			if (m_angleRotationY > 0)
			{
				m_angleRotationY -= 6.28f;
			}
			else
			{
				m_angleRotationY += 6.28f;
			}
		}
		absRotationValue = PxAbs(m_angleRotationZ);
		if (absRotationValue > 6.28f)
		{
			if (m_angleRotationZ > 0)
			{
				m_angleRotationZ -= 6.28f;
			}
			else
			{
				m_angleRotationZ += 6.28f;
			}
		}
		/*if (absRotationZValue > 1.57f && absRotationZValue < 4.71f)
		{
			m_flipDirection = true;
		}
		else if (absRotationZValue > 4.71f && absRotationZValue < 6.28f)
		{
			m_flipDirection = false;
		}
		else if (absRotationZValue > 6.28f)
		{
			if (m_angleRotationZ > 0)
			{
				m_angleRotationZ -= 6.28f;
			}
			else
			{
				m_angleRotationZ += 6.28f;
			}
		}*/
	}

	void setMiniMapPosition()
	{
		//Get the origin of the map in the scene
		playerPosition = *getParent()->getAllChildren()[0]->getTransform()->getPosition();
		//Checks to see if the GUI component needs to be rendered
		float distance;
		for (size_t i = 0; i < getParent()->getAllChildren().size(); i++)
		{
			distance = Utility::getDistance(playerPosition, *getParent()->getAllChildren()[i]->getTransform()->getPosition());
			//Distance check
			if (distance < m_miniMapRadius)
			{
				//Get the direction
				PxVec3 direction = *getParent()->getAllChildren()[i]->getTransform()->getPosition() - playerPosition;
				direction.y = 0.0f;
				direction.normalize();

				PxVec3 playerDir = Utility::getForward(*m_playerGameObject->getTransform()->getRotation());
				playerDir.y = 0.0f;
				playerDir.normalize();

					/*PxVec3(Utility::getRight(*m_playerGameObject->getTransform()->getRotation()).x, 0.0f,
					Utility::getForward(*m_playerGameObject->getTransform()->getRotation()).z);*/
				//playerDir.normalize();

				float angle = PxAcos(playerDir.dot(PxVec3(0.0f, 0.0f, 1.0f)));
				
				if ((playerDir.x > 0.0f && playerDir.z < 0.0f) || (playerDir.x > 0.0f && playerDir.z > 0.0f))
				{
					angle = ToRadians(360.0f) - angle;
				}

				//Rotate
				PxVec3 tempDirection = direction;
				direction.z = tempDirection.z * cos(angle) - tempDirection.x * sin(angle);
				direction.x = tempDirection.z * sin(angle) + tempDirection.x * cos(angle);

				/*PxQuat pureQuat = PxQuat(direction.x, direction.y, direction.z, 0);
				PxQuat playerQuat = *m_playerGameObject->getTransform()->getRotation();
				PxQuat finalRot = playerQuat * pureQuat * playerQuat.getConjugate();
				direction = PxVec3(finalRot.x, finalRot.y, finalRot.z);
				direction *= -1;*/


				direction *= distance / m_miniMapRadius;
				direction.z *= -1;

				//Rotate the direction according to the identidy forward
				//PxVec3 tempDirection = direction;
				//direction.z = tempDirection.z * cos(m_angleRotationY) - tempDirection.x * sin(m_angleRotationY);
				//direction.x = tempDirection.z * sin(m_angleRotationY) + tempDirection.x * cos(m_angleRotationY);
				//if (m_flipDirection)
				//{
				//	direction.z *= -1.0f;
				//}

				//Update on the GUI Image
				getParent()->getAllChildren()[i]->getGUIComponent<GUIImage>()->setPercentPosition(
					PxVec2(0.5f, 0.5f) +
					PxVec2(direction.x, direction.z)
					- getParent()->getAllChildren()[i]->getGUIComponent<GUIImage>()->getPercentSize() / 2);
			}
			else
			{
				//Off the minimap
				getParent()->getAllChildren()[i]->setEnabled(false);
			}

		}
	}

private:
	GameObject* m_playerGameObject;
	GameObject* m_passengerGameObject;
	std::vector<GameObject*> m_allEnemyGameObjects;
	PxVec3 playerPosition;
	float m_miniMapRadius;
	PxVec2 m_miniMapCenterPosition;

	float m_angleRotationY = 0.0f;
	float m_angleRotationX = 0.0f;
	float m_angleRotationZ = 0.0f;
	bool m_flipDirection = false;
	bool m_flipCalculations = false;
};