#include "Collider.h"

namespace cogs
{
		namespace ecs
		{
				Collider::Collider(const ColliderShape& _shape, float _radius, const glm::vec3& _halfExtents, float _height) : m_shape(_shape)
				{
						switch (m_shape)
						{
								case ColliderShape::SPHERE:
								{
										m_collisionShape = std::make_shared<btSphereShape>(_radius);
										break;
								}
								case ColliderShape::BOX:
								{
										m_collisionShape = std::make_shared<btBoxShape>(btVector3(_halfExtents.x, _halfExtents.y, _halfExtents.z));
										break;
								}
								case ColliderShape::CYLINDER:
								{
										m_collisionShape = std::make_shared<btCylinderShape>(btVector3(_halfExtents.x, _halfExtents.y, _halfExtents.z));
										break;
								}
								case ColliderShape::CAPSULE:
								{
										m_collisionShape = std::make_shared<btCapsuleShape>(_radius, _height);
										break;
								}
								case ColliderShape::CONE:
								{
										m_collisionShape = std::make_shared<btConeShape>(_radius, _height);
										break;
								}
								case ColliderShape::STATIC_PLANE:
								{
										m_collisionShape = std::make_shared<btStaticPlaneShape>(btVector3(0, 1, 0), 0);
										break;
								}
								default: break;
						}
				}
				Collider::~Collider()
				{
				}
				void Collider::init()
				{
				}
				void Collider::update(float _deltaTime)
				{
				}
				void Collider::setLocalScaling(const glm::vec3 & _scale)
				{
						m_collisionShape->setLocalScaling(btVector3(_scale.x, _scale.y, _scale.z));
				}
		}
}