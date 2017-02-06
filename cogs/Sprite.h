#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"
#include "SpriteRenderer.h"
#include "GLTexture.h"

namespace cogs
{
		namespace ecs
		{
				class Transform;
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
						Sprite(const glm::vec2& _size, const graphics::Color& _color,
								const std::string& _texturePath, bool _alpha, std::weak_ptr<graphics::SpriteRenderer> _renderer);

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
						void render(std::weak_ptr<Camera> _camera) override;

						void setTexture(const std::string& _filePath, bool _alpha);
						void setRenderer(std::weak_ptr<graphics::SpriteRenderer> _renderer);

						/**
								* Getters
								*/
						inline const graphics::Color& getColor()								const noexcept { return m_color; }
						inline const glm::vec2& getSize()														 const noexcept { return m_size; }
						inline const std::array<glm::vec2, 4>& getUV()		const noexcept	{ return m_uv; }
						inline const graphics::GLTexture& getTexture()  const noexcept { return m_texture; }

				private:
						void setUVdefaults();

				private:
						std::weak_ptr<Transform> m_transform;
						graphics::Color m_color;
						glm::vec2 m_size{ 0.0f };
						std::array<glm::vec2, 4> m_uv;

						graphics::GLTexture m_texture;
						std::weak_ptr<graphics::SpriteRenderer> m_renderer;
				};
		}
}
#endif // !SPRITE_H