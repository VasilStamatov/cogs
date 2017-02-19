#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

#include <Bullet\btBulletDynamicsCommon.h>
#include <glm\vec3.hpp>

namespace cogs
{
		namespace ecs
		{
				/**
				* \brief Collider component which needs to be added before a rigidbody, as the rigidbody uses it for its contruction
				*/
				class Collider : public Component
				{
				public:
						Collider() {}
						virtual ~Collider() {}

						/**
						* The initialize component function
						*/
						void init() override {}

						/**
						* The update component function
						*/
						void update(float _deltaTime) override {}

						/**
						* \brief scaled the collider on each axis
						*/
						void setLocalScaling(const glm::vec3& _scale) 
						{
								m_collisionShape->setLocalScaling(btVector3(_scale.x, _scale.y, _scale.z));
						}

						/**
						* \brief Gets the btCollisionShape instance (usually for rigidbody construction)
						*/
						std::weak_ptr<btCollisionShape> getShape() { return m_collisionShape; }

				protected:
						std::shared_ptr<btCollisionShape> m_collisionShape{ nullptr }; ///< bullet shape instance
				};
		}
}

#endif // !COLLIDER_H
