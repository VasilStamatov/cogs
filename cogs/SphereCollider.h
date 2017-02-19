#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		namespace ecs
		{
				/**
				* \brief Constructor to create the btSphereShape
				*/
				class SphereCollider : public Collider
				{
				public:
						/**
						* \brief Constructor to create the btSphereShape
						* \param _radius - the radius of the sphere
						*/
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