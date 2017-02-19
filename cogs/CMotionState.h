#ifndef CMOTION_STATE_H
#define CMOTION_STATE_H

#include <Bullet/btBulletDynamicsCommon.h>
#include "Transform.h"

namespace cogs
{
		namespace physics
		{
				/**
				* Implementation of bullet's motion state for this engine
				*/
				class CMotionState : public btMotionState
				{
				public:
						/**
						* Pass the address of the entity's transform,
						so the motion state can use it to set the rigidbody's position, and apply physics to it
						*/
						CMotionState(std::weak_ptr<ecs::Transform> _transform);
						virtual ~CMotionState();

						/**
						* Overriden bullet functions to tie the bullet engine with this one
						*/
						virtual void getWorldTransform(btTransform & _worldTrans) const override;
						virtual void setWorldTransform(const btTransform & _worldTrans) override;

				private:
						std::weak_ptr<ecs::Transform> m_transform; ///< reference to the transform of the entity
				};
		}
}

#endif // !CMOTION_STATE_H