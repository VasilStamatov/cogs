#include "BillboardRenderer.h"

#include "Camera.h"
#include "SpriteRenderer.h"
#include "GLSLProgram.h"

#include <GL\glew.h>

namespace cogs
{
		namespace graphics
		{
				BillboardRenderer::BillboardRenderer(std::weak_ptr<GLSLProgram> _shader) : Renderer(_shader)
				{
						init();
				}


				BillboardRenderer::~BillboardRenderer()
				{
						dispose();
				}

				void BillboardRenderer::init()
				{
						//generate the vertex array buffer
						glGenVertexArrays(1, &m_VAO);

						//generate the vertex buffer objects
						glGenBuffers(1, &m_VBO);

						//bind the vao and continue working on the vbos under it
						glBindVertexArray(m_VAO);

						//bind the position buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

						//tell opengl how to read it

						glEnableVertexAttribArray(0); // on channel 0

						glVertexAttribPointer(0, // index/channel 0
								3,								// size = 3 (vec3 = 3 floats)
								GL_FLOAT, // GL_FLOAT = tell it that the numbers will be floats
								GL_FALSE, // don't normalize them
								0,								// 0 stride as it's tightly packed data (1 buffer per data rather than 1 buffer with a vertex struct)
								nullptr); // no offset as it's tightly packed

						// unbind the vao after the setup is done
						glBindVertexArray(0);
				}
				void BillboardRenderer::begin()
				{
						m_spriteBatches.clear();
						m_entities.clear();
				}
				void BillboardRenderer::submit(std::weak_ptr<ecs::Entity> _entity)
				{
						m_entities.push_back(_entity);
				}
				void BillboardRenderer::end()
				{
						//after everything has been submitted, sort it and create the batches
						sortSprites();
						createSpriteBatches();
				}
				void BillboardRenderer::flush()
				{
						std::weak_ptr<ecs::Camera> currentCam = ecs::Camera::getCurrent();

						m_shader.lock()->use();
						m_shader.lock()->uploadValue("projection", currentCam.lock()->getProjectionMatrix());
						m_shader.lock()->uploadValue("view", currentCam.lock()->getViewMatrix());
						//m_shader.lock()->uploadValue("cameraPos", currentCam.lock()->getEntity().lock()->getComponent<ecs::Transform>().lock()->worldPosition());

						/* Bind the VAO. This sets up the opengl state we need, including the
						vertex attribute pointers and it binds the VBO */
						glBindVertexArray(m_VAO);

						for (size_t i = 0; i < m_spriteBatches.size(); i++)
						{
								glBindTexture(GL_TEXTURE_2D, m_spriteBatches[i].m_texture);

								//first index is offset, numindices = num vertices (just reuising the spritebatch struct)
								glDrawArrays(GL_POINTS, m_spriteBatches[i].m_firstIndex, m_spriteBatches[i].m_numIndices);
						}
						//unbind the vao
						glBindVertexArray(0);

						m_shader.lock()->unUse();
				}
				void BillboardRenderer::dispose()
				{
						if (m_VAO != 0)
						{
								glDeleteVertexArrays(1, &m_VAO);
								m_VAO = 0;
						}

						if (m_VBO != 0)
						{
								glDeleteBuffers(1, &m_VBO);
						}
				}
				void BillboardRenderer::sortSprites()
				{
						switch (m_sortType)
						{
						case SpriteSortType::FRONT_TO_BACK:
						{
								//use the z value (which is unused in 2D) to imitate layers
								std::stable_sort(m_entities.begin(), m_entities.end(),
										[](std::weak_ptr<ecs::Entity> _a, std::weak_ptr<ecs::Entity> _b)
								{
										return (_a.lock()->getComponent<ecs::Transform>().lock()->worldPosition().z <
												_b.lock()->getComponent<ecs::Transform>().lock()->worldPosition().z);
								});
								break;
						}
						case SpriteSortType::BACK_TO_FRONT:
						{
								//use the z value (which is unused in 2D) to imitate layers
								std::stable_sort(m_entities.begin(), m_entities.end(),
										[](std::weak_ptr<ecs::Entity> _a, std::weak_ptr<ecs::Entity> _b)
								{
										return (_a.lock()->getComponent<ecs::Transform>().lock()->worldPosition().z >
												_b.lock()->getComponent<ecs::Transform>().lock()->worldPosition().z);
								});
								break;
						}
						case SpriteSortType::TEXTURE:
						{
								// sort based on textures (best for optimization as all sprites with the same texture will be rendered with 1 call)
								std::stable_sort(m_entities.begin(), m_entities.end(),
										[](std::weak_ptr<ecs::Entity> _a, std::weak_ptr<ecs::Entity> _b)
								{
										return (_a.lock()->getComponent<ecs::SpriteRenderer>().lock()->getSprite().lock()->getTexture().lock() >
												_b.lock()->getComponent<ecs::SpriteRenderer>().lock()->getSprite().lock()->getTexture().lock());
								});
								break;
						}
						default:
								break;
						}
				}
				void BillboardRenderer::createSpriteBatches()
				{
						if (m_entities.empty())
						{
								return;
						}

						// This will store all the vertices that we need to upload
						std::vector<glm::vec3> positions;

						// 1 entity = 1 world space position (center) to send to the shader
						positions.resize(m_entities.size());

						int offset = 0;

						int currentVertex = 0;

						//set all the vertices
						for (size_t currentSprite = 0; currentSprite < m_entities.size(); currentSprite++)
						{
								std::weak_ptr<Sprite> sprite = m_entities.at(currentSprite).lock()->getComponent<ecs::SpriteRenderer>().lock()->getSprite();

								//The transform values of the sprite
								std::weak_ptr<ecs::Transform> transform = m_entities.at(currentSprite).lock()->getComponent<ecs::Transform>();

								if (currentSprite == 0)
								{
										m_spriteBatches.emplace_back(offset, 1, sprite.lock()->getTexture().lock()->getTextureID());
								}
								else if (sprite.lock()->getTexture().lock() !=
										m_entities.at(currentSprite - 1).lock()->getComponent<ecs::SpriteRenderer>().lock()->getSprite().lock()->getTexture().lock())
								{
										m_spriteBatches.emplace_back(offset, 1, sprite.lock()->getTexture().lock()->getTextureID());
								}
								else
								{
										// If its part of the current batch, just increase numVertices
										m_spriteBatches.back().m_numIndices += 1;
								}

								positions.at(currentVertex++) = transform.lock()->worldPosition();

								offset += 1;
						}

						//bind the position buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
						// Orphan the buffer (for speed)
						glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions.at(0)), nullptr, GL_DYNAMIC_DRAW);
						//upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(positions.at(0)), positions.data());
						// Unbind the VBO
						glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
		}
}