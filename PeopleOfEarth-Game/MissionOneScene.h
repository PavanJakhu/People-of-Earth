#pragma once
#include <MegaEngine.h>

#include "FreeLook.h"
#include "FreeMove.h"
#include "ShowPauseComp.h"
#include "FireProjectile.h"
#include "EnemyFighterShipAI.h"
#include "PassengerShipAI.h"
#include "AsteroidField.h"
#include "UpdateGUi.h"
#include "MiniMap.h"
#include "RogueAsteroid.h"
#include <PhysX/PxPhysicsAPI.h>
#include "QuatRotator.h"
using namespace physx;

class MissionOneScene : public Scene
{
public:
	MissionOneScene() : Scene("Mission 1") { }
	~MissionOneScene() { }

	virtual void init(const Viewport& window) override
	{
		Material ship1("ship1", 0.5f, 4, Texture("Ships/AF-SS01/AF-SS01_White.png"), Texture("Ships/AF-SS01/AF-SS01_Normalmap.png"));
		Material alienShip("alien_ship", 5.0f, 4,
			Texture("Ships/Alien/astra_elements2_c.png"),
			Texture("Ships/Alien/astraBattleship_Normals.png"),
			Texture("Ships/Alien/astra_elements2_i.png"));

		Material motherShip("passengerShip", 0.5f, 4, Texture("Ships/MotherShip/MotherShip.png"));
		Material earth("earth", 1.0f, 10, Texture("Planets/earth.png"));
		Material mars("mars", 1.0f, 10, Texture("Planets/mars.jpg"));
		Material jupiter("jupiter", 1.0f, 10, Texture("Planets/jupiter.jpg"));
		Material sun("sun", 10.0f, 100, Texture("Planets/sun.jpg"));
		Material moon("moon", 1.0f, 10, Texture("Planets/moon.jpg"));

		Material asteroid("asteroid", 1.0f, 1.0f, Texture("Asteroids/AM1.jpg"));

		addToRoot((new GameObject("Skybox"))
			->addGameComponent(new SkyboxRenderer("Skybox/Starfield/starfield.tga")));

		GameObject * starBoardLight_Fighter = new GameObject("starBoardPointLight_Fighter", PxVec3(7.0f, 1.0f, -3.5f));
		starBoardLight_Fighter->addGameComponent(new PointLight(PxVec3(0.0f, 1.0f, 0.0f), 0.5f));

		GameObject * portLight_Fighter = new GameObject("portPointLight_Fighter", PxVec3(-7.0f, 1.0f, -3.5f));
		portLight_Fighter->addGameComponent(new PointLight(PxVec3(1.0f, 0.0f, 0.0f), 0.5f));

		GameObject * rearLight_Fighter = new GameObject("rearPointLight_Fighter", PxVec3(0.0f, 1.0f, -6.0f));
		rearLight_Fighter->addGameComponent(new PointLight(PxVec3(0.1f, 0.1f, 0.1f), 0.5f));

		GameObject * thrusterLight_Fighter = new GameObject("thrusterPointLight_Fighter", PxVec3(0.0f, 0.0f, -8.0f));
		thrusterLight_Fighter->addGameComponent(new PointLight(PxVec3(1.0f, 0.75f, 0.0f), 0.5f));

		GameObject * spotLight_Fighter = new GameObject("spotLight_Fighter", PxVec3(0.0f, 0.0f, 8.0f), PxQuat(ToRadians(1.0f), PxVec3(0.0f, 1.0f, 0.0f)));

		spotLight_Fighter->addGameComponent(new SpotLight(PxVec3(1.0f), 150.0f, Attenuation(1.0f, 1.0f, 0.0f)));

		GameObject* fighterShip =
			(new GameObject("Fighter Ship", PxVec3(0.0f, 0.0f, 0.0f), PxQuat(ToRadians(180.0f), PxVec3(0.0f, 1.0f, 0.0f))))
			->addGameComponent(new MeshRenderer(Mesh("Ships/AF-SS01.obj", 1.0f), Material("ship1")))
			->addGameComponent(new RigidBody(PxVec3(1.0f, 1.0f, 1.0f), PxQuat(PxIdentity), 1.0f, 7.0f, 1.0f, 5.0f))
			->addGameComponent(new FireProjectile("(laser_fired)268168__shaun105__laser.wav"))
			->addGameComponent(new PlayerShipMovementController("camera", 50.0f))
			->addChild(starBoardLight_Fighter)
			->addChild(portLight_Fighter)
			->addChild(rearLight_Fighter)
			->addChild(thrusterLight_Fighter)
			->addChild(spotLight_Fighter)
			->addGameComponent(new ShipStats)
			->addGameComponent(new UpdateGUI);

		// The human fighter ship and camera
		GameObject* camera =
			(new GameObject("camera",
			*fighterShip->getTransform()->getPosition() - Utility::getForward(*fighterShip->getTransform()->getRotation()) * 30.0f
			+ PxVec3(0.0f, 5.0f, 0.0f)))
			->addGameComponent(new CameraComponent(ToRadians(75.0f), window.getAspectRatio(), 0.1f, 200000.0f))
			->addGameComponent(new Listener());
		addToRoot(fighterShip);
		addToRoot(camera);


		addToRoot((new GameObject("DirectionalLight", PxVec3(0.0f, 0.0f, 0.0f), PxQuat(ToRadians(180.0f), PxVec3(0.0f, 1.0f, 0.0f).getNormalized())))
			->addGameComponent(new DirectionalLight(PxVec3(1.0f, 1.0f, 1.0f), 0.6f, 8)));

		// PassengerShip Lights

		// Tower North Bottom East
		GameObject * spotLight1_Passenger = 
			new GameObject("starBoardPointLight_Passenger", 
			PxVec3(17.0f, 0.0f, -50.0f),
			PxQuat(ToRadians(45.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		spotLight1_Passenger->addGameComponent(
			new SpotLight(
			PxVec3(1.0f), 100.0f, Attenuation(1.0f, 1.0f, 0.0f)));
		spotLight1_Passenger->addGameComponent(new PointLight(
			PxVec3(1.0f), 100.0f));

		// Tower North Bottom West
		GameObject * spotLight2_Passenger =
			new GameObject("starBoardPointLight_Passenger",
			PxVec3(-17.0f, 0.0f, -50.0f),
			PxQuat(ToRadians(-45.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		spotLight2_Passenger->addGameComponent(
			new SpotLight(
			PxVec3(1.0f), 100.0f, Attenuation(1.0f, 1.0f, 0.0f)));
		spotLight2_Passenger->addGameComponent(new PointLight(
			PxVec3(1.0f), 100.0f));

		// Tower South Bottom West
		GameObject * spotLight3_Passenger =
			new GameObject("starBoardPointLight_Passenger",
			PxVec3(-17.0f, 0.0f, -140.0f),
			PxQuat(ToRadians(-135.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		spotLight3_Passenger->addGameComponent(
			new SpotLight(
			PxVec3(1.0f), 100.0f, Attenuation(1.0f, 1.0f, 0.0f)));
		spotLight3_Passenger->addGameComponent(new PointLight(
			PxVec3(1.0f), 100.0f));

		// Tower South Bottom East
		GameObject * spotLight4_Passenger =
			new GameObject("starBoardPointLight_Passenger",
			PxVec3(17.0f, 0.0f, -140.0f),
			PxQuat(ToRadians(135.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		spotLight4_Passenger->addGameComponent(
			new SpotLight(
			PxVec3(1.0f), 100.0f, Attenuation(1.0f, 1.0f, 0.0f)));
		spotLight4_Passenger->addGameComponent(new PointLight(
			PxVec3(1.0f), 100.0f));

		// Tower North Top West
		GameObject * spotLight5_Passenger =
			new GameObject("starBoardPointLight_Passenger",
			PxVec3(17.0f, 45.0f, -62.0f),
			PxQuat(ToRadians(45.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		spotLight5_Passenger->addGameComponent(
			new SpotLight(
			PxVec3(1.0f), 100.0f, Attenuation(1.0f, 1.0f, 0.0f)));
		spotLight5_Passenger->addGameComponent(new PointLight(
			PxVec3(1.0f), 100.0f));

		// Tower North Top East
		GameObject * spotLight6_Passenger =
			new GameObject("starBoardPointLight_Passenger",
			PxVec3(-17.0f, 45.0f, -62.0f),
			PxQuat(ToRadians(-45.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		spotLight6_Passenger->addGameComponent(
			new SpotLight(
			PxVec3(1.0f), 100.0f, Attenuation(1.0f, 1.0f, 0.0f)));
		spotLight6_Passenger->addGameComponent(new PointLight(
			PxVec3(1.0f), 100.0f));

		// Off North of Ship to the West 
		GameObject * spotLight7_Passenger =
			new GameObject("spotLight7_Passenger",
			PxVec3(100.0f, 0.0f, 370.0f),
			PxQuat(ToRadians(-135.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		spotLight7_Passenger->addGameComponent(
			new SpotLight(
			PxVec3(1.0f), 150.0f, Attenuation(1.0f, 1.0f, 0.0f)));

		// Off North of Ship to the East
		GameObject * spotLight8_Passenger =
			new GameObject("spotLight8_Passenger",
			PxVec3(-100.0f, 0.0f, 370.0f),
			PxQuat(ToRadians(135.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		spotLight8_Passenger->addGameComponent(
			new SpotLight(
			PxVec3(1.0f), 100.0f, Attenuation(1.0f, 1.0f, 0.0f)));

		// Thruster South Bottom East
		PxVec3 thruster1(53.0f, -40.0f, -84.0f);
		GameObject * passenger_Thruster1 =
			new GameObject("passenger_Thruster1",
			thruster1,
			PxQuat(ToRadians(-45.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		passenger_Thruster1->addGameComponent(new QuatRotator(thruster1));
		passenger_Thruster1->addGameComponent(
			new PointLight(
			PxVec3(1.0f, 0.5f, 0.0f), 150.0f));
		passenger_Thruster1->addGameComponent(
			new SpotLight(
			PxVec3(1.0f, 0.5f, 0.0f), 150.0f));

		// Thruster South Bottom West
		PxVec3 thruster2(-53.0f, -40.0f, -84.0f);
		GameObject * passenger_Thruster2 =
			new GameObject("passenger_Thruster2",
			thruster2,
			PxQuat(ToRadians(-45.0f), PxVec3(0.0f, 1.0f, 0.0f)));
		passenger_Thruster2->addGameComponent(new QuatRotator(thruster2));
		passenger_Thruster2->addGameComponent(
			new PointLight(
			PxVec3(1.0f, 0.5f, 0.0f), 150.0f));
		passenger_Thruster2->addGameComponent(
			new SpotLight(
			PxVec3(1.0f, 0.5f, 0.0f), 150.0f));

		// the passenger ship
		addToRoot((new GameObject("passengerShip", PxVec3(0.0f, 0.0f, 300.0f), PxQuat(0.0f, 0.0f, 0.0f, 1.0f), PxVec3(1.0f)))
			->addGameComponent(new MeshRenderer(Mesh("Ships/MotherShip.obj", 50.0f), Material("passengerShip")))
			->addGameComponent(new RigidBody(PxVec3(0.0f, 0.0f, 300.0f), PxQuat(PxIdentity), 1.0f, 50.0f, 40.0f, 275.0f))
			->addGameComponent(new PassengerShipAI)
			->addGameComponent(new ShipStats)
			->addChild(spotLight1_Passenger)
			->addChild(spotLight2_Passenger)
			->addChild(spotLight3_Passenger)
			->addChild(spotLight4_Passenger)
			->addChild(spotLight5_Passenger)
			->addChild(spotLight6_Passenger)
			->addChild(spotLight7_Passenger)
			->addChild(spotLight8_Passenger)
			->addChild(passenger_Thruster1)
			->addChild(passenger_Thruster2)
			);

		addToRoot((new GameObject("Asteroid", PxVec3(0.0f, 0.0f, 0.0f), PxQuat(ToRadians(180.0f), PxVec3(0.0f, 1.0f, 0.0f).getNormalized())))
			->addGameComponent(new MeshRenderer(Mesh("Asteroids/Asteroid_C.obj", 50.0f), Material("asteroid")))
			->addGameComponent(new RigidBody(PxVec3(0), PxQuat(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 1.0f))
			->addGameComponent(new RogueAsteroid(10.0f, 0.005f)));

		//addToRoot((new GameObject("GameOver"))
		//	->addGUIComponent(new GUIButton(PxVec4(0.35f, 0.5f, 0.3f, 0.05f), PxVec4(0.0f), "Return To Mission Select",
		//	std::bind(&MissionOneScene::onRetryClick, this, std::placeholders::_1)))
		//	);

		addArea1OfMission1();

		//addArea2OfMission1();

		addGUI();
	}

private:
	void addGUI()
	{
		float barXPos = 0.02f;
		float barWidth = 0.25f;
		float barHeight = 0.1f;

		addToRoot((new GameObject("Health Bar Background"))
			->addGUIComponent(new GUIImage(PxVec4(barXPos, 0.05f, barWidth, barHeight), PxVec4(0.0f), "Images/BarBackground.png", 287.0f / 20.0f))
			->addChild((new GameObject("Health Bar"))
			->addGUIComponent(new GUIImage(PxVec4(0.05f, 0.18f, 12.7f, 0.62f), PxVec4(0.0f), "Images/healthbar.png"))));

		addToRoot((new GameObject("Energy Bar Background"))
			->addGUIComponent(new GUIImage(PxVec4(barXPos, 0.09f, barWidth, barHeight), PxVec4(0.0f), "Images/BarBackground.png", 287.0f / 20.0f))
			->addChild((new GameObject("Energy Bar"))
			->addGUIComponent(new GUIImage(PxVec4(0.05f, 0.18f, 12.7f, 0.62f), PxVec4(0.0f), "Images/energybar.png"))));

		addToRoot((new GameObject("Fuel Bar Background"))
			->addGUIComponent(new GUIImage(PxVec4(barXPos, 0.13f, barWidth, barHeight), PxVec4(0.0f), "Images/BarBackground.png", 287.0f / 20.0f))
			->addChild((new GameObject("Fuel Bar"))
			->addGUIComponent(new GUIImage(PxVec4(0.05f, 0.18f, 12.7f, 0.62f), PxVec4(0.0f), "Images/fuelbar.png"))));
	

		addToRoot((new GameObject("MiniMap"))
			->addGUIComponent(new GUIImage(PxVec4(0.0f, 0.70f, 0.3f, 0.3f), PxVec4(0.0f), "Images/minimap_graphics/GUI_MiniMap.png"))
			->addGameComponent(new MiniMap(PxVec2(0.15f, 0.15f), 3000.0f))
			);
	}

	void addArea1OfMission1()
	{
		// the alien fighter ship
		addToRoot((new GameObject("enemyFighter", PxVec3(20.0f, 0.0f, 1000.0f), PxQuat(0.0f, 0.0f, 0.0f, 1.0f), PxVec3(1.0f)))
			->addGameComponent(new MeshRenderer(Mesh("Ships/enemyBattleShip.obj", 10.0f), Material("alien_ship")))
			->addGameComponent(new RigidBody(PxVec3(0.0f, -5.0f, 80.0f), PxQuat(PxIdentity), 1.0f, 10.0f, 6.0f, 24.0f))
			->addGameComponent(new EnemyFighterShipAI)
			->addGameComponent(new ShipStats)
			);
		// //the alien fighter ship
		//addToRoot((new GameObject("enemyFighter", PxVec3(-20.0f, 0.0f, 3000.0f), PxQuat(0.0f, 0.0f, 0.0f, 1.0f), PxVec3(1.0f)))
		//	->addGameComponent(new MeshRenderer(Mesh("Ships/enemyBattleShip.obj", 10.0f), Material("alien_ship")))
		//	->addGameComponent(new RigidBody(PxVec3(0.0f, -5.0f, 80.0f), PxQuat(PxIdentity), 1.0f, 10.0f, 6.0f, 24.0f))
		//	->addGameComponent(new EnemyFighterShipAI)
		//	->addGameComponent(new ShipStats)
		//	);
		// //the alien fighter ship
		//addToRoot((new GameObject("enemyFighter", PxVec3(0.0f, 20.0f, 1000.0f), PxQuat(0.0f, 0.0f, 0.0f, 1.0f), PxVec3(1.0f)))
		//	->addGameComponent(new MeshRenderer(Mesh("Ships/enemyBattleShip.obj", 10.0f), Material("alien_ship")))
		//	->addGameComponent(new RigidBody(PxVec3(0.0f, -5.0f, 80.0f), PxQuat(PxIdentity), 1.0f, 10.0f, 6.0f, 24.0f))
		//	->addGameComponent(new EnemyFighterShipAI)
		//	->addGameComponent(new ShipStats)
		//	);
		//// the alien fighter ship
		//addToRoot((new GameObject("enemyFighter", PxVec3(0.0f, -20.0f, 3000.0f), PxQuat(0.0f, 0.0f, 0.0f, 1.0f), PxVec3(1.0f)))
		//	->addGameComponent(new MeshRenderer(Mesh("Ships/enemyBattleShip.obj", 10.0f), Material("alien_ship")))
		//	->addGameComponent(new RigidBody(PxVec3(0.0f, -5.0f, 80.0f), PxQuat(PxIdentity), 1.0f, 10.0f, 6.0f, 24.0f))
		//	->addGameComponent(new EnemyFighterShipAI)
		//	->addGameComponent(new ShipStats)
		//	);

		//// Planets and Moons
		//addToRoot((new GameObject("moon", PxVec3(1000.0f, 0.0f, 1000.0f)))
		//	->addGameComponent(new MeshRenderer(Mesh("Planets/Planet_A.obj", 32.0f), Material("moon")))
		//	);

		//addToRoot((new GameObject("earth", PxVec3(-1000.0f, 0.0f, 0.0f)))
		//	->addGameComponent(new MeshRenderer(Mesh("Planets/Planet_B.obj", 100.0f), Material("earth")))
		//	);

		//addToRoot((new GameObject("mars", PxVec3(7500.0f, 0.0f, 7500.0f)))
		//	->addGameComponent(new MeshRenderer(Mesh("Planets/Planet_C.obj", 35.0f), Material("mars")))
		//	);

	}

	void addArea2OfMission1()
	{
		// TODO : Optimize AsteroidField class

		// Asteroids
		//addToRoot((new GameObject("Asteroid Field", PxVec3(0.0f, 0.0f, 4000.0f)))
		//	->addGameComponent(new AsteroidField())
		//	);
		//addToRoot((new GameObject("Asteroid Field", PxVec3(0.0f, 5.0f, 4500.0f)))
		//	->addGameComponent(new AsteroidField())
		//	);

		addToRoot((new GameObject("jupiterMoon1", PxVec3(1000.0f, 0.0f, 10300.0f)))
			->addGameComponent(new MeshRenderer(Mesh("Planets/Planet_D.obj", 75.0f), Material("moon")))
			);

		addToRoot((new GameObject("jupiterMoon2", PxVec3(-900.0f, 0.0f, 9800.0f)))
			->addGameComponent(new MeshRenderer(Mesh("Planets/Planet_E.obj", 20.0f), Material("mars")))
			);

		addToRoot((new GameObject("jupiterMoon3", PxVec3(-800.0f, 0.0f, 11000.0f)))
			->addGameComponent(new MeshRenderer(Mesh("Planets/Planet_F.obj", 32.0f), Material("moon")))
			);

		addToRoot((new GameObject("jupiter", PxVec3(0.0f, 0.0f, 10000.0f)))
			->addGameComponent(new MeshRenderer(Mesh("Planets/Planet_G.obj", 200.0f), Material("jupiter")))
			);

		addToRoot((new GameObject("sun", PxVec3(20000.0f, 5000.0f, 20000.0f)))
			->addGameComponent(new MeshRenderer(Mesh("Planets/Planet_A.obj", 400.0f), Material("sun")))
			);
	}

private:

	//bool onRetryClick(const GameObject& go)
	//{
	//	getCoreEngine()->getSceneManager()->pop();
	//	return true;
	//}
};
