#include "Physics.h"

namespace cogs
{
		namespace physics
		{
				std::unique_ptr<btBroadphaseInterface> Physics::m_broadphase = nullptr;
				std::unique_ptr<btDefaultCollisionConfiguration> Physics::m_collisionConfiguration = nullptr;
				std::unique_ptr<btCollisionDispatcher> Physics::m_dispatcher = nullptr;
				std::unique_ptr<btSequentialImpulseConstraintSolver> Physics::m_solver = nullptr;
				std::unique_ptr<btDiscreteDynamicsWorld> Physics::m_dynamicsWorld = nullptr;

				void Physics::init(float _xGravity, float _yGravity, float _zGravity)
				{
						m_broadphase = std::make_unique<btDbvtBroadphase>();
						m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
						m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());
						m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
						m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get());

						setGravity(_xGravity, _yGravity, _zGravity);
				}

				void Physics::setGravity(float _xGravity, float _yGravity, float _zGravity)
				{
						m_dynamicsWorld->setGravity(btVector3(_xGravity, _yGravity, _zGravity));
				}
				void Physics::addRigidBody(btRigidBody* _rb)
				{
						m_dynamicsWorld->addRigidBody(_rb);
				}
				void Physics::removeRigidBody(btRigidBody* _rb)
				{
						m_dynamicsWorld->removeRigidBody(_rb);
				}
				void Physics::stepSimulation()
				{
						m_dynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
				}
				void Physics::setDebugDrawer(btIDebugDraw * _debugDrawer)
				{
						m_dynamicsWorld->setDebugDrawer(_debugDrawer);
				}
				void Physics::debugDrawWorld()
				{
						m_dynamicsWorld->debugDrawWorld();
				}
		}
}