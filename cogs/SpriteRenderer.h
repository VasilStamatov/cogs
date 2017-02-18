#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Component.h"
#include "Material.h"
#include "Sprite.h"
#include "Renderer.h"

namespace cogs
{
		namespace ecs
		{
				class SpriteRenderer : public Component
				{
				public:
						SpriteRenderer(std::weak_ptr<graphics::Sprite> _sprite,
								std::weak_ptr<graphics::Renderer> _renderer);
						SpriteRenderer() {}
						~SpriteRenderer();

						void init() override;

						void update(float _deltaTime) override;

						void render() override;

						/**
						* Getters
						*/
						std::weak_ptr<graphics::Sprite> getSprite()			const noexcept { return m_sprite; }

						/**
						* Setters
						*/
						void setModel(std::weak_ptr<graphics::Sprite> _sprite)							   { m_sprite		 = _sprite; }
						void setRenderer(std::weak_ptr<graphics::Renderer> _renderer)	{ m_renderer = _renderer; }

				private:
						std::weak_ptr<graphics::Sprite> m_sprite;
						std::weak_ptr<graphics::Renderer> m_renderer;
				};
		}
}
#endif // !SPRITE_RENDERER_H