#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

namespace cogs
{
		/**
		* \brief Instantiates a btBoxShape using the passed half extents
		*/
		class BoxCollider : public Collider
		{
		public:
				/**
				* \brief Constructor to create the btBoxShape
				* \param _halfExtents - the half extent on every axis in meters
				*/
				BoxCollider(const glm::vec3& _halfExtents)
				{
						m_collisionShape = std::make_shared<btBoxShape>(btVector3(_halfExtents.x, _halfExtents.y, _halfExtents.z));
				}
				virtual ~BoxCollider() {}

		private:

		};
}

#endif // !BOX_COLLIDER_H