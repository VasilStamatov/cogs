#ifndef PHYSICS_H
#define PHYSICS_H

#include <Bullet/btBulletDynamicsCommon.h>
#include <memory>

#include "Timing.h"

namespace cogs
{
		/**
		* The physics world class
		*/
		class Physics
		{
		public:
				/**
				* \brief Construct the physics world with gravity
				* \param[in] _xGravity - gravity to the x axis
				* \param[in] _yGravity - gravity to the y axis
				* \param[in] _zGravity - gravity to the z axis
				*/
				Physics(float _xGravity, float _yGravity, float _zGravity);
				Physics() {}
				~Physics() {}

				/**
				* \brief Init the physics world with gravity
				* \param[in] _xGravity - gravity to the x axis
				* \param[in] _yGravity - gravity to the y axis
				* \param[in] _zGravity - gravity to the z axis
				*/
				void init(float _xGravity, float _yGravity, float _zGravity);

				/**
				* \brief Sets the gravity of the physics world
				*/
				void setGravity(float _xGravity, float _yGravity, float _zGravity);

				/**
				* \brief Adds a rigid body to the physics world
				*/
				void addRigidBody(btRigidBody* _rb);

				/**
				* \brief Removes a rigid body from the physics world
				*/
				void removeRigidBody(btRigidBody* _rb);

				/**
				* \brief Steps the simulation for the physics world (called every frame)
				*/
				void stepSimulation();

				/**
				* \brief Sets the debug renderer
				*/
				void setDebugDrawer(btIDebugDraw* _debugDrawer);

				/**
				* \brief Uses the debug renderer to draw the debug world
				*/
				void debugDrawWorld();

		private:
				std::unique_ptr<btBroadphaseInterface>															m_broadphase{ nullptr };
				std::unique_ptr<btDefaultCollisionConfiguration>					m_collisionConfiguration{ nullptr };
				std::unique_ptr<btCollisionDispatcher>															m_dispatcher{ nullptr };
				std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver{ nullptr };
				std::unique_ptr<btDiscreteDynamicsWorld>											  m_dynamicsWorld{ nullptr };
				HRTimer m_timer;
		};
}

#endif // !PHYSICS_H