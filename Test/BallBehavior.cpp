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
}

void BallBehavior::update(float _deltaTime)
{
		if (m_rb.lock()->getActivationState() == 5)
		{
				if (cu::Input::isKeyPressed(cu::KeyCode::SPACE))
				{
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
		const std::string& otherTag = _other->getTag();

		if (otherTag == "brick")
		{
				_other->destroy();
		}
		else if (otherTag == "ground")
		{
				m_entity.lock()->destroy();
		}
}
