#include "MeshRenderer.h"
#include "Camera.h"

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
						Entity* root = m_entity->getRoot();
						m_camera = root->getComponentInChildren<Camera>();
				}
				void MeshRenderer::update(float _deltaTime)
				{
				}
				void MeshRenderer::render()
				{
						m_material->bind();

						/* add all the new per-model uniforms,
						such as camera transform matrices and the model matrix
						the .add functions in the material class treat the maps as associative arrays, meaning add
						changes a value if it already exists (and does not actually add a new one) */

						m_material->addMat4("model", m_entity->getComponent<Transform>()->worldTransform());
						m_material->addMat4("view", m_camera->getViewMatrix());
						m_material->addMat4("projection", m_camera->getProjectionMatrix());

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