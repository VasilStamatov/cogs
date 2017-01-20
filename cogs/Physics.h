#ifndef PHYSICS_H
#define PHYSICS_H

#include <Bullet/btBulletDynamicsCommon.h>
#include <memory>

namespace cogs
{
		namespace physics
		{
				/**
				* A static class for the physics world, as only a single world is ever needed
				*/
				class Physics
				{
				public:
						/**
						* Initializes the physics world with the passed parameters being gravity on each axis
						*/
						static void init(float _xGravity, float _yGravity, float _zGravity);

						/**
						* Sets the gravity of the physics world
						*/
						static void setGravity(float _xGravity, float _yGravity, float _zGravity);

						/**
						* Adds a rigid body to the physics world
						*/
						static void addRigidBody(btRigidBody* _rb);

						/**
						* Removes a rigid body from the physics world
						*/
						static void removeRigidBody(btRigidBody* _rb);

						/**
						* Steps the simulation for the physics world (called every frame)
						*/
						static void stepSimulation();

						/**
						* Sets the debug renderer
						*/
						static void setDebugDrawer(btIDebugDraw* _debugDrawer);

						/**
						* Uses the debug renderer to draw the debug world
						*/
						static void debugDrawWorld();

				private:
						Physics() {}
						~Physics() {}

				private:
						static std::unique_ptr<btBroadphaseInterface> m_broadphase;
						static std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
						static std::unique_ptr<btCollisionDispatcher> m_dispatcher;
						static std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
						static std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
				};
		}
}

#endif // !PHYSICS_H