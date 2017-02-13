#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Component.h"
#include "Model.h"
#include "Material.h"
#include "Renderer3D.h"

namespace cogs
{
		namespace ecs
		{
				/**
				* This component, given to an entity should render a mesh with a material
				*/
				class MeshRenderer : public Component
				{
				public:
						/**
						* Pass the model that you want to render and the material to be rendered with to the contructor
						*/
						MeshRenderer(std::weak_ptr<graphics::Model> _model,
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
						std::weak_ptr<graphics::Material> getMaterial() const noexcept { return m_material; }
						std::weak_ptr<graphics::Model> getModel()					  const noexcept { return m_model; }

						/**
						* Setters
						*/
						void setMaterial(std::weak_ptr<graphics::Material> _material) { m_material = _material; }
						void setModel(std::weak_ptr<graphics::Model> _model) { m_model = _model; }
						void setRenderer(std::weak_ptr<graphics::Renderer3D> _renderer) { m_renderer = _renderer; }

				private:
						std::weak_ptr<graphics::Model> m_model;
						std::weak_ptr<graphics::Material> m_material;
						std::weak_ptr<graphics::Renderer3D> m_renderer;
				};
		}
}
#endif // !MESH_RENDERER_H
