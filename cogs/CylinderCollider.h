#ifndef CYLINDER_COLLIDER_H
#define CYLINDER_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		namespace ecs
		{
				/**
				* \brief Constructor to create the btCylinderShape
				*/
				class CylinderCollider : public Collider
				{
				public:
						/**
						* \brief Constructor to create the btCylinderShape
						* \param _halfExtents - the half extent on every axis in meters
						*/
						CylinderCollider(const glm::vec3& _halfExtents)
						{
								m_collisionShape = std::make_shared<btCylinderShape>(btVector3(_halfExtents.x, _halfExtents.y, _halfExtents.z));
						}
						virtual ~CylinderCollider() {}

				private:

				};
		}
}

#endif // !CYLINDER_COLLIDER_H