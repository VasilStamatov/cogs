#ifndef CAPSULE_COLLIDER_H
#define CAPSULE_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		namespace ecs
		{
				class CapsuleCollider : public Collider
				{
				public:
						CapsuleCollider(float _radius, float _height)
						{
								m_collisionShape = std::make_shared<btCapsuleShape>(_radius, _height);
						}
						virtual ~CapsuleCollider() {}

				private:

				};
		}
}

#endif // !CAPSULE_COLLIDER_H