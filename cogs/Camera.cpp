#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace cogs
{
		namespace ecs
		{
				CameraComponent::CameraComponent(const ProjectionType& _projType /* = ORTHO */,
						int _screenWidth /* = 1024 */, int _screenHeight /* = 576 */)
				{
						m_projType = _projType;
						m_screenWidth = _screenWidth;
						m_screenHeight = _screenHeight;
						//projection matrix
						m_perspMatrix = glm::perspective(glm::radians((float)m_fov), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight, m_nearPlane, m_farPlane);
				}

				CameraComponent::~CameraComponent()
				{
						m_transform = nullptr;
				}

				void CameraComponent::init()
				{
						m_transform = this->m_entity->getComponent<Transform>();
				}

				void CameraComponent::update(float _deltaTime)
				{
						if (m_needsMatrixUpdate)
						{
								m_viewMatrix = glm::lookAt(m_transform->getTransformedPos(),
										m_transform->getTransformedPos() + m_transform->getForward(), m_transform->getUp());
								m_needsMatrixUpdate = false;
						}
				}

				void CameraComponent::render()
				{
				}

				void CameraComponent::setFoV(int _value)
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
						m_perspMatrix = glm::perspective(glm::radians((float)m_fov), getAspectRatio(), m_nearPlane, m_farPlane);
				}

				void CameraComponent::setSize(float _value)
				{
						m_size = _value;
						m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight, m_nearPlane, m_farPlane);
				}

				void CameraComponent::setNearPlane(float _value)
				{
						m_nearPlane = _value;
						m_perspMatrix = glm::perspective(glm::radians((float)m_fov), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight, m_nearPlane, m_farPlane);
				}

				void CameraComponent::setFarPlane(float _value)
				{
						m_farPlane = _value;
						m_perspMatrix = glm::perspective(glm::radians((float)m_fov), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight, m_nearPlane, m_farPlane);
				}

				void CameraComponent::resize(int _screenWidth, int _screenHeight)
				{
						m_screenWidth = _screenWidth;
						m_screenHeight = _screenHeight;
						//projection matrix
						m_perspMatrix = glm::perspective(glm::radians((float)m_fov), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight, m_nearPlane, m_farPlane);
				}

				const glm::mat4 & CameraComponent::getProjectionMatrix() const noexcept
				{
						if (m_projType == ProjectionType::ORTHOGRAPHIC) return m_orthoMatrix;
						else 
								return m_perspMatrix;
				}
		}
}
