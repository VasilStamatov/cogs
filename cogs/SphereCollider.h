#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		namespace ecs
		{
				class SphereCollider : public Collider
				{
				public:
						SphereCollider(float _radius) 
						{
								m_collisionShape = std::make_shared<btSphereShape>(_radius);
						}
						virtual ~SphereCollider() {}

				private:

				};
		}
}

#endif // !SPHERE_COLLIDER_H