#include "Sprite.h"
#include "Entity.h"

namespace cogs
{
		namespace ecs
		{
				Sprite::Sprite() { }
				Sprite::Sprite(const glm::vec2& _size, const glm::vec4& _color,
						const std::string& _texturePath, bool _alpha, std::weak_ptr<graphics::SpriteRenderer> _renderer)
						: m_color(_color), m_renderer(_renderer), m_size(_size)
				{
						setUVdefaults();
						m_texture.load(_texturePath, _texturePath, _alpha);
				}
				
				Sprite::~Sprite()
				{
						m_transform = nullptr;
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
						m_renderer.lock()->submit(m_entity);
				}

				void Sprite::setTexture(const std::string & _filePath, bool _alpha)
				{
						m_texture.load(_filePath, _filePath, _alpha);
				}

				void Sprite::setRenderer(std::weak_ptr<graphics::SpriteRenderer> _renderer)
				{
						m_renderer = _renderer;
				}

				void Sprite::setUVdefaults()
				{
						//bottom left
						m_uv.at(0) = glm::vec2(0, 0);

						//top left
						m_uv.at(1) = glm::vec2(0, 1);

						//top right
						m_uv.at(2) = glm::vec2(1, 1);

						//bottom right
						m_uv.at(3) = glm::vec2(1, 0);
				}
		}
}