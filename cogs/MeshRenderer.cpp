#include "MeshRenderer.h"
#include "Entity.h"

namespace cogs
{
		namespace ecs
		{
				MeshRenderer::MeshRenderer(std::weak_ptr<graphics::Model> _model,
						std::weak_ptr<graphics::Material> _material,
						std::weak_ptr<graphics::Renderer3D> _renderer)
						: m_model(_model), m_material(_material), m_renderer(_renderer) { }

				MeshRenderer::~MeshRenderer()
				{
				}
				void MeshRenderer::init()
				{
				}
				void MeshRenderer::update(float _deltaTime)
				{
				}
				void MeshRenderer::render()
				{
						m_renderer.lock()->submit(m_entity);
				}
		}
}