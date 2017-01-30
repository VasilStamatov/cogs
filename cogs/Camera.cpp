#include "Camera.h"
#include "Entity.h"

#include <glm\gtc\matrix_transform.hpp>

namespace cogs
{
		namespace ecs
		{
				Camera::Camera(const ProjectionType& _projType /* = ORTHO */,
						int _screenWidth /* = 1024 */, int _screenHeight /* = 576 */)
				{
						m_projType = _projType;
						m_screenWidth = _screenWidth;
						m_screenHeight = _screenHeight;
						//projection matrix
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth) * m_size, 0.0f, static_cast<float>(m_screenHeight) * m_size, m_nearPlane, m_farPlane);
				}

				Camera::~Camera()
				{
						m_transform = nullptr;
				}

				void Camera::init()
				{
						m_transform = m_entity->getComponent<Transform>();
						m_oldTransform = *m_transform;
				}

				void Camera::update(float _deltaTime)
				{
						if (!(*m_transform == m_oldTransform))
						{
								m_viewMatrix = glm::lookAt(m_transform->worldPosition(),
										m_transform->worldPosition() + m_transform->worldForwardAxis(),
										m_transform->worldUpAxis());

								m_oldTransform = *m_transform;
						}
				}

				void Camera::setFoV(int _value)
				{
						m_fov = _value;
						if (m_fov < 1)
						{
								m_fov = 1;
						}
						else if (m_fov > 179)
						{
								m_fov = 179;
						}
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
				}

				void Camera::offsetFoV(int _value)
				{
						m_fov += _value;
						if (m_fov < 1)
						{
								m_fov = 1;
						}
						else if (m_fov > 179)
						{
								m_fov = 179;
						}
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
				}

				void Camera::setSize(float _value)
				{
						m_size = _value;
						if (m_size < 1.0f)
						{
								m_size = 1.0f;
						}
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth) * m_size, 0.0f, static_cast<float>(m_screenHeight) * m_size, m_nearPlane, m_farPlane);
				}

				void Camera::offsetSize(float _value)
				{
						m_size += _value;
						if (m_size < 1.0f)
						{
								m_size = 1.0f;
						}
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth) * m_size, 0.0f, static_cast<float>(m_screenHeight) * m_size, m_nearPlane, m_farPlane);
				}

				void Camera::setNearPlane(float _value)
				{
						m_nearPlane = _value;
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth) * m_size, 0.0f, static_cast<float>(m_screenHeight) * m_size, m_nearPlane, m_farPlane);
				}

				void Camera::setFarPlane(float _value)
				{
						m_farPlane = _value;
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth) * m_size, 0.0f, static_cast<float>(m_screenHeight) * m_size, m_nearPlane, m_farPlane);
				}

				void Camera::resize(int _screenWidth, int _screenHeight)
				{
						m_screenWidth = _screenWidth;
						m_screenHeight = _screenHeight;
						//projection matrix
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth) * m_size, 0.0f, static_cast<float>(m_screenHeight) * m_size, m_nearPlane, m_farPlane);
				}

				const glm::mat4 & Camera::getProjectionMatrix() const noexcept
				{
						if (m_projType == ProjectionType::ORTHOGRAPHIC) return m_orthoMatrix;
						else
								return m_perspMatrix;
				}
		}
}