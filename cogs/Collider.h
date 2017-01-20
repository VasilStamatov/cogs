#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

#include <Bullet\btBulletDynamicsCommon.h>
#include <glm\vec3.hpp>
#include <memory>

namespace cogs
{
		namespace ecs
		{
				enum class ColliderShape
				{
						SPHERE,
						BOX,
						CYLINDER,
						CAPSULE,
						CONE,
						STATIC_PLANE
				};

				/**
				* Collider component which needs to be added before a rigidbody, as the rigidbody uses it for its contruction
				*/
				class Collider : public Component
				{
				public:
						/**
						* \brief Constructor, which creates the collider (btCollisionShape).
						Pass the shape that you desire and the parameters that are needed for its construction,
						the rest parameters can be left null
						* \param _shape - is used to know which shape to instantiate and which parameters to pass
						* \param _radius - used in shape instantiation for a sphere, capsule and cone
						* \param _halfExtents - used in shape instantiation for a box and cylinder
						* \param _height - used in shape instantiation for a capsule and cone
						*/
						Collider(const ColliderShape& _shape, float _radius = 0.0f, const glm::vec3& _halfExtents = glm::vec3(0.0f), float _height = 0.0f);
						~Collider();

						/**
						* The initialize component function
						*/
						void init() override;

						/**
						* The update component function
						*/
						void update(float _deltaTime) override;

						/**
						* The render component function
						*/
						void render() override;

						/**
						* Gets the btCollisionShape instance (usually for rigidbody construction)
						*/
						std::weak_ptr<btCollisionShape> getShape() { return m_collisionShape; }

				protected:
						ColliderShape m_shape; ///< the shape this collider is
						std::shared_ptr<btCollisionShape> m_collisionShape{ nullptr }; ///< bullet shape instance
				};
		}
}

#endif // !COLLIDER_H
