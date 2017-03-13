#include "CMotionState.h"
#include "Transform.h"

#include <glm\gtc\type_ptr.hpp>

namespace cogs
{
		CMotionState::CMotionState(std::weak_ptr<Transform> _transform)
		{
				m_transform = _transform;
		}

		CMotionState::~CMotionState()
		{
		}

		void CMotionState::getWorldTransform(btTransform & _worldTrans) const
		{
				if (m_transform.expired())
				{
						return;
				}

				btTransform transform;
				transform.setFromOpenGLMatrix(glm::value_ptr(m_transform.lock()->worldTransform()));

				_worldTrans = transform;
		}

		void CMotionState::setWorldTransform(const btTransform & _worldTrans)
		{
				if (m_transform.expired())
				{
						return;
				}

				btQuaternion rot = _worldTrans.getRotation();
				const btVector3& pos = _worldTrans.getOrigin();

				m_transform.lock()->setWorldOrientation(glm::quat(rot.w(), rot.x(), rot.y(), rot.z()));
				m_transform.lock()->setWorldPosition(glm::vec3(pos.x(), pos.y(), pos.z()));
		}
}