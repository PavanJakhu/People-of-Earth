#include "PlayerShipMovementController.h"

/*
	Player Ship Movement Controller Macros
*/
#define COS_ANGLE(x) glm::cos(glm::radians(0.5f * x))
#define SIN_ANGLE(x) glm::sin(glm::radians(0.5f * x))
#define ROTATE_X_AXIS(x) glm::quat(COS_ANGLE(x), 0, SIN_ANGLE(x), 0)
#define ROTATE_Y_AXIS(x) glm::quat(COS_ANGLE(x), SIN_ANGLE(x), 0, 0)
#define ROTATE_Z_AXIS(x) glm::quat(COS_ANGLE(x), 0, 0, SIN_ANGLE(x))
#define SHIP_ROTATION(x, y)	glm::quat(						\
		glm::cos(glm::radians(0.5f * (x + y))),				\
		glm::sin(glm::radians(0.5f * x)),					\
		glm::sin(glm::radians(0.5f * y)), 0					\
)		

PlayerShipMovementController::PlayerShipMovementController(const std::string & nameOfCameraInstance, float velocityValue, float accelerationValue)
{
	m_velocityValue = velocityValue;
	m_accelerationValue = accelerationValue;
	m_cameraInstanceName = nameOfCameraInstance;
}

PlayerShipMovementController::~PlayerShipMovementController()
{
}

void PlayerShipMovementController::onStart()
{
	m_rigidBody = getParent()->getGameComponent<RigidBody>();
	m_camera = getGameObjectByName(m_cameraInstanceName)->getGameComponent<CameraComponent>();
	m_distance = glm::length((m_rigidBody->getPosition() - *m_camera->getTransform()->getPosition()));
	m_forwardDirection = Utility::getForward(m_rigidBody->getRotation());
}

void PlayerShipMovementController::processInput(const InputManager& input, float delta)
{
	lookAround(input);

	movement(input, delta);
	
	checkLerp(input);
	
	if (m_lerp_X_Axis_Ship || m_lerp_Y_Axis_Ship)
	{
		lerp();
	}

	//Updates the camera position from the rigidbody
	m_camera->getTransform()->setPosition(m_rigidBody->getPosition() - (m_distance * m_forwardDirection));

}

void PlayerShipMovementController::lookAround(const InputManager& input)
{
	if (input.GetThumbRPosition().x > 0.3f)
	{
		returnToActualRotation();

		m_rigidBody->updateRotation(ROTATE_X_AXIS(-1.0f));

		m_forwardDirection = Utility::getForward(m_rigidBody->getRotation());

		//Rotates the camera view
		m_camera->getTransform()->setRotation(*m_camera->getTransform()->getRotation() * ROTATE_X_AXIS(-1.0f));

		if (m_shipsVisualRotation.y > -15.0f)
		{
			m_shipsVisualRotation.y--;
		}

		showVisualShipRotation();

		m_lerp_Y_Axis_Ship = false;
	}

	if (input.GetThumbRPosition().x < -0.3f)
	{
		returnToActualRotation();
		m_rigidBody->updateRotation(ROTATE_X_AXIS(1.0f));

		m_forwardDirection = Utility::getForward(m_rigidBody->getRotation());

		//Rotates the camera view
		m_camera->getTransform()->setRotation(*m_camera->getTransform()->getRotation() * ROTATE_X_AXIS(1.0f));
		
		if (m_shipsVisualRotation.y < 15.0f)
		{
			m_shipsVisualRotation.y++;
		}
		showVisualShipRotation();
		m_lerp_Y_Axis_Ship = false;
	}

	if (input.GetThumbRPosition().y > 0.3f)
	{
		returnToActualRotation();

		m_rigidBody->updateRotation(ROTATE_Y_AXIS(1.0f));

		m_forwardDirection = Utility::getForward(m_rigidBody->getRotation());

		//Rotates the camera view
		m_camera->getTransform()->setRotation(*m_camera->getTransform()->getRotation() * ROTATE_Y_AXIS(1.0f));
		
		if (m_shipsVisualRotation.x < 15.0f)
		{
			m_shipsVisualRotation.x++;
		}
		showVisualShipRotation();
		m_lerp_X_Axis_Ship = false;
	}

	if (input.GetThumbRPosition().y < -0.3f)
	{
		returnToActualRotation();

		m_rigidBody->updateRotation(ROTATE_Y_AXIS(-1.0f));

		m_forwardDirection = Utility::getForward(m_rigidBody->getRotation());

		//Rotates the camera view
		m_camera->getTransform()->setRotation(*m_camera->getTransform()->getRotation() * ROTATE_Y_AXIS(-1.0f));

		if (m_shipsVisualRotation.x > -15.0f)
		{
			m_shipsVisualRotation.x--;
		}

		showVisualShipRotation();
		m_lerp_X_Axis_Ship = false;
	}
}

void PlayerShipMovementController::movement(const InputManager& input, float delta)
{
	m_rigidBody->updateVelocity(glm::vec3());

	//Controller inputs
	if (input.GetThumbLPosition().y > 0.3f)
	{
		m_rigidBody->updateVelocity(m_forwardDirection * m_velocityValue);
	}
	if (input.GetThumbLPosition().y < -0.3f)
	{
		m_rigidBody->updateVelocity(-m_forwardDirection * m_velocityValue);
	}
	if (input.PadButtonDown(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
	{
		returnToActualRotation();
		m_rigidBody->updateRotation(ROTATE_Z_AXIS(-1.0f));
		m_forwardDirection = Utility::getForward(m_rigidBody->getRotation());
		//Rotates the camera view
		m_camera->getTransform()->setRotation(*m_camera->getTransform()->getRotation() * ROTATE_Z_AXIS(-1.0f));
		showVisualShipRotation();
	}
	if (input.PadButtonDown(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
	{
		returnToActualRotation();
		m_rigidBody->updateRotation(ROTATE_Z_AXIS(1.0f));
		m_forwardDirection = Utility::getForward(m_rigidBody->getRotation());
		//Rotates the camera view
		m_camera->getTransform()->setRotation(*m_camera->getTransform()->getRotation() * ROTATE_Z_AXIS(1.0f));
		showVisualShipRotation();
	}
}

void PlayerShipMovementController::returnToActualRotation()
{
	if (glm::abs(m_shipsVisualRotation.x) != 0 || glm::abs(m_shipsVisualRotation.y) != 0)
	{
		m_rigidBody->updateRotation(ROTATE_Y_AXIS(-m_shipsVisualRotation.x));
		m_rigidBody->updateRotation(ROTATE_X_AXIS(-m_shipsVisualRotation.y));
	}
}

void PlayerShipMovementController::showVisualShipRotation()
{
	m_rigidBody->updateRotation(ROTATE_X_AXIS(m_shipsVisualRotation.y));
	m_rigidBody->updateRotation(ROTATE_Y_AXIS(m_shipsVisualRotation.x));
}

void PlayerShipMovementController::checkLerp(const InputManager& input)
{
	if ((input.GetThumbRPosition().y > 0.3f) || (input.GetThumbRPosition().y < -0.3f))
	{
		m_lerp_X_Axis_Ship = false;
	}
	else
	{
		m_lerp_X_Axis_Ship = true;
	}

	if ((input.GetThumbRPosition().x > 0.3f) || (input.GetThumbRPosition().x < -0.3f))
	{
		m_lerp_Y_Axis_Ship = false;
	}
	else
	{
		m_lerp_Y_Axis_Ship = true;
	}
}

void PlayerShipMovementController::lerp()
{
	returnToActualRotation();
	if (m_lerp_X_Axis_Ship)
	{
		if (m_shipsVisualRotation.x > 0)
		{
			m_shipsVisualRotation.x--;
		}
		else if (m_shipsVisualRotation.x < 0)
		{
			m_shipsVisualRotation.x++;
		}
	}
	if (m_lerp_Y_Axis_Ship)
	{
		if (m_shipsVisualRotation.y > 0)
		{
			m_shipsVisualRotation.y--;
		}
		else if (m_shipsVisualRotation.y < 0)
		{
			m_shipsVisualRotation.y++;
		}
	}
	showVisualShipRotation();

}