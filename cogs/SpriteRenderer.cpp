#include "SpriteRenderer.h"

#include "Entity.h"
#include "Sprite.h"
#include "Renderer2D.h"

namespace cogs
{
		SpriteRenderer::SpriteRenderer(std::weak_ptr<Sprite> _sprite,
				std::weak_ptr<Renderer2D> _renderer)
				: m_sprite(_sprite), m_renderer(_renderer)
		{
		}
		SpriteRenderer::~SpriteRenderer()
		{
		}
		void SpriteRenderer::init()
		{
		}
		void SpriteRenderer::update(float _deltaTime)
		{
		}
		void SpriteRenderer::render()
		{
				m_renderer.lock()->submit(m_entity);
		}
}