#include "Camera.h"
#include "Entity.h"

#include <glm\gtc\matrix_transform.hpp>

namespace cogs
{
		namespace ecs
		{
				std::weak_ptr<Camera> Camera::s_mainCamera;
				std::weak_ptr<Camera> Camera::s_currentCamera;
				std::vector<std::weak_ptr<Camera>> Camera::s_allCameras;

				Camera::Camera(int _screenWidth,
						int _screenHeight,
						const ProjectionType& _projType) :
						m_projType(_projType),
						m_cameraWidth(_screenWidth),
						m_cameraHeight(_screenHeight)
				{
						//projection matrix
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_cameraWidth) * m_size, 0.0f, static_cast<float>(m_cameraHeight) * m_size, m_nearPlane, m_farPlane);
				}

				Camera::~Camera()
				{
				}

				void Camera::init()
				{
						m_transform = m_entity.lock()->getComponent<Transform>();
						m_oldTransform = *m_transform.lock();

						if (m_entity.lock()->getName() == "MainCamera")
						{
								setMain(m_entity.lock()->getComponent<Camera>());
						}
						addCamera(m_entity.lock()->getComponent<Camera>());
				}

				void Camera::update(float _deltaTime)
				{
						if (!(*m_transform.lock() == m_oldTransform))
						{
								m_viewMatrix = glm::lookAt(m_transform.lock()->worldPosition(),
										m_transform.lock()->worldPosition() + m_transform.lock()->worldForwardAxis(),
										m_transform.lock()->worldUpAxis());
								m_oldTransform = *m_transform.lock();
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
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_cameraWidth) * m_size, 0.0f, static_cast<float>(m_cameraHeight) * m_size, m_nearPlane, m_farPlane);
				}

				void Camera::offsetSize(float _value)
				{
						m_size += _value;
						if (m_size < 1.0f)
						{
								m_size = 1.0f;
						}
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_cameraWidth) * m_size, 0.0f, static_cast<float>(m_cameraHeight) * m_size, m_nearPlane, m_farPlane);
				}

				void Camera::setNearPlane(float _value)
				{
						m_nearPlane = _value;
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_cameraWidth) * m_size, 0.0f, static_cast<float>(m_cameraHeight) * m_size, m_nearPlane, m_farPlane);
				}

				void Camera::setFarPlane(float _value)
				{
						m_farPlane = _value;
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_cameraWidth) * m_size, 0.0f, static_cast<float>(m_cameraHeight) * m_size, m_nearPlane, m_farPlane);
				}

				void Camera::resize(int _screenWidth, int _screenHeight)
				{
						m_cameraWidth = _screenWidth;
						m_cameraHeight = _screenHeight;
						//projection matrix
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)), getAspectRatio(), m_nearPlane, m_farPlane);
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_cameraWidth) * m_size, 0.0f, static_cast<float>(m_cameraHeight) * m_size, m_nearPlane, m_farPlane);
				}

				const glm::mat4 & Camera::getProjectionMatrix() const noexcept
				{
						if (m_projType == ProjectionType::ORTHOGRAPHIC) return m_orthoMatrix;
						else
								return m_perspMatrix;
				}
				void Camera::renderSkybox()
				{
						if (!m_skybox.expired())
						{
								m_skybox.lock()->render();
						}
				}
		}
}