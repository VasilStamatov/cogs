#ifndef SPRITE_H
#define SPRITE_H

#include <glm\vec2.hpp>
#include <memory>

#include "Color.h"
#include "GLTexture2D.h"

namespace cogs
{
		namespace graphics
		{
				/**
						* The sprite
						*/
				class Sprite
				{
				public:
						Sprite(const std::string& _name, std::weak_ptr<GLTexture2D> _texture, const glm::vec2& _size, const Color& _color)
								: m_name(_name), m_size(_size), m_color(_color), m_texture(_texture)
						{
						}
						Sprite() {}
						~Sprite() {}

						void setName(const std::string& _name) { m_name = _name; }
						void setSize(const glm::vec2& _size) { m_size = _size; }
						void setColor(const Color& _color) { m_color = _color; }
						void setTexture(std::weak_ptr<GLTexture2D> _filePath) { m_texture = m_texture; }

						const std::string& getName() { return m_name; }
						const glm::vec2& getSize() { return m_size; }
						const Color& getColor() { return m_color; }
				  std::weak_ptr<GLTexture2D> getTexture() { return m_texture; }

				private:
						std::string m_name{ "" };
						glm::vec2 m_size{ 0.0f, 0.0f };
						Color m_color{ Color::white };
						std::weak_ptr<GLTexture2D> m_texture;
				};
		}
}
#endif // !SPRITE_H