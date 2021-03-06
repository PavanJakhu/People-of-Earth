// ***********************************************************************
// Author           : Christopher Maeda
// Created          : 09-15-2015
//
// Last Modified By : Pavan Jakhu
// Last Modified On : 01-24-2016
// ***********************************************************************
// <copyright file="PhysicsEngine.h" company="Team MegaFox">
//     Copyright (c) Team MegaFox. All rights reserved.
// </copyright>
// <summary>
// </summary>
// ***********************************************************************
#pragma once
#include "World.h"
class Camera3D;

/// <summary>
/// The Physics Engine class that updates the physics world.
/// </summary>
class PhysicsEngine
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="PhysicsEngine"/> class.
	/// </summary>
	PhysicsEngine();
	/// <summary>
	/// Finalizes an instance of the <see cref="PhysicsEngine"/> class.
	/// </summary>
	~PhysicsEngine();

	/// <summary>
	/// Updates the physics engine.
	/// </summary>
	/// <param name="timeStep">The time step.</param>
	void updatePhysicsEngine(float timeStep);

	/// <summary>
	/// Gets the physics world.
	/// </summary>
	/// <returns>World *.</returns>
	static World* getPhysicsWorld()
	{
		return m_physicsWorld;
	}

	/// <summary>
	/// Sets the main camera in the scene using this physics world.
	/// </summary>
	/// <param name="camera">The camera.</param>
	void setMainCamera(const Camera3D& camera);

private:
	/// <summary>
	/// The physics world.
	/// </summary>
	static World *m_physicsWorld;

	/// <summary>
	/// The main camera in this physics world
	/// </summary>
	const Camera3D* m_mainCamera;
};

