#include "FPSCameraControl.h"

#include "Entity.h"
#include "Input.h"

namespace cogs
{
		namespace ecs
		{
				FPSCameraControl::FPSCameraControl(float _moveSpeed) : m_moveSpeed(_moveSpeed)
				{
				}
				FPSCameraControl::~FPSCameraControl()
				{
				}
				void FPSCameraControl::init()
				{
						m_transform = m_entity.lock()->getComponent<Transform>();
				}
				void FPSCameraControl::update(float _deltaTime)
				{
						//const glm::vec3& orientation = m_transform.lock()->worldOrientation();

						////keep the z axis locked
						//m_transform.lock()->setWorldOrientation(glm::vec3(orientation.x, orientation.y, 0.0f));

						// Handle movement

						// Forward
						if (utils::Input::isKeyDown(utils::KeyCode::W))
						{
								m_transform.lock()->translate(m_transform.lock()->localForwardAxis() * m_moveSpeed * _deltaTime);
						}

						// Backwards
						if (utils::Input::isKeyDown(utils::KeyCode::S))
						{
								m_transform.lock()->translate(m_transform.lock()->localForwardAxis() * -m_moveSpeed * _deltaTime);
						}

						// Left
						if (utils::Input::isKeyDown(utils::KeyCode::A))
						{
								m_transform.lock()->translate(m_transform.lock()->localRightAxis() * -m_moveSpeed * _deltaTime);
						}

						// Right
						if (utils::Input::isKeyDown(utils::KeyCode::D))
						{
								m_transform.lock()->translate(m_transform.lock()->localRightAxis() * m_moveSpeed * _deltaTime);
						}

						const glm::vec2& mouseMotion = utils::Input::getMouseMotion();

						// Handle rotation
						if (mouseMotion.x != 0.0f || mouseMotion.y != 0.0f)
						{
								//how much the mouse moves horizontally
								float xRotation = (-mouseMotion.y * m_moveSpeed * _deltaTime);
								//how much the mouse moves vertically
								float yRotation = (-mouseMotion.x * m_moveSpeed * _deltaTime);

								m_transform.lock()->rotate(glm::vec3(glm::radians(xRotation), glm::radians(yRotation), 0.0f));

								utils::Input::setMouseMotion(0.0f, 0.0f);
						}
				}
		}
}