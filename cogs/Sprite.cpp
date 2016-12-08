#include "Sprite.h"

namespace cogs
{
		namespace ecs
		{
				Sprite::Sprite() { }
				Sprite::Sprite(const glm::vec2& _size, const glm::vec4& _color, graphics::SpriteRenderer* _renderer)
						: m_color(_color), m_renderer(_renderer), m_size(_size) { }
				
				Sprite::~Sprite()
				{
						m_transform = nullptr;
						m_renderer = nullptr;
				}

				void Sprite::init()
				{
						m_transform = m_entity->getComponent<Transform>();
				}

				void Sprite::update(float _deltaTime)
				{
						
				}

				void Sprite::render()
				{
						m_renderer->submit(m_entity);
				}
		}
}