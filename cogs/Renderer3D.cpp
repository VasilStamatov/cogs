#include "Renderer3D.h"

#include "Camera.h"
#include "GLSLProgram.h"
#include "MeshRenderer.h"
#include "Light.h"

namespace cogs
{
		namespace graphics
		{
				Renderer3D::Renderer3D(std::weak_ptr<GLSLProgram> _shader) : Renderer(_shader)
				{
				}
				Renderer3D::Renderer3D()
				{
				}
				Renderer3D::~Renderer3D()
				{
				}
				void Renderer3D::init()
				{

				}
				void Renderer3D::submit(std::weak_ptr<ecs::Entity> _entity)
				{
						m_entities.push_back(_entity);
				}
				void Renderer3D::flush()
				{
						//get the current cam that will be used for space-transforms
						std::weak_ptr<ecs::Camera> currentCam = ecs::Camera::getCurrent();

						//begind using the shader this renderer uses
						m_shader.lock()->use();

						//upload the projection and view matrices as they are the same for every entity in this render queue
						m_shader.lock()->uploadValue("projection", currentCam.lock()->getProjectionMatrix());
						m_shader.lock()->uploadValue("view", currentCam.lock()->getViewMatrix());

						//upload the lights as they are also the same for the whole scene
						std::vector<std::weak_ptr<ecs::Light>> lights = ecs::Light::getAllLights();

						int pointLightIndex{ 0 };
						int spotLightIndex{ 0 };
						int dirLightIndex{ 0 };

						for (std::weak_ptr<ecs::Light> light : ecs::Light::getAllLights())
						{
								if (!light.expired())
								{
										switch (light.lock()->getLightType())
										{
												case ecs::LightType::POINT:
												{
														m_shader.lock()->uploadValue("pointLights[" + std::to_string(pointLightIndex++) + "]", light);
														break;
												}
												case ecs::LightType::SPOT:
												{
														m_shader.lock()->uploadValue("spotLights[" + std::to_string(spotLightIndex++) + "]", light);
														break;
												}
												case ecs::LightType::DIRECTIONAL:
												{
														m_shader.lock()->uploadValue("dirLights[" + std::to_string(dirLightIndex++) + "]", light);
														break;
												}
												default:
														printf("Invalid Light");
														break;
										}
								}
						}

						for (std::weak_ptr<ecs::Entity> entity : m_entities)
						{
								//upload the model matrix as it's the same for 1 whole entity
								m_shader.lock()->uploadValue("model", entity.lock()->getComponent<ecs::Transform>().lock()->worldTransform());

								//get the mesh
								std::weak_ptr<Mesh> mesh = entity.lock()->getComponent<ecs::MeshRenderer>().lock()->getMesh();
								//get the Material
								std::weak_ptr<Material> material = entity.lock()->getComponent<ecs::MeshRenderer>().lock()->getMaterial();

						 	m_shader.lock()->uploadMaterial(material);

								mesh.lock()->render();
						}

						//finally unbind the current shader program
						m_shader.lock()->unUse();
				}
				void Renderer3D::begin()
				{
						m_entities.clear();
				}
				void Renderer3D::end()
				{
						//TODO: possibly sorting and batching/preparing for instanced rendering ?
				}
		}
}