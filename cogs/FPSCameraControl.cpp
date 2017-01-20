#include "FPSCameraControl.h"

#include "Entity.h"
#include "Input.h"

namespace cogs
{
		namespace ecs
		{
				FPSCameraControl::FPSCameraControl()
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
								m_transform->translate(glm::vec3(0.0f, 0.0f, -0.5f));
						}
						if (utils::Input::isKeyDown(utils::KeyCode::S))
						{
								m_transform->translate(glm::vec3(0.0f, 0.0f, 0.5f));
						}
						if (utils::Input::isKeyDown(utils::KeyCode::A))
						{
								m_transform->translate(glm::vec3(-0.5f, 0.0f, 0.0f));
						}
						if (utils::Input::isKeyDown(utils::KeyCode::D))
						{
								m_transform->translate(glm::vec3(0.5f, 0.0f, 0.0f));
						}
				}
				void FPSCameraControl::render()
				{
				}
		}
}