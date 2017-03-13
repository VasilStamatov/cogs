#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Component.h"

namespace cogs
{
		class Mesh;
		class Material;
		class Renderer3D;
		/**
		* \brief This component, given to an entity should submit it to a 3D renderer
		*/
		class MeshRenderer : public Component
		{
		public:
				/**
				* \brief Construct the mesh renderer by giving if a reference to the mesh it will submit,
				* the material of the mesh, and the renderer the mesh will be submitted to
				*/
				MeshRenderer(std::weak_ptr<Mesh> _mesh,
						//std::weak_ptr<Material> _material,
						std::weak_ptr<Renderer3D> _renderer);
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
				std::weak_ptr<Mesh> getMesh()									const noexcept { return m_mesh; }
				//std::weak_ptr<Material> getMaterial()	const noexcept { return m_material; }

				/**
				* Setters
				*/
				void setMesh(std::weak_ptr<Mesh> _mesh) { m_mesh = _mesh; }
				//void setMaterial(std::weak_ptr<Material> _material) { m_material = _material; }
				void setRenderer(std::weak_ptr<Renderer3D> _renderer) { m_renderer = _renderer; }

		private:
				std::weak_ptr<Mesh> m_mesh; ///< reference to the mesh rendererd
				//std::weak_ptr<Material> m_material; ///< reference to the material the mesh is rendered with
				std::weak_ptr<Renderer3D> m_renderer; ///< reference to the renderer the mesh is submitted to
		};
}
#endif // !MESH_RENDERER_H
