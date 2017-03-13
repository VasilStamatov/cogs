#include "MeshRenderer.h"
#include "Entity.h"

#include "Mesh.h"
#include "Material.h"
#include "Renderer3D.h"

namespace cogs
{
		MeshRenderer::MeshRenderer(std::weak_ptr<Mesh> _mesh,
				//std::weak_ptr<Material> _material,
				std::weak_ptr<Renderer3D> _renderer) :
				m_mesh(_mesh),
				//m_material(_material),
				m_renderer(_renderer)
		{
		}

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