#include "Utility.h"
#include <SDL2\SDL.h>

void Utility::sleep(int milliseconds)
{
	SDL_Delay(milliseconds);
}

std::vector<std::string> Utility::split(const std::string& s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = (unsigned int)s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}

const PxVec3 Utility::rotateQuatByVec(const PxQuat& quat, const PxVec3& v)
{
	/*const glm::f32 vx = 2.0f*v.x;
	const glm::f32 vy = 2.0f*v.y;
	const glm::f32 vz = 2.0f*v.z;
	const glm::f32 w2 = quat.w*quat.w - 0.5f;
	const glm::f32 dot2 = (quat.x*vx + quat.y*vy + quat.z*vz);
	return glm::vec3
		(
		(vx*w2 + (quat.y * vz - quat.z * vy)*quat.w + quat.x*dot2),
		(vy*w2 + (quat.z * vx - quat.x * vz)*quat.w + quat.y*dot2),
		(vz*w2 + (quat.x * vy - quat.y * vx)*quat.w + quat.z*dot2)
		);*/
	return PxVec3(-1, -1, -1);
}

float Utility::maxVec3(const PxVec3 & vec)
{
	float Max = vec[0];

	for (int i = 0; i < 3; i++)
		if (vec[i] > Max)
			Max = vec[i];

	return Max;
}

float Utility::maxVec4(const PxVec4 & vec)
{
	float Max = vec[0];

	for (int i = 0; i < 4; i++)
		if (vec[i] > Max)
			Max = vec[i];

	return Max;
}

PxVec3 Utility::getForward(const PxQuat& quat)
{
	return quat.rotate(PxVec3(0, 0, 1));
}

PxVec3 Utility::getBack(const PxQuat& quat)
{
	return quat.rotate(PxVec3(0, 0, -1));
}

PxVec3 Utility::getUp(const PxQuat& quat)
{
	return quat.rotate(PxVec3(0, 1, 0));
}

PxVec3 Utility::getDown(const PxQuat& quat)
{
	return quat.rotate(PxVec3(0, -1, 0));
}

PxVec3 Utility::getRight(const PxQuat& quat)
{
	return quat.rotate(PxVec3(1, 0, 0));
}

PxVec3 Utility::getLeft(const PxQuat& quat)
{
	return quat.rotate(PxVec3(-1, 0, 0));
}

PxMat44 Utility::initTranslation(PxVec3 translation)
{
	return PxMat44(PxVec3(1, 0, 0), PxVec3(0, 1, 0), PxVec3(0, 0, 1), translation);
}

PxMat44 Utility::initScale(PxVec3 scale)
{
	return PxMat44(PxVec4(scale, 1));
}

PxMat44 Utility::initRotationFromDirection(const PxVec3& target, const PxVec3& up)
{
	PxVec3 n = target;
	n.normalize();

	PxVec3 u = up;
	u.normalize();
	u = u.cross(n);

	PxVec3 v = n.cross(u);
	PxMat44 m;
	m[0][0] = u.x;   m[1][0] = u.y;   m[2][0] = u.z;   m[3][0] = 0.0f;
	m[0][1] = v.x;   m[1][1] = v.y;   m[2][1] = v.z;   m[3][1] = 0.0f;
	m[0][2] = n.x;   m[1][2] = n.y;   m[2][2] = n.z;   m[3][2] = 0.0f;
	m[0][3] = 0.0f;  m[1][3] = 0.0f;  m[2][3] = 0.0f;  m[3][3] = 1.0f;
	return m;
}

PxMat44 Utility::initPerspective(float fov, float aspect, float zNear, float zFar)
{
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(fov / 2.0f);

	return PxMat44(
		PxVec4(1.0f / (tanHalfFOV * aspect), 0.0f, 0.0f, 0.0f),
		PxVec4(0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f),
		PxVec4(0.0f, 0.0f, (-zNear - zFar) / zRange, 1.0f),
		PxVec4(0.0f, 0.0f, 2.0f * zFar * zNear / zRange, 0.0f));
}

PxMat44 Utility::initOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	const float width = (right - left);
	const float height = (top - bottom);
	const float depth = (far - near);

	PxMat44 m;

	m[0][0] = 2.0f / width;	m[1][0] = 0.0f;				m[2][0] = 0.0f;				m[3][0] = -(right + left) / width;
	m[0][1] = 0.0f;			m[1][1] = 2.0f / height;	m[2][1] = 0.0f;				m[3][1] = -(top + bottom) / height;
	m[0][2] = 0.0f;			m[1][2] = 0.0f;				m[2][2] = -2.0f / depth;	m[3][2] = -(far + near) / depth;
	m[0][3] = 0.0f;			m[1][3] = 0.0f;				m[2][3] = 0.0f;				m[3][3] = 1.0f;

	return m;
}
