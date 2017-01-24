#include "CMotionState.h"

#include <glm\gtc\type_ptr.hpp>

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

						btTransform transform;
						transform.setFromOpenGLMatrix(glm::value_ptr(m_transform->worldTransform()));

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