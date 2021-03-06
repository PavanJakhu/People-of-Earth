// ***********************************************************************
// Author           : Christopher Maeda
// Created          : 09-15-2015
//
// Last Modified By : Christopher Maeda
// Last Modified On : 03-18-2016
// ***********************************************************************
// <copyright file="Collider.cpp" company="Team MegaFox">
//     Copyright (c) Team MegaFox. All rights reserved.
// </copyright>
// <summary>
// Base abstract class that has all the functionality to handle collision.
// This class will know what type of collider it is.
// It will have basic physics representation in the world such as position.
// It has it's own ID number to know if it is not check itself for collision or part of another collider
// It also has a bool flag to know if this class collided or not
// </summary>
// ***********************************************************************
#include "Collider.h"

Collider::Collider()
{
}

Collider::Collider(Collider& collider)
{
	m_position = collider.m_position;
	m_rotation = collider.m_rotation;
	m_scale = collider.m_scale;
	m_mass = collider.m_mass;
	m_velocity = collider.m_velocity;
	m_acceleration = collider.m_acceleration;
	m_id = collider.m_id;
}

Collider::~Collider()
{
}

void Collider::init(
		PxVec3 position,
		PxQuat rotation,
		float scale,
		float mass,
		PxVec3 velocity,
		PxVec3 acceleration,
        int id
		) 
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
	m_mass = mass;
	m_velocity = velocity;
	m_acceleration = acceleration;
    if (id == 0)
	{
		createUniqueID();
	}
	else
	{
		m_id = id;
	}
}

void Collider::applyRotation(PxQuat rotation)
{
    m_rotation *= rotation;
	m_rotation = m_rotation.getNormalized();
}

void Collider::applyForce(PxVec3 force)
{
	//Newton's Second Law
	//Force = mass * acceleration 
	//Convert that formula to solve the accerleration instead
	//acceleration = Force / mass
	m_acceleration += force / m_mass;
}

void Collider::applyAcceleration(PxVec3 accel)
{
	m_acceleration += accel;
}

void Collider::createUniqueID()
{
	//Create a static variable 
	static int namer = 1;
	//Set the name
	m_id = namer;
	//Increment the static variable value
	namer++;
}

void Collider::update(float timeStep)
{
    //Displacement formula (Kinematic formula) split into 2 part because of loss of memory and number
	PxVec3 displacement = m_velocity * timeStep + 0.5f * m_acceleration * timeStep * timeStep;

    //Velocity formula (Kinematic formula)
	m_velocity += m_acceleration * timeStep;

	//Damping on the velocity (friction)
	m_velocity *= 0.999f;

	//Have it update the position a frame more
	//This is to prevent it so the object does not collide again because position did not move so be stuck in one spot (loop)
	if (m_acceleration != PxVec3(0) && displacement.magnitude() == 0)
	{
		displacement = m_velocity * timeStep + 0.5f * m_acceleration * timeStep * timeStep;
	}

	//Update the position
	m_position += displacement;

	//Reset the accleration
	m_acceleration = PxVec3(0.0f, 0.0f, 0.0f);
	//m_velocity = PxVec3(0);
}
