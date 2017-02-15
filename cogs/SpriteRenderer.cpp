#include "SpriteRenderer.h"

#include "Entity.h"

namespace cogs
{
		namespace ecs
		{
				SpriteRenderer::SpriteRenderer(std::weak_ptr<graphics::Sprite> _sprite,
						std::weak_ptr<graphics::Renderer2D> _renderer)
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
}