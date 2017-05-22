#include "Physics.h"

#include "Entity.h"

namespace cogs
{
		Physics::Physics(float _xGravity, float _yGravity, float _zGravity)
		{
				init(_xGravity, _yGravity, _zGravity);
		}
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
				// must follow this equation:
				// timeStep < maxSubSteps * fixedTimeStep
				// as deltaTime is == fixedTimeStep it would work without the 5
				// but it's helpful just in case the deltaTime gets higher (game lags) by 5 times more
				m_dynamicsWorld->stepSimulation(m_timer.seconds());
				m_timer.start();
				int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
				for (int i = 0; i < numManifolds; i++)
				{
						btPersistentManifold* contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
						const btCollisionObject* obA = contactManifold->getBody0();
						const btCollisionObject* obB = contactManifold->getBody1();

						int numContacts = contactManifold->getNumContacts();
						for (int j = 0; j < numContacts; j++)
						{
								btManifoldPoint& pt = contactManifold->getContactPoint(j);
								if (pt.getDistance() < 0.f)
								{
										const btVector3& ptA = pt.getPositionWorldOnA();
										const btVector3& ptB = pt.getPositionWorldOnB();
										const btVector3& normalOnB = pt.m_normalWorldOnB;

										Entity* objA = static_cast<Entity*>(obA->getUserPointer());
										Entity* objB = static_cast<Entity*>(obB->getUserPointer());

										if (objA == nullptr || objB == nullptr)
										{
												continue;
										}

										objA->collide(glm::vec3(ptA.x(), ptA.y(), ptA.z()),
												glm::vec3(ptB.x(), ptB.y(), ptB.z()),
												glm::vec3(normalOnB.x(), normalOnB.y(), normalOnB.z()),
												objB);

										objB->collide(glm::vec3(ptA.x(), ptA.y(), ptA.z()),
												glm::vec3(ptB.x(), ptB.y(), ptB.z()),
												glm::vec3(normalOnB.x(), normalOnB.y(), normalOnB.z()),
												objA);
								}
						}
				}
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