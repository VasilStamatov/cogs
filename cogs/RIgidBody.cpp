#include "RigidBody.h"

#include "Entity.h"
#include "Collider.h"
#include "Physics.h"

namespace cogs
{
		namespace ecs
		{
				RigidBody::RigidBody(float _mass) : m_mass(_mass)
				{
						btClamp(m_mass, 0.0f, 1.0f);
				}

				RigidBody::~RigidBody()
				{
						physics::Physics::removeRigidBody(m_rigidBody.get());
				}

				void RigidBody::init()
				{
						m_motionState = std::make_shared<physics::CMotionState>(m_entity->getComponent<Transform>());

						btVector3 intertia(0.0f, 0.0f, 0.0f);

						Collider* colliderShape = m_entity->getComponent<Collider>();
						colliderShape->getShape().lock()->calculateLocalInertia(m_mass, intertia);

						btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass, m_motionState.get(), colliderShape->getShape().lock().get(), intertia);

						m_rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);

						physics::Physics::addRigidBody(m_rigidBody.get());
				}

				void RigidBody::update(float _deltaTime)
				{
				}
				void RigidBody::render()
				{
				}
				void RigidBody::translate(const glm::vec3 & _offset)
				{
						m_rigidBody->translate(btVector3(_offset.x, _offset.y, _offset.z));
				}
				void RigidBody::applyCentralForce(const glm::vec3 & _force)
				{
						m_rigidBody->applyCentralForce(btVector3(_force.x, _force.y, _force.z));
				}
				void RigidBody::setLinearVelocity(const glm::vec3 & _offset)
				{
						m_rigidBody->setLinearVelocity(btVector3(_offset.x, _offset.y, _offset.z));
				}
				void RigidBody::setAngularVelocity(const glm::vec3 & _offset)
				{
						m_rigidBody->setAngularVelocity(btVector3(_offset.x, _offset.y, _offset.z));
				}
				void RigidBody::setRestitution(float _restitution)
				{
						m_rigidBody->setRestitution(_restitution);
				}
				void RigidBody::setLinearFactor(const glm::vec3 & _offset)
				{
						m_rigidBody->setLinearFactor(btVector3(_offset.x, _offset.y, _offset.z));
				}
				void RigidBody::setAngularFactor(const glm::vec3 & _offset)
				{
						m_rigidBody->setAngularFactor(btVector3(_offset.x, _offset.y, _offset.z));
				}
				void RigidBody::setDamping(float _x, float _y)
				{
						m_rigidBody->setDamping(_x, _y);
				}
		}
}