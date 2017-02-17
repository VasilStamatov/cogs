#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Component.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer3D.h"

namespace cogs
{
		namespace ecs
		{
				/**
				* This component, given to an entity should submit it to a 3D renderer
				*/
				class MeshRenderer : public Component
				{
				public:
						/**
						* Pass the model (collection of meshes) that will be rendered
						* and the renderer that it will be submitted to
						*/
						MeshRenderer(std::weak_ptr<graphics::Mesh> _mesh,
								std::weak_ptr<graphics::Material> _material,
								std::weak_ptr<graphics::Renderer3D> _renderer);
						~MeshRenderer();

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
						std::weak_ptr<graphics::Mesh> getMesh()									const noexcept { return m_mesh; }
						std::weak_ptr<graphics::Material> getMaterial()	const noexcept { return m_material; }

						/**
						* Setters
						*/
						void setMesh(std::weak_ptr<graphics::Mesh> _mesh) { m_mesh = _mesh; }
						void setMaterial(std::weak_ptr<graphics::Material> _material) { m_material = _material; }
						void setRenderer(std::weak_ptr<graphics::Renderer3D> _renderer) { m_renderer = _renderer; }

				private:
						std::weak_ptr<graphics::Mesh> m_mesh;
						std::weak_ptr<graphics::Material> m_material;
						std::weak_ptr<graphics::Renderer3D> m_renderer;
				};
		}
}
#endif // !MESH_RENDERER_H
