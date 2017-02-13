#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Component.h"
#include "Material.h"
#include "Sprite.h"
#include "Renderer2D.h"

namespace cogs
{
		namespace ecs
		{
				class SpriteRenderer : public Component
				{
				public:
						SpriteRenderer(std::weak_ptr<graphics::Sprite> _sprite,
								std::weak_ptr<graphics::Material> _material,
								std::weak_ptr<graphics::Renderer2D> _renderer);
						SpriteRenderer() {}
						~SpriteRenderer();

						void init() override;

						void update(float _deltaTime) override;

						void render() override;

						/**
						* Getters
						*/
						std::weak_ptr<graphics::Material> getMaterial() const noexcept { return m_material; }
						std::weak_ptr<graphics::Sprite>   getSprite()			const noexcept { return m_sprite; }

						/**
						* Setters
						*/
						void setMaterial(std::weak_ptr<graphics::Material> _material) { m_material = _material; }
						void setModel(std::weak_ptr<graphics::Sprite> _sprite)							 { m_sprite		 = _sprite; }
						void setRenderer(std::weak_ptr<graphics::Renderer2D> _renderer)	{ m_renderer = _renderer; }

				private:
						std::weak_ptr<graphics::Material> m_material;
						std::weak_ptr<graphics::Sprite> m_sprite;
						std::weak_ptr<graphics::Renderer2D> m_renderer;
				};
		}
}
#endif // !SPRITE_RENDERER_H