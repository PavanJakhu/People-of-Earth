// ***********************************************************************
// Author           : Pavan Jakhu, Jesse Derochie and Christopher Maeda
// Created          : 09-15-2015
//
// Last Modified By : Pavan Jakhu
// Last Modified On : 03-21-2016
// ***********************************************************************
// <copyright file="GameObject.h" company="Team MegaFox">
//     Copyright (c) Team MegaFox. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#pragma once
#include <vector>
#include <string>
#include "Transform.h"
#include "InputManager.h"
#include <PhysX/PxPhysicsAPI.h>
using namespace physx;

class Camera3D;
class CoreEngine;
class GameComponent;
class GUIComponent;
class Shader;
class RenderingEngine;
class GUIEngine;

/// <summary>
/// The entity object placed in the scene.
/// Also stores the game components like Mesh Renderer and Cameras.
/// </summary>
class GameObject
{
public:

	/// <summary>
	/// Constructs the GameObject and its position, rotation and scale.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="pos">The position of the GameObject.</param>
	/// <param name="rot">The rotation of the GameObject.</param>
	/// <param name="scale">The scale of the GameObject.</param>
	GameObject(
		const std::string& name, 
		const PxVec3& pos = PxVec3(0.0f, 0.0f, 0.0f), 
		const PxQuat& rot = PxQuat(PxIdentity),
		const PxVec3& scale = PxVec3(1.0f, 1.0f, 1.0f))
		: m_name(name), m_enabled(true), m_transform(pos, rot, scale), m_coreEngine(nullptr) 
	{
		m_transform.setAttachedGameObject(this);
	}


	/// <summary>
	/// Finalizes an instance of the <see cref="GameObject" /> class.
	/// </summary>
	~GameObject();

	/// <summary>
	/// Notifies the game components and the object's children that 
	/// they are being covered by a pop scene.
	/// </summary>
	void notifyCoveredComponents();
	/// <summary>
	/// Notifies the game components and the object's children that 
	/// they are no longer being covered by a pop scene.
	/// </summary>
	void notifyUncoveredComponents();

	/// <summary>
	/// Updates all children GameObjects, Game Components and GUI Components.
	/// </summary>
	/// <param name="delta">The delta time between frames.</param>
	void updateAll(float delta);
	/// <summary>
	/// Renders all children GameObjects, Game Components and GUI Components.
	/// </summary>
	/// <param name="shader">The shader program.</param>
	/// <param name="guiEngine">The GUI Engine object.</param>
	/// <param name="renderingEngine">The Rendering Engine object.</param>
	/// <param name="camera">The main active camera.</param>
	void renderAll(const Shader& shader, const RenderingEngine& renderingEngine, const Camera3D& camera) const;
	/// <summary>
	/// Processes all inputs for the children GameObjects, Game Components and GUI Components.
	/// </summary>
	/// <param name="input">The manager to get inputs from the viewport.</param>
	/// <param name="delta">The delta time between frames.</param>
	void processAll(const InputManager& input, float delta);

	/// <summary>
	/// Activates this game object (used for GUI Components)
	/// </summary>
	void activate();

	/// <summary>
	/// Deactivates this game object (used for GUI Components)
	/// </summary>
	void deactivate();

	/// <summary>
	/// Adds a child GameObject then returns the child GameObject.
	/// </summary>
	/// <param name="child">The child GameObject to add.</param>
	/// <returns>Returns the GameObject added.</returns>
	GameObject* addChild(GameObject* child);
	/// <summary>
	/// Attaches a Game Component to the GameObject.
	/// </summary>
	/// <param name="component">The Game Component to attach.</param>
	/// <param name="callOnStart">Wheather or no the onStart method is called. 
	/// DO NOT set to true if adding in the scene init function.</param>
	/// <returns>Returns the GameObject the component is attached too.</returns>
	GameObject* addGameComponent(GameComponent* component, bool callOnStart = false);
	/// <summary>
	/// Attaches a GUI Component to the GameObject.
	/// </summary>
	/// <param name="component">The GUI Component to attach.</param>
	/// <returns>Returns the GameObject the component is attached too.</returns>
	GameObject* addGUIComponent(GUIComponent* component);

	/// <summary>
	/// Removes the specified GameObject
	/// </summary>
	/// <param name="child">The GameObject to remove.</param>
	/// <returns>If the GameObject was remvoed successfully.</returns>
	bool removeChild(GameObject* child);
	/// <summary>
	/// Removes the specified Game Components.
	/// </summary>
	/// <param name="component">The Game Component to remove.</param>
	/// <returns>If the Game Component was removed successfully.</returns>
	bool removeGameComponent(GameComponent* component, bool del = true);
	/// <summary>
	/// Removes the specified GUI Components.
	/// </summary>
	/// <param name="component">The GUI Component to remove.</param>
	/// <returns>If the GUI Component was removed successfully.</returns>
	bool removeGUIComponent(GUIComponent* component);


	/// <summary>
	/// Gets the Game Component if it is attached.
	/// </summary>
	/// <returns>The specified Game Component if it is attached to the GameObject. Otherwise it returns a nullptr.</returns>
	template<typename T> T* getGameComponent() const
	{
		if (!m_gameComponents.empty())
		{
			for (size_t i = 0; i < m_gameComponents.size(); i++)
			{
				T* check = dynamic_cast<T*>(m_gameComponents[i]);
				if (check != nullptr)
				{
					return check;
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Gets the GUI Component if it is attached.
	/// </summary>
	/// <returns>The specified GUI Component if it is attached to the GameObject. Otherwise it returns a nullptr.</returns>
	template<typename T> T* getGUIComponent() const
	{
		if (!m_guiComponents.empty())
		{
			for (size_t i = 0; i < m_guiComponents.size(); i++)
			{
				T* check = dynamic_cast<T*>(m_guiComponents[i]);
				if (check != nullptr)
				{
					return check;
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Gets all sub-children GameOBjects and stores them into a vector.
	/// </summary>
	/// <returns>All sub-children GameObject pointers stored in a vector.</returns>
	std::vector<GameObject*> getAllAttached();
	/// <summary>
	/// Gets all immediate children and stores them into a vector.
	/// </summary>
	/// <returns>All immediate children stored in a vector.</returns>
	std::vector<GameObject*> getAllChildren();

	/// <summary>
	/// Gets all game components attached to the game object into a vector.
	/// </summary>
	/// <returns>All game components attached.</returns>
	std::vector<GameComponent*> getAllGameComponents() const;

	/// <summary>
	/// Gets the core engine.
	/// </summary>
	/// <returns>CoreEngine *.</returns>
	CoreEngine* getCoreEngine() const { return m_coreEngine; }

	/// <summary>
	/// Gets the Transform object of this GameObject.
	/// </summary>
	/// <returns>A pointer to the Transform object.</returns>
	Transform* getTransform() { return &m_transform; }

	/// <summary>
	/// Gets the name of the GameObject.
	/// </summary>
	/// <returns>The string value of the GameObject name.</returns>
	std::string getName() const { return m_name; }

	/// <summary>
	/// Checks if the gameobject is enabled.
	/// </summary>
	/// <returns>Whether the gameobject is enabled.</returns>
	bool isEnabled() const { return m_enabled; }

	/// <summary>
	/// Checks if the gameobject was enabled before an exclusive scene was pushed.
	/// Mostly used by the Scene Manager.
	/// </summary>
	/// <returns>Whether the gameobject is enabled.</returns>
	bool wasEnabled() const { return m_wasEnabled; }
	
	/// <summary>
	/// Sets the Core Engine so the GameObject can access the different systems.
	/// </summary>
	/// <param name="engine">The pointer to the Core Engine object.</param>
	void setEngine(CoreEngine* engine);

	/// <summary>
	/// Sets the name of this game object.
	/// </summary>
	/// <param name="name">The name.</param>
	void setName(const std::string& name) { m_name = name; }

	/// <summary>
	/// Sets the gameobject to be enabled or not.
	/// </summary>
	/// <param name="enabled">If the gameobject and its children is enabled.</param>
	/// <param name="childrenEnabled">If the gameobject's children should be enabled or not.</param>
	void setEnabled(const bool enabled, const bool childrenEnabled = true);

	/// <summary>
	/// Sets the gameobject to know it was enabled before an exclusive scene was pushed.
	/// </summary>
	/// <param name="enabled">If the gameobject was enabled.</param>
	void setWasEnabled(const bool enabled);

private:
	/// <summary>
	/// Updates all the Game Components.
	/// </summary>
	/// <param name="delta">The delta time between frames.</param>
	void updateGameComponents(float delta);
	/// <summary>
	/// Renders all the Game Components.
	/// </summary>
	/// <param name="shader">The shader program.</param>
	/// <param name="renderingEngine">The Rendering Engine object.</param>
	/// <param name="camera">The main active camera.</param>
	void renderGameComponents(const Shader& shader, const RenderingEngine& renderingEngine, const Camera3D& camera) const;
	/// <summary>
	/// Processes all inputs for the Game Components.
	/// </summary>
	/// <param name="input">The manager to get inputs from the viewport.</param>
	/// <param name="delta">The delta time between frames.</param>
	void processInputGameComponents(const InputManager& input, float delta);

	/// <summary>
	/// Updates all the GUI Components.
	/// </summary>
	/// <param name="delta">The delta time between frames.</param>
	void updateGUIComponents(float delta);
	/// <summary>
	/// Processes all inputs for the GUI Components.
	/// </summary>
	/// <param name="input">The manager to get inputs from the viewport.</param>
	/// <param name="delta">The delta time between frames.</param>
	void processInputGUIComponents(const InputManager& input, float delta);

	/// <summary>
	/// The name of the GameObject.
	/// </summary>
	std::string m_name;
	/// <summary>
	/// If the game object, it's components and children are accepting input, updating and rendering.
	/// </summary>
	bool m_enabled;
	bool m_wasEnabled;
	/// <summary>
	/// The vector of child GameObjects.
	/// </summary>
	std::vector<GameObject*> m_children;
	/// <summary>
	/// The vector of attached Game Components.
	/// </summary>
	std::vector<GameComponent*> m_gameComponents;
	/// <summary>
	/// The vector of attached GUI Components.
	/// </summary>
	std::vector<GUIComponent*> m_guiComponents;
	/// <summary>
	/// The Transform object for the GameObject.
	/// </summary>
	Transform m_transform;
	/// <summary>
	/// A pointer to the Core Engine object.
	/// </summary>
	CoreEngine* m_coreEngine;
};

