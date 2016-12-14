#ifndef SPRITE_H
#define SPRITE_H

#include "Entity.h"
#include "SpriteRenderer.h"
#include "GLTexture.h"

namespace cogs
{
		namespace ecs
		{
				/**
						* The sprite component
						*/
				class Sprite : public Component
				{
				public:
						/**
								* Default sprite constructor
								*/
						Sprite();

						/**
								* Sprite contstructor
								* \param _color - the color of the sprite
								* \param _renderer - the renderer this sprite will submit itself to
								*/
						Sprite(const glm::vec2& _size, const glm::vec4& _color,
								const std::string& _texturePath, bool _alpha, graphics::SpriteRenderer* _renderer);

						/**
								* Sprite destructor
								*/
						~Sprite();

						/**
								* The initialize component function
								*/
						void init() override;

						/**
								* The update component function
								*/
						void update(float _deltaTime) override;

						/**
						 	* The render component function
						 	*/
						void render() override;

						void setTexture(const std::string& _filePath, bool _alpha);

						/**
								* Getters
								*/
						const glm::vec4& getColor()													 const noexcept { return m_color; }
						const glm::vec2& getSize()														 const noexcept { return m_size; }
						const std::array<glm::vec2, 4>& getUV()		const noexcept	{ return m_uv; }
						const graphics::GLTexture& getTexture()  const noexcept { return m_texture; }

				private:
						void setUVdefaults();

				private:
						Transform* m_transform{ nullptr };
						glm::vec4 m_color{ 0.0f };
						glm::vec2 m_size{ 0.0f };
						std::array<glm::vec2, 4> m_uv;

						graphics::GLTexture m_texture;
						graphics::SpriteRenderer* m_renderer = nullptr;
				};
		}
}
#endif // !SPRITE_H