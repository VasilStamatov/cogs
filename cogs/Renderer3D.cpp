#include "Renderer3D.h"

#include "Camera.h"
#include "GLSLProgram.h"
#include "MeshRenderer.h"
#include "Light.h"

#include <GL\glew.h>

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
						std::weak_ptr<ecs::Camera> currentCam = ecs::Camera::getCurrent();

						std::weak_ptr<Mesh> mesh = _entity.lock()->getComponent<ecs::MeshRenderer>().lock()->getMesh();

						std::weak_ptr<ecs::Transform> transform = _entity.lock()->getComponent<ecs::Transform>();

						//get the center vertex position in model space
						const MeshBoundingSphere& sphereBounds = mesh.lock()->getSphereBounds();

						//get the transformation matrix to world space
						const glm::mat4& toWorldMat = transform.lock()->worldTransform();

						//calculate the center vertex from model to world space
						glm::vec3 point = glm::vec3(toWorldMat * glm::vec4(sphereBounds.m_center, 1.0f));

						const glm::vec3& scale = transform.lock()->worldScale();

						//scale the radius
						float radius = sphereBounds.m_radius * glm::max(scale.x, glm::max(scale.y, scale.z));
						//submit the mesh if it's in the view frustum

						if (currentCam.lock()->sphereInFrustum(point, radius))
						{
								auto iter = m_entitiesMap.find(mesh.lock()->m_VAO);

								//check if it's not in the map
								if (iter == m_entitiesMap.end())
								{
										InstanceData instance;
										instance.mesh = mesh;
										m_entitiesMap.insert(std::make_pair(mesh.lock()->m_VAO, instance));
								}
								m_entitiesMap[mesh.lock()->m_VAO].worldmats.push_back(toWorldMat);
						}

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

						for (auto& it : m_entitiesMap)
						{
								InstanceData instances = it.second;

								const std::vector<SubMesh>& subMeshes = instances.mesh.lock()->getSubMeshes();
								const std::vector<std::weak_ptr<Material>>& materials = instances.mesh.lock()->getMaterials();

								//bind the per-instance buffers
								glBindBuffer(GL_ARRAY_BUFFER, instances.mesh.lock()->m_VBOs[instances.mesh.lock()->BufferObject::WORLDMAT]);
								//upload the data
								glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances.worldmats.size(), instances.worldmats.data(), GL_DYNAMIC_DRAW);

								glBindVertexArray(it.first);

								for (unsigned int i = 0; i < subMeshes.size(); i++)
								{
										const unsigned int materialIndex = subMeshes[i].m_materialIndex;
										assert(materialIndex < materials.size());

										if (!materials.at(materialIndex).expired())
										{
												m_shader.lock()->uploadMaterial(materials.at(materialIndex));
										}

										glDrawElementsInstancedBaseVertex(GL_TRIANGLES, subMeshes.at(i).m_numIndices, GL_UNSIGNED_INT,
												(void*)(sizeof(unsigned int) * subMeshes.at(i).m_baseIndex), instances.worldmats.size(),
												subMeshes.at(i).m_baseVertex);
								}

								glBindVertexArray(0);
						}

						//finally unbind the current shader program
						m_shader.lock()->unUse();
				}

				void Renderer3D::dispose()
				{
				}

				void Renderer3D::begin()
				{
						m_entitiesMap.clear();
				}

				void Renderer3D::end()
				{
						//TODO: possibly sorting and batching/preparing for instanced rendering ?
				}
		}
}