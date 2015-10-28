#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\quaternion.hpp>

class GameObject;

class Transform
{
public:

	Transform(const glm::vec3 & position = glm::vec3(0.0f),
		const glm::vec3 & rotation = glm::vec3(0.0f),
		const glm::vec3 & scale = glm::vec3(1.0f));

	bool hasChanged();
	glm::mat4 getModel();
	const glm::mat4 getParentMatrix();
	glm::quat getTransformedRot();

	// Getters
	GameObject* getAttachedGameObject() { return m_attached; }
	Transform* getParent() { return m_parent; }
	glm::vec3 & getPosition() { return m_pos; }
	glm::quat & getRotation() { return m_rot; }
	inline glm::vec3 & getScale() { return m_scale; }

	// Setters
	void setAttachedGameObject(GameObject* attach) { m_attached = attach; }
	void setParent(Transform* parent) { m_parent = parent; }
	void setPosition(const glm::vec3 & pos) { m_pos = pos; }
	void setRotation(const glm::quat & rot) { m_rot = rot; }
	void setScale(const glm::vec3 & scale) { m_scale = scale; }

private:
	glm::vec3 m_pos, m_scale;
	glm::quat m_rot;

	GameObject* m_attached;
	Transform* m_parent;
	mutable glm::mat4 m_parentMatrix;

	mutable glm::vec3 m_oldPos;
	mutable glm::quat m_oldRot;
	mutable float m_oldScale;
	mutable bool m_initializedOldStuff;

};