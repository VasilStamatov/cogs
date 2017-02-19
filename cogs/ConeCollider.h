#ifndef CONE_COLLIDER_H
#define CONE_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		namespace ecs
		{
				/**
				* \brief Instantiates a btConeShape using the passed radius and height
				*/
				class ConeCollider : public Collider
				{
				public:
						/**
						* \brief create the btConeShape
						* \param _radius - the radius of the capsule
						* \param _height - the height of the capsule
						*/
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