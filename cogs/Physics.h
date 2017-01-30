#ifndef PHYSICS_H
#define PHYSICS_H

#include <Bullet/btBulletDynamicsCommon.h>
#include <memory>

namespace cogs
{
		namespace physics
		{
				/**
				* A  class for the physics world, as only a single world is ever needed
				*/
				class Physics
				{
				public:
						Physics(float _xGravity, float _yGravity, float _zGravity);
						Physics() {}
						~Physics() {}

						/**
						* Initializes the physics world with the passed parameters being gravity on each axis
						*/
						void init(float _xGravity, float _yGravity, float _zGravity);

						/**
						* Sets the gravity of the physics world
						*/
						void setGravity(float _xGravity, float _yGravity, float _zGravity);

						/**
						* Adds a rigid body to the physics world
						*/
						void addRigidBody(btRigidBody* _rb);

						/**
						* Removes a rigid body from the physics world
						*/
						void removeRigidBody(btRigidBody* _rb);

						/**
						* Steps the simulation for the physics world (called every frame)
						*/
						void stepSimulation();

						/**
						* Sets the debug renderer
						*/
						void setDebugDrawer(btIDebugDraw* _debugDrawer);

						/**
						* Uses the debug renderer to draw the debug world
						*/
						void debugDrawWorld();

				private:
						std::unique_ptr<btBroadphaseInterface>															m_broadphase{ nullptr };
						std::unique_ptr<btDefaultCollisionConfiguration>					m_collisionConfiguration{ nullptr };
						std::unique_ptr<btCollisionDispatcher>															m_dispatcher{ nullptr };
						std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver{ nullptr };
						std::unique_ptr<btDiscreteDynamicsWorld>											  m_dynamicsWorld{ nullptr };
				};
		}
}

#endif // !PHYSICS_H