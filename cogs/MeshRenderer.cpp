#include "MeshRenderer.h"
#include "Camera.h"
#include "Entity.h"

namespace cogs
{
		namespace ecs
		{
				MeshRenderer::MeshRenderer(std::unique_ptr<graphics::Model> _model,
						std::unique_ptr<graphics::Material> _material)
						: m_model(std::move(_model)), m_material(std::move(_material)) { }

				MeshRenderer::~MeshRenderer()
				{
				}
				void MeshRenderer::init()
				{
				}
				void MeshRenderer::update(float _deltaTime)
				{
				}
				void MeshRenderer::render(std::weak_ptr<Camera> _camera)
				{
						m_material->bind();

						/* add all the new per-model uniforms,
						such as camera transform matrices and the model matrix
						the .add functions in the material class treat the maps as associative arrays, meaning add
						changes a value if it already exists (and does not actually add a new one) */

						m_material->addMat4("model", m_entity.lock()->getComponent<Transform>().lock()->worldTransform());
						m_material->addMat4("view", _camera.lock()->getViewMatrix());
						m_material->addMat4("projection", _camera.lock()->getProjectionMatrix());

						m_material->uploadUniforms();

						for (auto& mesh : m_model->getMeshes())
						{
								m_material->uploadTextures(mesh.getTextures());

								mesh.render();
						}

						m_material->unbind();
				}
		}
}