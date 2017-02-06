#include "BallBehavior.h"

#include <cogs\Entity.h>
#include <cogs\Input.h>

namespace cu = cogs::utils;

BallBehavior::BallBehavior()
{
}

BallBehavior::~BallBehavior()
{
}

void BallBehavior::init()
{
		m_rb = m_entity.lock()->getComponent<ce::RigidBody>();
		m_transform = m_entity.lock()->getComponent<ce::Transform>();
}

void BallBehavior::update(float _deltaTime)
{
		if (m_rb.lock()->getActivationState() == 5)
		{
				if (cu::Input::isKeyPressed(cu::KeyCode::SPACE))
				{
						m_transform.lock()->setParent(m_transform.lock()->getParent().lock()->getParent());
						m_rb.lock()->setActivationState(1);
						m_rb.lock()->setLinearVelocity(glm::vec3(20.0f, 20.0f, 0.0f));
						const btVector3& desiredVelocity = m_rb.lock()->getRigidBody().lock()->getLinearVelocity();
						m_desiredVelocity = desiredVelocity.length();
				}
		}
		else
		{
			 btVector3 currentVelocityDirection = m_rb.lock()->getRigidBody().lock()->getLinearVelocity();
				float currentVelocty = currentVelocityDirection.length();
				if (currentVelocty < m_desiredVelocity)
				{
						currentVelocityDirection *= (m_desiredVelocity / currentVelocty);
						m_rb.lock()->setLinearVelocity(glm::vec3(currentVelocityDirection.x(), currentVelocityDirection.y(), currentVelocityDirection.z()));
				}
		}
}

void BallBehavior::onCollision(const glm::vec3 & _pointA, const glm::vec3 & _pointB, const glm::vec3 & _normalOnB, ce::Entity * _other)
{
		const std::string& otherEntityName = _other->getName();

		if (otherEntityName == "Brick")
		{
				_other->destroy();
		}
		else if (otherEntityName == "GroundBoundary")
		{
				m_entity.lock()->destroy();
		}
		else if (otherEntityName == "PlayerPaddle")
		{
		}
}
