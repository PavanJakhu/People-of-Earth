#pragma once
#include <MegaEngine.h>
#include <glm\glm.hpp>
#include <iostream>

#include "FreeLook.h"
#include "FreeMove.h"
#include "PlanetSpin.h"
//#include "Klingon.h"

class TestScene : public Scene
{
public:
	TestScene() : Scene("Hello World") { }
	~TestScene() { }

	virtual void init(const Viewport& window) override
	{
		//getPhysicsWorld().init(glm::vec3(0, 0, 0), 100);
		Material bricks("bricks", 0.5f, 4, Texture("bricks.jpg"), Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);
		Material humanShip("human_ship", 0.5f, 4, Texture("Human-Ship-UVWs.png"));
		Material alienShip("alien_ship", 0.5f, 4, Texture("AlienshipUVWs.png"));
		Material skySphereTex("skySphereTexture", 0.5f, 4, Texture("night_sky.png"));
		Material earthTex("earthTexture", 0.5f, 4, Texture("earth.png"));
		Material sunTex("sunTexture", 0.5f, 4, Texture("sun.jpg"));
		Material moonTex("moonTexture", 0.5f, 4, Texture("moon.jpg"));

		// AudioListener
		AudioListener bill;
		AudioSource * theListener = new AudioSource(bill);

		// Stream of Music Passed through a AudioSource and into the AudioEngine
		Stream bob("./Assets/Music/music.mp3");
		AudioSource * ambientSounds = new AudioSource(bob);

		// The human fighter ship and camera
		GameObject* camera =
			(new GameObject)
			->addGameComponent(new CameraComponent(glm::perspective(glm::radians(75.0f), window.getAspectRatio(), 0.1f, 1000.0f)))
			->addGameComponent(new FreeLook(window.getCenter()))
			->addGameComponent(new FreeMove(50.0f));
		GameObject* fighterShip = 
			(new GameObject(glm::vec3(-2.0f, -4.0f, -10.0f), glm::quat(glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)))))
			->addGameComponent(theListener)
			->addGameComponent(new MeshRenderer(Mesh("HumanFighter_Final.obj", 0.1f), Material("human_ship")));

		//theListener->setAsListener();
		camera->addChild(fighterShip);
		addToRoot(camera);

		// the alien fighter ship
		addToRoot((new GameObject(glm::vec3(0.0f, -5.0f, 80.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(4.0f)))
			->addGameComponent(new MeshRenderer(Mesh("AlienFighter_FINAL.obj", 0.1f), Material("alien_ship"))));

		// the second human fighter ship
		addToRoot((new GameObject(glm::vec3(0.0f, 15.0f, 80.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(4.0f)))
			->addGameComponent(new MeshRenderer(Mesh("HumanFighter_Final.obj", 0.1f), Material("human_ship"))));

		// The skysphere
		addToRoot((new GameObject(glm::vec3(0.0f, -5.0f, 80.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(4000.0f)))
			->addGameComponent(new MeshRenderer(Mesh("sphere2.obj", 0.1f), Material("skySphereTexture")))
			->addGameComponent(new FreeMove()));

		// The Earth
		GameObject * earth = 
		(new GameObject(glm::vec3(0.0f, -5.0f, 550.0f), glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(400.0f)))
			->addGameComponent(new MeshRenderer(Mesh("sphere.obj", 0.1f), Material("earthTexture")))
			->addGameComponent(new PlanetSpin);
		addToRoot(earth);

		addToRoot((new GameObject(glm::vec3(0.0f, -5.0f, 4550.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(150.0f)))
			->addGameComponent(new MeshRenderer(Mesh("sphere.obj", 0.1f), Material("moonTexture"))));

		// The Sun
		addToRoot((new GameObject(glm::vec3(0.0f, -5.0f, -55000.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1000.0f)))
			->addGameComponent(new MeshRenderer(Mesh("sphere.obj", 0.1f), Material("sunTexture")))
			->addGameComponent(ambientSounds));



		addToRoot((new GameObject(glm::vec3(0.0f), glm::quat(glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0)))))
			->addGameComponent(new DirectionalLight(glm::vec3(1.0f), 0.02f, 7, 8.0f, 1.0f)));
		
		
		ambientSounds->setStream("./Assets/Music/music.mp3");
		ambientSounds->setStream3DMinDist(1.0f);
		ambientSounds->setStreamDoppler(0.5f);
		ambientSounds->setStreamPosition(glm::vec3(0.0f, -5.0f, 550.0f));
		ambientSounds->playStream(true);

		/*
		
		The Listener in the scene must be updated every frame, this allows for its information to be updated.
		But how can i do that...?
		Create a Script that is the listener and set its position in the update method overriden from game component
		then attach that script to the listener in the scene.
		
		*/

		if (ambientSounds->isStreamPlaying())
		{
			printf("yup\n");
		}

		CameraComponent* cc = camera->getComponent<CameraComponent>();
		if (cc != nullptr)
		{
			std::cout << "There is a camera component!" << std::endl;
		}

	}
};

