#ifndef CAPSULE_COLLIDER_H
#define CAPSULE_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		/**
		* \brief Instantiates a btCapsuleShape using the passed radius and height
		*/
		class CapsuleCollider : public Collider
		{
		public:
				/**
				* \brief create the btCapsuleShape
				* \param _radius - the radius of the capsule
				* \param _height - the height of the capsule
				*/
				CapsuleCollider(float _radius, float _height)
				{
						m_collisionShape = std::make_shared<btCapsuleShape>(_radius, _height);
				}
				virtual ~CapsuleCollider() {}

		private:

		};
}

#endif // !CAPSULE_COLLIDER_H