#include "Transform.h"
#include "Entity.h"

#include <glm\gtc\matrix_transform.hpp>

namespace cogs
{
		namespace ecs
		{
				Transform::Transform(const glm::vec3 & _pos /* = glm::vec3(0.0f)  */,
						const glm::vec3 & _eulerAngles 											/* = identity quat				*/,
						const glm::vec3 & _scale 																	/* = glm::vec3 (1.0f) */,
						Transform * _parent																					 	/* = nullptr										*/)
				{
						m_parent = _parent;
						internal_setWorldPosition(_pos);
						internal_setWorldOrientation(_eulerAngles);
						internal_setWorldScale(_scale);
				}

				Transform::~Transform()
				{
				}
				void Transform::init()
				{
				}
				void Transform::update(float _deltaTime)
				{
						//internal_updateTransform();
				}
				void Transform::render()
				{
				}

				void Transform::rotate(const glm::vec3 & _eulerAngles)
				{
						glm::quat toRotate(glm::vec3(
								glm::radians(_eulerAngles.x),
								glm::radians(_eulerAngles.y),
								glm::radians(_eulerAngles.z)));

						internal_setLocalOrientation(toRotate * m_localOrientationRaw);
				}

				void Transform::rotate(const glm::quat & _rotationQuat)
				{
						internal_setLocalOrientation(_rotationQuat * m_localOrientationRaw);
				}

				void Transform::lookAt(const glm::vec3 & _target)
				{
						glm::vec3 direction = glm::normalize(m_worldPosition - _target);
						glm::mat4 lookAtMat = glm::lookAt(m_worldPosition, m_worldPosition + direction, worldUpAxis());
						internal_setLocalOrientation(glm::quat_cast(lookAtMat));
				}

				void Transform::translate(const glm::vec3 & _offset)
				{
						internal_setLocalPosition(m_localPosition + _offset);
				}

				void Transform::scale(const glm::vec3 & _offset)
				{
						internal_setLocalScale(m_localScale + _offset);
				}

				glm::mat4 Transform::localTransform() const
				{
						glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_localPosition);
						glm::mat4 scale							= glm::scale(glm::mat4(1.0f), m_localScale);
						glm::mat4 rotation				= glm::mat4_cast(m_localOrientationRaw);

						glm::mat4 localTransform = translation * rotation * scale;

						return localTransform;
				}

				glm::mat4 Transform::worldTransform() const
				{
						glm::mat4 worldTrans = localTransform();

						if (m_parent != nullptr)
						{
								worldTrans = m_parent->worldTransform() * worldTrans;
						}

						return worldTrans;
				}

				bool Transform::operator==(const Transform & _rhs) const
				{
						auto boolVec = glm::equal(m_worldPosition, _rhs.m_worldPosition);

						if (boolVec.x == boolVec.y == boolVec.z == true)
						{
								boolVec = glm::equal(m_worldScale, _rhs.m_worldScale);

								if (boolVec.x == boolVec.y == boolVec.z == true)
								{
										boolVec = glm::equal(m_worldOrientation, _rhs.m_worldOrientation);

										if (boolVec.x == boolVec.y == boolVec.z == true)
										{
												return true;
										}
								}
						}
						return false;
				}

				void Transform::internal_setLocalOrientation(const glm::quat & _value)
				{
						//set the raw orientation value
						m_localOrientationRaw = glm::normalize(_value);

						//set the orientation's euler angles representation
						m_localOrientation = glm::eulerAngles(m_localOrientationRaw);

						//update the world orientation to be correct
						if (m_parent != nullptr)
						{
								//to get the correct world orientation, multiply parent's world orientation by the childs local orientation
								m_worldOrientationRaw = glm::normalize(m_parent->m_worldOrientationRaw * m_localOrientationRaw);
						}
						else
						{
								//if there is no parent, world == local
								m_worldOrientationRaw = m_localOrientationRaw;
						}

						//set the world orientation's euler angle representation
						m_worldOrientation = glm::eulerAngles(m_worldOrientationRaw);
				}

				void Transform::internal_setLocalOrientation(const glm::vec3 & _value)
				{
						//convert the euler angle value to a quat and pass it to the overloaded function
						internal_setLocalOrientation(glm::quat(glm::vec3(glm::radians(_value.x), glm::radians(_value.y), glm::radians(_value.z))));
				}

				void Transform::internal_setLocalPosition(const glm::vec3 & _value)
				{
						//set the new world position
						m_localPosition = _value;

						//update the world position to be correct
						if (m_parent != nullptr)
						{
								//to get the correct world position, simply add the parent's world translation from the child's local translation
								m_worldPosition = m_parent->m_worldPosition + m_localPosition;
						}
						else
						{
								//if there is no parent, world == local
								m_worldPosition = m_localPosition;
						}
				}

				void Transform::internal_setLocalScale(const glm::vec3 & _value)
				{
						//set the new local scale
						m_localScale = _value;

						//update the world scale to be correct
						if (m_parent != nullptr)
						{
								//to get the correct world scale, multiply the components of the parent against the child's
								m_worldScale = m_parent->m_worldScale * m_localScale;
						}
						else
						{
								//if there is no parent, world == local
								m_worldScale = m_localScale;
						}
				}

				void Transform::internal_setWorldOrientation(const glm::quat & _value)
				{
						// set the raw orientation value
						m_worldOrientationRaw = glm::normalize(_value);

						//set the orientation's euler angles representation
						m_worldOrientation = glm::eulerAngles(m_worldOrientationRaw);

						//update the local orientation to be correct
						if (m_parent != nullptr)
						{
								//to get the correct local orientation, multiply the child's world rotation by the parent's conjugate
								m_localOrientationRaw = m_worldOrientationRaw * glm::normalize(glm::conjugate(m_parent->m_worldOrientationRaw));
						}
						else
						{
								//if there is no parent, world == local
								m_localOrientationRaw = m_worldOrientationRaw;
						}

						//set the local orientation's euler angle representation
						m_localOrientation = glm::eulerAngles(m_localOrientationRaw);
				}

				void Transform::internal_setWorldOrientation(const glm::vec3 & _value)
				{
						//convert the euler angle value to a quat and pass it to the overloaded function
						internal_setWorldOrientation(glm::quat(glm::vec3(glm::radians(_value.x), glm::radians(_value.y), glm::radians(_value.z))));
				}

				void Transform::internal_setWorldPosition(const glm::vec3 & _value)
				{
						//set the new world position
						m_worldPosition = _value;

						//update the local position to be correct
						if (m_parent != nullptr)
						{
								//to get the correct local position, subtract the parent's world translation from the child's world translation
								m_localPosition = m_worldPosition - m_parent->m_worldPosition;
						}
						else
						{
								//if there is no parent, world == local
								m_localPosition = m_worldPosition;
						}
				}
				void Transform::internal_setWorldScale(const glm::vec3 & _value)
				{
						//set the new local scale
						m_worldScale = _value;

						//update the local scale to be correct
						if (m_parent != nullptr)
						{
								//to get the correct local scale, multiply the child's world scale with the reciprocal of the parent's world scale
								m_localScale = m_worldScale * (1.0f / m_parent->m_worldScale);
						}
						else
						{
								//if there is no parent, world == local
								m_localScale = m_worldScale;
						}
				}
		}
}