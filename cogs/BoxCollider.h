#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		namespace ecs
		{
				class BoxCollider : public Collider
				{
				public:
						BoxCollider(const glm::vec3& _halfExtents)
						{
								m_collisionShape = std::make_shared<btBoxShape>(btVector3(_halfExtents.x, _halfExtents.y, _halfExtents.z));
						}
						virtual ~BoxCollider() {}

				private:

				};
		}
}

#endif // !BOX_COLLIDER_H