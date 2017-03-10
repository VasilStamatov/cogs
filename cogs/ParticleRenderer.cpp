#include "ParticleRenderer.h"

#include "Camera.h"
#include "GLSLProgram.h"
#include "ParticleSystem.h"

#include <GL\glew.h>
#include <glm\gtx\norm.hpp>

namespace cogs
{
		namespace graphics
		{
				ParticleRenderer::ParticleRenderer(std::weak_ptr<GLSLProgram> _shader) : Renderer2D(_shader)
				{
						init();
						setSortType(SpriteSortType::BACK_TO_FRONT);
				}
				ParticleRenderer::~ParticleRenderer()
				{
				}
				void ParticleRenderer::init()
				{
						//generate the vertex array buffer
						glGenVertexArrays(1, &m_VAO);

						//generate the vertex buffer objects
						glGenBuffers(BufferObjects::NUM_BUFFERS, m_VBOs);

						//bind the vao and continue working on the vbos under it
						glBindVertexArray(m_VAO);

						//bind the position buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::POSITION]);

						//tell opengl how to read it

						glEnableVertexAttribArray(BufferObjects::POSITION); // on channel 0

						glVertexAttribPointer(BufferObjects::POSITION, // index/channel 0
								4,								// size = 4 (3 for the xyz position coordinates, 1 for the width/size)
								GL_FLOAT, // GL_FLOAT = tell it that the numbers will be floats
								GL_FALSE, // don't normalize them
								0,								// 0 stride as it's tightly packed data (1 buffer per data rather than 1 buffer with a vertex struct)
								nullptr);  // no offset as it's tightly packed

																		// bind the color buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::COLOR]);

						glEnableVertexAttribArray(BufferObjects::COLOR); //channel 2

						glVertexAttribPointer(BufferObjects::COLOR, // index 2
								4,																// size = 4 ( 4 numbers for r,g,b and a )
								GL_UNSIGNED_BYTE, // the 4 numbers will be bytes (0-255)
								GL_TRUE,										// normalize them (divided by 255)
								0,																// tightly packed
								nullptr);									// no offset

						// unbind the vao after the setup is done
						glBindVertexArray(0);
				}
				void ParticleRenderer::begin()
				{
						m_spriteBatches.clear();
						m_entities.clear();
				}
				void ParticleRenderer::submit(std::weak_ptr<ecs::Entity> _entity)
				{
						m_entities.push_back(_entity);
				}
				void ParticleRenderer::end()
				{
						//after everything has been submitted, sort it and create the batches
						sortSprites();
						createSpriteBatches();
				}

				void ParticleRenderer::flush()
				{
						std::weak_ptr<ecs::Camera> currentCam = ecs::Camera::getCurrent();

						m_shader.lock()->use();
						m_shader.lock()->uploadValue("projection", currentCam.lock()->getProjectionMatrix());
						m_shader.lock()->uploadValue("view", currentCam.lock()->getViewMatrix());
						//m_shader.lock()->uploadValue("cameraPos", currentCam.lock()->getEntity().lock()->getComponent<ecs::Transform>().lock()->worldPosition());

						/* Bind the VAO. This sets up the opengl state we need, including the
						vertex attribute pointers and it binds the VBO */
						glBindVertexArray(m_VAO);
						glDepthMask(GL_FALSE);
						//set the blend func to a nice one for particles
						//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

						for (size_t i = 0; i < m_spriteBatches.size(); i++)
						{
								glBindTexture(GL_TEXTURE_2D, m_spriteBatches[i].m_texture);

								//first index is offset, numindices = num vertices (just reuising the spritebatch struct)
								glDrawArrays(GL_POINTS, m_spriteBatches[i].m_firstIndex, m_spriteBatches[i].m_numIndices);
						}
						//unbind the vao
						glBindVertexArray(0);
						glDepthMask(GL_TRUE);

						//restore the usual blending
						//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

						m_shader.lock()->unUse();
				}

				void ParticleRenderer::dispose()
				{
						// Dispose of all the buffest if they have't been disposed already

						if (m_VAO != 0)
						{
								glDeleteVertexArrays(1, &m_VAO);
								m_VAO = 0;
						}

						glDeleteBuffers(BufferObjects::NUM_BUFFERS, m_VBOs);
						for (size_t i = 0; i < BufferObjects::NUM_BUFFERS; i++)
						{
								m_VBOs[i] = 0;
						}
				}
				void ParticleRenderer::createSpriteBatches()
				{
						if (m_entities.empty())
						{
								return;
						}

						// This will store all the vertices that we need to upload
						std::vector<glm::vec4> positions;
						std::vector<Color> colors;

						int offset = 0;

						for (size_t currentPSystem = 0; currentPSystem < m_entities.size(); currentPSystem++)
						{
								std::weak_ptr<ecs::ParticleSystem> particleSystem = m_entities.at(currentPSystem).lock()->getComponent<ecs::ParticleSystem>();
								std::weak_ptr<GLTexture2D> texture = particleSystem.lock()->getTexture();
								std::weak_ptr<ecs::Transform> transform = m_entities.at(currentPSystem).lock()->getComponent<ecs::Transform>();

								ecs::Particle* particles = particleSystem.lock()->getParticles();

								if (currentPSystem == 0)
								{
										m_spriteBatches.emplace_back(offset, 0, texture.lock()->getTextureID());
								}
								else if (texture.lock() !=
										m_entities.at(currentPSystem - 1).lock()->getComponent<ecs::ParticleSystem>().lock()->getTexture().lock())
								{
										m_spriteBatches.emplace_back(offset, 0, texture.lock()->getTextureID());
								}

								for (int i = 0; i < particleSystem.lock()->getMaxParticles(); ++i)
								{
										if (particles[i].m_life > 0.0f)
										{
												m_spriteBatches.back().m_numIndices += 1;
												glm::vec3 position = transform.lock()->worldPosition() + particles[i].m_position;
												positions.push_back(glm::vec4(position, particles[i].m_width));
												colors.push_back(particles[i].m_color);
												offset += 1;
										}
								}
						}

						//bind the position buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::POSITION]);
						// Orphan the buffer (for speed)
						glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
						//upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec4), positions.data());
						// Unbind the VBO
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						//bind the color buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::COLOR]);
						// Orphan the buffer (for speed)
						glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Color), nullptr, GL_DYNAMIC_DRAW);
						//upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(Color), colors.data());
						// Unbind the VBO
						glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				void ParticleRenderer::sortSprites()
				{
						std::weak_ptr<ecs::Camera> currentCam = ecs::Camera::getCurrent();
						const glm::vec3& cameraPos = currentCam.lock()->getEntity().lock()->getComponent<ecs::Transform>().lock()->worldPosition();

						switch (m_sortType)
						{
						case SpriteSortType::FRONT_TO_BACK:
						{
								//use the z value (which is unused in 2D) to imitate layers
								std::stable_sort(m_entities.begin(), m_entities.end(),
										[&cameraPos](std::weak_ptr<ecs::Entity> _a, std::weak_ptr<ecs::Entity> _b)
								{
										glm::vec3 entity1WorldPos = _a.lock()->getComponent<ecs::Transform>().lock()->worldPosition();
										glm::vec3 entity2WorldPos = _b.lock()->getComponent<ecs::Transform>().lock()->worldPosition();

										float distanceFromCamera1 = glm::length2(entity1WorldPos - cameraPos);
										float distanceFromCamera2 = glm::length2(entity2WorldPos - cameraPos);

										return (distanceFromCamera1 < distanceFromCamera2);
								});
								break;
						}
						case SpriteSortType::BACK_TO_FRONT:
						{
								//use the z value (which is unused in 2D) to imitate layers
								std::stable_sort(m_entities.begin(), m_entities.end(),
										[&cameraPos](std::weak_ptr<ecs::Entity> _a, std::weak_ptr<ecs::Entity> _b)
								{
										glm::vec3 entity1WorldPos = _a.lock()->getComponent<ecs::Transform>().lock()->worldPosition();
										glm::vec3 entity2WorldPos = _b.lock()->getComponent<ecs::Transform>().lock()->worldPosition();

										float distanceFromCamera1 = glm::length2(entity1WorldPos - cameraPos);
										float distanceFromCamera2 = glm::length2(entity2WorldPos - cameraPos);

										return (distanceFromCamera1 > distanceFromCamera2);
								});
								break;
						}
						case SpriteSortType::TEXTURE:
						{
								// sort based on textures (best for optimization as all sprites with the same texture will be rendered with 1 call)
								std::stable_sort(m_entities.begin(), m_entities.end(),
										[](std::weak_ptr<ecs::Entity> _a, std::weak_ptr<ecs::Entity> _b)
								{
										return (_a.lock()->getComponent<ecs::ParticleSystem>().lock()->getTexture().lock() >
												_b.lock()->getComponent<ecs::ParticleSystem>().lock()->getTexture().lock());
								});
								break;
						}
						default:
								break;
						}
				}
		}
}