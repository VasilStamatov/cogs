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
						m_transform = nullptr;
				}
				void FPSCameraControl::init()
				{
						m_transform = m_entity->getComponent<Transform>();
				}
				void FPSCameraControl::update(float _deltaTime)
				{
						if (utils::Input::isKeyDown(utils::KeyCode::W))
						{
								//m_transform->translate(glm::vec3(0.0f, 0.0f, -0.5f * _deltaTime));
								m_transform->translate(m_transform->localForwardAxis() * m_moveSpeed * _deltaTime);
						}
						if (utils::Input::isKeyDown(utils::KeyCode::S))
						{
								//m_transform->translate(glm::vec3(0.0f, 0.0f, 0.5f * _deltaTime));
								m_transform->translate(m_transform->localForwardAxis() * -m_moveSpeed * _deltaTime);
						}
						if (utils::Input::isKeyDown(utils::KeyCode::A))
						{
								//m_transform->translate(glm::vec3(-0.5f * _deltaTime, 0.0f, 0.0f));
								m_transform->translate(m_transform->localRightAxis() * -m_moveSpeed * _deltaTime);
						}
						if (utils::Input::isKeyDown(utils::KeyCode::D))
						{
								//m_transform->translate(glm::vec3(0.5f * _deltaTime, 0.0f, 0.0f));
								m_transform->translate(m_transform->localRightAxis() * m_moveSpeed * _deltaTime);
						}

						const glm::vec2& mouseMotion = utils::Input::getMouseMotion();

						if (mouseMotion.x != 0.0f || mouseMotion.y != 0.0f)
						{
								m_transform->rotate(glm::vec3(-mouseMotion.y * m_moveSpeed * _deltaTime,
										-mouseMotion.x * m_moveSpeed * _deltaTime, 0.0f));

								utils::Input::setMouseMotion(0.0f, 0.0f);
						}
				}
		}
}