#ifndef SPRITE_H
#define SPRITE_H

#include "Entity.h"
#include "SpriteRenderer.h"

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
						Sprite(const glm::vec4& _color, graphics::SpriteRenderer* _renderer);

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

						/**
								* Getters
								*/
						const glm::vec4& getColor()			 const noexcept { return m_color; }

				private:
						Transform* m_transform{ nullptr };
						glm::vec4 m_color{ 0.0f };

						graphics::SpriteRenderer* m_renderer;
				};
		}
}
#endif // !SPRITE_H