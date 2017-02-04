#include "PaddleController.h"

#include <cogs\Entity.h>
#include <cogs\Input.h>

namespace cu = cogs::utils;

PaddleController::PaddleController(float _moveSpeed) : m_moveSpeed(_moveSpeed)
{
}

PaddleController::~PaddleController()
{
		m_rb = nullptr;
}

void PaddleController::init()
{
		m_rb = m_entity->getComponent<ce::RigidBody>();
}

void PaddleController::update(float _deltaTime)
{
		m_rb->setLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

		if (cu::Input::isKeyDown(cu::KeyCode::LEFT))
		{
				m_rb->translate(glm::vec3(-m_moveSpeed * _deltaTime, 0.0f, 0.0f));
		}
		if (cu::Input::isKeyDown(cu::KeyCode::RIGHT))
		{
				m_rb->translate(glm::vec3(m_moveSpeed * _deltaTime, 0.0f, 0.0f));
		}
}

void PaddleController::onCollision(const glm::vec3 & _pointA, const glm::vec3 & _pointB, const glm::vec3 & _normalOnB, ce::Entity * _other)
{
}
