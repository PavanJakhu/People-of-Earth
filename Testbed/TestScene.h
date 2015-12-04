#pragma once
#include <Scene.h>
#include <GameObject.h>
#include <MeshRenderer.h>
#include <DirectionalLight.h>
#include <Camera3D.h>
#include <glm\glm.hpp>

#include "FreeLook.h"
#include "FreeMove.h"

class TestScene : public Scene
{
public:
	TestScene() : Scene("Hello World") { }
	~TestScene() { }

	virtual void init(const Viewport& window) override
	{
		Material bricks("bricks", Texture("bricks.jpg"), 0.5f, 4, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);

		addToRoot((new GameObject(glm::vec3(0.0f, -1.0f, 5.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(4.0f)))
			->addGameComponent(new MeshRenderer(Mesh("plane4.obj"), Material("bricks"))));

		// Added only to prove we are rotating around the origin of the world /////////////////////////////////////////
		Material uhm("uhm", Texture("defaultTexture.png"), 0.5f, 4, Texture("default_normal.jpg"), Texture("default_disp.png"), 0.03f, -0.5f);
		addToRoot((new GameObject(glm::vec3(5.0f, -1.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(4.0f)))
			->addGameComponent(new MeshRenderer(Mesh("sphere.obj"), Material("uhm")))
			->addGameComponent(new FreeLook(window.getCenter())));
		/////////////////////////////////////////////////////////////////////////////////////////////

		addToRoot((new GameObject)
			->addGameComponent(new CameraComponent(glm::perspective(ToRadians(75.0f), window.getAspectRatio(), 0.1f, 1000.0f)))
			->addGameComponent(new FreeMove()));

		addToRoot((new GameObject(glm::vec3(0.0f), glm::quat(ToRadians(45.0f), glm::vec3(1, 0, 0))))
			->addGameComponent(new DirectionalLight(glm::vec3(1, 1, 1), 0.4f, 10, 80.0f, 1.0f)));

	}
};

