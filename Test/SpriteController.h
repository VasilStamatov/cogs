#ifndef SPRITE_CONTROLLER_H
#define SPRITE_CONTROLLER_H

#include <cogs\Entity.h>
#include <cogs\Input.h>

namespace ce = cogs::ecs;
namespace cu = cogs::utils;

class SpriteController : public ce::Component
{
public:
		SpriteController()
		{

		}
		~SpriteController()
		{

		}

		/**
		* \brief The initialize component function
		*/
		void init() override
		{
				m_transform = m_entity.lock()->getComponent<ce::Transform>();
		}

		/**
		* \brief The update component function
		*/
		void update(float _deltaTime) override
		{
				if (cu::Input::isKeyDown(cu::KeyCode::Q))
				{
						m_transform.lock()->rotate(0.0f, 0.0f, glm::radians(5.0f));
				}
		}

private:
		std::weak_ptr<ce::Transform> m_transform; ///< the paddle's rigidbody
};
#endif // !SPRITE_CONTROLLER_H
