#ifndef CYLINDER_COLLIDER_H
#define CYLINDER_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		namespace ecs
		{
				class CylinderCollider : public Collider
				{
				public:
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