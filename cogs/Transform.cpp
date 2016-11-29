#include "Transform.h"
#include <glm\gtc\matrix_transform.hpp>

namespace cogs
{
		namespace ecs
		{
				Transform::Transform(const glm::vec3 & _pos /* = glm::vec3(0.0f) */,
						const glm::quat & _rot /* = identity quat */,
						const glm::vec3 _scale /* = glm::vec3 (1.0f) */,
						Transform * _parent /* = nullptr */)
				{
						m_position = _pos;
						m_rotation = _rot;
						m_scale = _scale;
						m_parent = _parent;
				}

				Transform::~Transform()
				{
				}
				void Transform::init()
				{
				}
				void Transform::update(float _deltaTime)
				{
				}
				void Transform::render()
				{
				}

				void Transform::rotate(const glm::vec3 & _eulerAngles)
				{
						glm::quat toRotate(glm::vec3(glm::radians(_eulerAngles.x), glm::radians(_eulerAngles.y), glm::radians(_eulerAngles.z)));

						m_rotation = glm::normalize(toRotate * m_rotation);
				}

				void Transform::lookAt(const glm::vec3 & _target, const glm::vec3& _up)
				{
						glm::vec3 direction = glm::normalize(m_position - _target);
						glm::mat4 lookAtMat = glm::lookAt(m_position, m_position + direction, _up);
						m_rotation = glm::quat_cast(lookAtMat);
				}

				void Transform::translate(const glm::vec3 & _offset)
				{
						m_position += _offset;
				}
				
				glm::vec3 Transform::getTransformedPos() const
				{
						/*glm::mat4 transform = getTransformation();
						return (transform[3]);*/
						glm::mat4 positionMat = glm::translate(glm::mat4(1.0f), m_position);
						glm::mat4 parentPos(1.0f);
						if (m_parent != nullptr)
						{
								parentPos = glm::translate(parentPos, m_parent->getTransformedPos());
						}
						glm::mat4 result = parentPos * positionMat;
						return result[3];
				}

				glm::quat Transform::getTransformedRot() const
				{
						glm::quat parentRot(0.0f, 0.0f, 0.0f, 1.0f);
						if (m_parent != nullptr)
						{
								parentRot = m_parent->getTransformedRot();
						}
						return parentRot * m_rotation;
				}

				glm::mat4 Transform::getTransformation() const
				{
						glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), m_position);
						glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), m_scale);
						glm::mat4 rotationMat = glm::mat4_cast(m_rotation);

						glm::mat4 transformMat = translationMat * rotationMat * scaleMat;

						glm::mat4 parentMat(1.0f);

						if (m_parent != nullptr)
						{
								parentMat = m_parent->getTransformation();
						}

						return parentMat * transformMat;
				}
		}
}