#include "Camera.h"
#include "Entity.h"
#include "Skybox.h"
#include "Framebuffer.h"
#include "BulletDebugRenderer.h"

#include <glm\gtc\matrix_transform.hpp>

namespace cogs
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
				updateProjection();
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
				else
				{
						addCamera(m_entity.lock()->getComponent<Camera>());
				}
		}

		void Camera::update(float _deltaTime)
		{
				if (!(*m_transform.lock() == m_oldTransform))
				{
						updateView();
				}
		}

		void Camera::setFoV(int _value)
		{
				if (m_fov < 1)
				{
						m_fov = 1;
				}
				else if (m_fov > 179)
				{
						m_fov = 179;
				}
				updateProjection();
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
				updateProjection();
		}

		void Camera::setSize(float _value)
		{
				m_size = _value;
				if (m_size < 1.0f)
				{
						m_size = 1.0f;
				}
				updateProjection();
		}

		void Camera::offsetSize(float _value)
		{
				m_size += _value;
				if (m_size < 1.0f)
				{
						m_size = 1.0f;
				}
				updateProjection();
		}

		void Camera::setNearPlane(float _value)
		{
				m_nearPlane = _value;
				updateProjection();
		}

		void Camera::setFarPlane(float _value)
		{
				m_farPlane = _value;
				updateProjection();
		}

		void Camera::setClippingPlanes(float _zNear, float _zFar)
		{
				m_nearPlane = _zNear;
				m_farPlane = _zFar;
				updateProjection();
		}

		void Camera::setProjectionType(const ProjectionType & _projType)
		{
				m_projType = _projType;
				updateProjection();
		}

		void Camera::resize(int _screenWidth, int _screenHeight)
		{
				m_cameraWidth = _screenWidth;
				m_cameraHeight = _screenHeight;
				//projection matrix
				updateProjection();
		}

		const glm::mat4 & Camera::getProjectionMatrix() const noexcept
		{
				if (m_projType == ProjectionType::ORTHOGRAPHIC) return m_orthoMatrix;
				else
						return m_perspMatrix;
		}

		glm::vec2 Camera::convertWorldToScreen(const glm::vec3 & _worldCoordinate)
		{
				glm::vec4 clipCoords = m_perspMatrix * m_viewMatrix * glm::vec4(_worldCoordinate, 1.0f);

				clipCoords.x /= clipCoords.w;
				clipCoords.y /= clipCoords.w;
				clipCoords.z /= clipCoords.w;

				glm::vec2 screenPoint(0.0f);

				screenPoint.x = ((clipCoords.x + 1) / 2.0f) * m_cameraWidth;
				screenPoint.y = ((1 - clipCoords.y) / 2.0f) * m_cameraHeight;

				return screenPoint;
		}

		glm::vec3 Camera::convertScreenToWorld(const glm::vec2 & _screenCoordinate)
		{
				float x = 2.0f * _screenCoordinate.x / m_cameraWidth - 1;
				float y = -2.0f * _screenCoordinate.y / m_cameraHeight + 1;
				glm::mat4 viewProjInverse = glm::inverse(m_perspMatrix * m_viewMatrix);
				glm::vec3 worldCoord(x, y, 0.0f);
				return viewProjInverse * glm::vec4(worldCoord, 1.0f);
		}

		void Camera::renderFrustum(BulletDebugRenderer* _renderer)
		{
				m_frustum.render(_renderer);
		}

		void Camera::renderSkybox()
		{
				if (!m_skybox.expired())
				{
						m_skybox.lock()->render();
				}
		}

		void Camera::updateView()
		{
				m_oldTransform = *m_transform.lock();

				m_viewMatrix = glm::inverse(m_oldTransform.worldTransform());

				m_frustum.update(m_oldTransform.worldPosition(),
						m_oldTransform.worldForwardAxis(),
						m_oldTransform.worldRightAxis(),
						m_oldTransform.worldUpAxis());
		}

		void Camera::updateProjection()
		{
				if (m_projType == ProjectionType::PERSPECTIVE)
				{
						m_perspMatrix = glm::perspective(glm::radians(static_cast<float>(m_fov)),
								getAspectRatio(), m_nearPlane, m_farPlane);

						//update the frustum
						m_frustum.setCamInternals(glm::radians((float)m_fov), getAspectRatio(), m_nearPlane, m_farPlane);
				}
				else
				{
						m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_cameraWidth) * m_size,
								0.0f, static_cast<float>(m_cameraHeight) * m_size, m_nearPlane, m_farPlane);
				}
		}
}