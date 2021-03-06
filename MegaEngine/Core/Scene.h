// ***********************************************************************
// Author           : Pavan Jakhu, Jesse Derochie and Christopher Maeda
// Created          : 09-15-2015
//
// Last Modified By : Pavan Jakhu
// Last Modified On : 03-21-2016
// ***********************************************************************
// <copyright file="Scene.h" company="Team MegaFox">
//     Copyright (c) Team MegaFox. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "GameObject.h"
#include "..\Rendering\Viewport.h"
class CoreEngine;
class RenderingEngine;
class InputManager;


/// <summary>
/// An abstract class that holds a root GameObject for the scene graph.
/// </summary>
class Scene
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="Scene"/> class.
	/// </summary>
	/// <param name="name">The name.</param>
	Scene(const std::string& name);
	/// <summary>
	/// Finalizes an instance of the <see cref="Scene"/> class.
	/// </summary>
	virtual ~Scene();

	/// <summary>
	/// Initializes the scene with GameObjects.
	/// </summary>
	/// <param name="window">The window.</param>
	virtual void init(const Viewport& window) = 0;
	/// <summary>
	/// Notifies all the game objects in the scene that they
	/// are being covered by a pop up scene.
	/// </summary>
	void notifyCoveredObjects();
	/// <summary>
	/// Notifies all the game objects in the scene that they
	/// are being uncovered from a pop scene.
	/// </summary>
	void notifyUncoveredObjects();
	/// <summary>
	/// Updates the scene calling GameObject's update.
	/// </summary>
	/// <param name="delta">The frame time delta.</param>
	void update(float delta);
	/// <summary>
	/// Renders the scene calling GameObject's render.
	/// </summary>
	/// <param name="renderingEngine">The rendering engine.</param>
	void render(RenderingEngine* renderingEngine);
	/// <summary>
	/// Processes the input by calling GameObject's processInput.
	/// </summary>
	/// <param name="input">The input.</param>
	/// <param name="delta">The frame time delta.</param>
	void processInput(const InputManager& input, float delta);

	/// <summary>
	/// Adds a GameObject to the root GameObject.
	/// </summary>
	/// <param name="gameobject">The gameobject.</param>
	void addToRoot(GameObject* gameobject);
	/// <summary>
	/// Removes the GameOject from the root.
	/// </summary>
	/// <param name="gameobject">The gameobject.</param>
	/// <returns>If the GameObject was removed.</returns>
	bool removeGameObject(GameObject* gameobject);

	/// <summary>
	/// Gets all game objects.
	/// </summary>
	/// <returns>All attached Objects to the root.</returns>
	std::vector<GameObject*> getAllGameObjects() { return m_root.getAllAttached(); }

	/// <summary>
	/// Gets the name.
	/// </summary>
	/// <returns>The string value.</returns>
	std::string getName() { return m_name; }

	/// <summary>
	/// Gets the root.
	/// </summary>
	/// <returns></returns>
	GameObject * getRoot() { return &m_root; }

	/// <summary>
	/// Gets the name counter (used to ensure no to game objects are named the same thing)
	/// </summary>
	/// <param name="name">The name.</param>
	/// <returns></returns>
	Uint16 getNameCounter(const std::string& name);

	/// <summary>
	/// Creates the specified game object and adds it to the root
	/// </summary>
	/// <param name="gameObject">The game object to create.</param>
	void instantiate(GameObject* gameObject);

	/// <summary>
	/// Sets the Core Engine.
	/// </summary>
	/// <param name="engine">The engine.</param>
	void setEngine(CoreEngine* engine) { 
		m_coreEngine = engine; 
		m_root.setEngine(engine); 
	}

protected:
	/// <summary>
	/// Gets a handle to the core engine.
	/// </summary>
	/// <returns></returns>
	CoreEngine* getCoreEngine() const { return m_coreEngine; }

private:
	
	/// <summary>
	/// The map that stores the names of game objects and the number of times
	/// that particular name is used, this map is used to ensure that no two
	/// game objects have an identical name, by numbering the repetitions
	/// </summary>
	std::unordered_map<std::string, Uint16> m_GONameCounter;

	/// <summary>
	/// The name of the Scene.
	/// </summary>
	std::string m_name;
	/// <summary>
	/// The root GameObject.
	/// </summary>
	GameObject m_root;
	/// <summary>
	/// A pointer to the Core Engine.
	/// </summary>
	CoreEngine* m_coreEngine;
};

