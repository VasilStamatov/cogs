#include "PaddleController.h"

#include <cogs\Entity.h>
#include <cogs\Input.h>

namespace cu = cogs::utils;

PaddleController::PaddleController(float _moveSpeed) : m_moveSpeed(_moveSpeed)
{
}

PaddleController::~PaddleController()
{
}

void PaddleController::init()
{
		m_rb = m_entity.lock()->getComponent<ce::RigidBody>();
}

void PaddleController::update(float _deltaTime)
{
		//don't let the paddle accelerate over time from the force applied to it
		//as the force should apply for only 1 frame to keep it well-controlled
		m_rb.lock()->setLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

		if (cu::Input::isKeyDown(cu::KeyCode::LEFT))
		{
				m_rb.lock()->applyCentralForce(glm::vec3(-m_moveSpeed * _deltaTime, 0.0f, 0.0f));
		}
		if (cu::Input::isKeyDown(cu::KeyCode::RIGHT))
		{
				m_rb.lock()->applyCentralForce(glm::vec3(m_moveSpeed * _deltaTime, 0.0f, 0.0f));
		}
}

void PaddleController::onCollision(const glm::vec3 & _pointA, const glm::vec3 & _pointB, const glm::vec3 & _normalOnB, ce::Entity * _other)
{
}
