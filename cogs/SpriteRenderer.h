#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Component.h"

namespace cogs
{
		class Renderer2D;
		class Sprite;
		/**
		* \brief This component, given to an entity should submit it to a 2D renderer
		*/
		class SpriteRenderer : public Component
		{
		public:
				/**
				* \brief Construct the sprite renderer by giving if a reference to the sprite it will submit,
				* and the renderer the sprite will be submitted to
				*/
				SpriteRenderer(std::weak_ptr<Sprite> _sprite,
						std::weak_ptr<Renderer2D> _renderer);
				SpriteRenderer() {}
				~SpriteRenderer();

				//Called after contructor
				void init() override;

				//called every frame
				void update(float _deltaTime) override;

				//submits the sprite to the 2d renderer
				void render() override;

				/**
				* Getters
				*/
				std::weak_ptr<Sprite> getSprite()			const noexcept { return m_sprite; }

				/**
				* Setters
				*/
				void setModel(std::weak_ptr<Sprite> _sprite) { m_sprite = _sprite; }
				void setRenderer(std::weak_ptr<Renderer2D> _renderer) { m_renderer = _renderer; }

		private:
				std::weak_ptr<Sprite> m_sprite; ///< reference to the sprite to be submitted
				std::weak_ptr<Renderer2D> m_renderer; ///< reference to the renderer
		};
}
#endif // !SPRITE_RENDERER_H