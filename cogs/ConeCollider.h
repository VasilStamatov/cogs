#ifndef CONE_COLLIDER_H
#define CONE_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		namespace ecs
		{
				class ConeCollider : public Collider
				{
				public:
						ConeCollider(float _radius, float _height)
						{
								m_collisionShape = std::make_shared<btConeShape>(_radius, _height);
						}
						virtual ~ConeCollider() {}

				private:

				};
		}
}

#endif // !CONE_COLLIDER_H