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
								float xAxisRotation = (-mouseMotion.y * m_moveSpeed * _deltaTime);
								//how much the mouse moves vertically
								float yAxisRotation = (-mouseMotion.x * m_moveSpeed * _deltaTime);

								//rotate around the x axis locally and around the y axis using the world up vector
								//this locks rotating around z axis, but instroduces gimbal locking again
								m_transform.lock()->rotate(m_transform.lock()->localRightAxis(), glm::radians(xAxisRotation));
								m_transform.lock()->rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(yAxisRotation));
								
								/*const glm::vec3& orientation = m_transform.lock()->localOrientation();

								printf("Orientation x: %f\n", orientation.x);
								printf("Orientation x degree: %f\n\n", glm::degrees(orientation.x));

								printf("Orientation y: %f\n", orientation.y);
								printf("Orientation y degree: %f\n\n", glm::degrees(orientation.y));

								printf("Orientation z: %f\n", orientation.z);
								printf("Orientation z degree: %f\n\n", glm::degrees(orientation.z));*/

								utils::Input::setMouseMotion(0.0f, 0.0f);
						}
				}
		}
}