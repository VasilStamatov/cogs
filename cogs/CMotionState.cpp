#include "CMotionState.h"

namespace cogs
{
		namespace physics
		{
				CMotionState::CMotionState(ecs::Transform * _transform)
				{
						m_transform = _transform;
				}

				CMotionState::~CMotionState()
				{
						m_transform = nullptr;
				}

				void CMotionState::getWorldTransform(btTransform & _worldTrans) const
				{
						if (m_transform == nullptr)
						{
								return;
						}

						const glm::vec3& orientation = m_transform->worldOrientation();
						const glm::vec3& pos = m_transform->worldPosition();

						btTransform transform(btQuaternion(orientation.y, orientation.x, orientation.z),
								btVector3(pos.x, pos.y, pos.z));

						_worldTrans = transform;
				}

				void CMotionState::setWorldTransform(const btTransform & _worldTrans)
				{
						if (m_transform == nullptr)
						{
								return;
						}

						btQuaternion rot = _worldTrans.getRotation();
						const btVector3& pos = _worldTrans.getOrigin();

						m_transform->setWorldOrientation(glm::quat(rot.x(), rot.y(), rot.z(), rot.w()));
						m_transform->setWorldPosition(glm::vec3(pos.x(), pos.y(), pos.z()));
				}
		}
}