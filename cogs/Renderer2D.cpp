#include "Renderer2D.h"

#include "SpriteRenderer.h"
#include "Camera.h"
#include "GLSLProgram.h"

#include <GL\glew.h>

namespace cogs
{
		namespace graphics
		{
				Renderer2D::Renderer2D(std::weak_ptr<GLSLProgram> _shader) : Renderer(_shader)
				{
						init();
				}
				Renderer2D::Renderer2D()
				{
						init();
				}
				Renderer2D::~Renderer2D()
				{
						dispose();
				}
				void Renderer2D::init()
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
								3,								// size = 3 (vec3 = 3 floats)
								GL_FLOAT, // GL_FLOAT = tell it that the numbers will be floats
								GL_FALSE, // don't normalize them
								0,								// 0 stride as it's tightly packed data (1 buffer per data rather than 1 buffer with a vertex struct)
								nullptr);  // no offset as it's tightly packed

																			// bind the texcoord/uv buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::TEXCOORD]);

						glEnableVertexAttribArray(BufferObjects::TEXCOORD); //channel 1

						glVertexAttribPointer(BufferObjects::TEXCOORD, // index 1
								2,								// size = 2 ( 2 numbers for the u and v )
								GL_FLOAT, // the 2 numbers will be floats
								GL_FALSE, // don't normalize them
								0,								// tightly packed
								nullptr); // no offset

																		// bind the color buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::COLOR]);

						glEnableVertexAttribArray(BufferObjects::COLOR); //channel 2

						glVertexAttribPointer(BufferObjects::COLOR, // index 2
								4,																// size = 4 ( 4 numbers for r,g,b and a )
								GL_UNSIGNED_BYTE, // the 4 numbers will be bytes (0-255)
								GL_TRUE,										// normalize them (divided by 255)
								0,																// tightly packed
								nullptr);									// no offset

																										// bind the index buffer for rendering with indices
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[BufferObjects::INDEX]);

						// unbind the vao after the setup is done
						glBindVertexArray(0);
				}
				void Renderer2D::submit(std::weak_ptr<ecs::Entity> _entity)
				{
						m_entities.push_back(_entity);
				}
				void Renderer2D::flush()
				{
						std::weak_ptr<ecs::Camera> currentCam = ecs::Camera::getCurrent();

						m_shader.lock()->use();
						m_shader.lock()->uploadValue("projection", currentCam.lock()->getProjectionMatrix());
						m_shader.lock()->uploadValue("view", currentCam.lock()->getViewMatrix());
						/* Bind the VAO. This sets up the opengl state we need, including the
						vertex attribute pointers and it binds the VBO */
						glBindVertexArray(m_VAO);

						for (size_t i = 0; i < m_spriteBatches.size(); i++)
						{
								glBindTexture(GL_TEXTURE_2D, m_spriteBatches.at(i).m_texture);

								glDrawElements(GL_TRIANGLES,
										m_spriteBatches.at(i).m_numIndices,
										GL_UNSIGNED_INT,
										(const GLvoid*)(m_spriteBatches.at(i).m_firstIndex * sizeof(unsigned int)));
						}
						//unbind the vao
						glBindVertexArray(0);

						m_shader.lock()->unUse();
				}
				void Renderer2D::begin()
				{
						m_spriteBatches.clear();
						m_entities.clear();
				}
				void Renderer2D::end()
				{
						//after everything has been submitted, sort it and create the batches
						sortSprites();
						createSpriteBatches();
				}
				void Renderer2D::dispose()
				{
						//Dispose of all the buffest if they have't been disposed already

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

				void Renderer2D::sortSprites()
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
				void Renderer2D::createSpriteBatches()
				{
						if (m_entities.empty())
						{
								return;
						}

						// This will store all the vertices that we need to upload
						std::vector<glm::vec3> positions;

						std::vector<glm::vec2> texCoords;

						std::vector<Color> colors;

						std::vector<GLuint> indices;

						//1 entity = 1 sprite = 4 vertices
						positions.resize(m_entities.size() * 4);
						texCoords.resize(m_entities.size() * 4);
						colors.resize(m_entities.size() * 4);

						//1 sprite = 6 indices to connect the 4 verts (0, 1, 2 - 2, 3, 0)
						indices.resize(m_entities.size() * 6);

						int offset = 0;

						//set all the indices
						for (size_t i = 0; i < indices.size(); i += 6)
						{
								indices.at(i) = offset + 0;
								indices.at(i + 1) = offset + 1;
								indices.at(i + 2) = offset + 2;

								indices.at(i + 3) = offset + 2;
								indices.at(i + 4) = offset + 3;
								indices.at(i + 5) = offset + 0;

								offset += 4;
						}

						int currentVertex = 0;
						offset = 0;

						//set all the vertices
						for (size_t currentSprite = 0; currentSprite < m_entities.size(); currentSprite++)
						{
								std::weak_ptr<Sprite> sprite = m_entities.at(currentSprite).lock()->getComponent<ecs::SpriteRenderer>().lock()->getSprite();

								//The transform values of the sprite
								std::weak_ptr<ecs::Transform> transform = m_entities.at(currentSprite).lock()->getComponent<ecs::Transform>();

								if (currentSprite == 0)
								{
										m_spriteBatches.emplace_back(offset, 6, sprite.lock()->getTexture().lock()->getTextureID());
								}
								else if (sprite.lock()->getTexture().lock() !=
										m_entities.at(currentSprite - 1).lock()->getComponent<ecs::SpriteRenderer>().lock()->getSprite().lock()->getTexture().lock())
								{
										m_spriteBatches.emplace_back(offset, 6, sprite.lock()->getTexture().lock()->getTextureID());
								}
								else
								{
										// If its part of the current batch, just increase numVertices
										m_spriteBatches.back().m_numIndices += 6;
								}

								//the components needed for the 4 vertices
								const graphics::Color& color = sprite.lock()->getColor();
								const glm::vec2& size = sprite.lock()->getSize();

								glm::mat4 worldTrans = transform.lock()->worldTransform();

								//get the 4 vertices around the center 
								glm::vec3 bottomLeft = glm::vec3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
								glm::vec3 topLeft = glm::vec3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
								glm::vec3 topRight = glm::vec3(size.x * 0.5f, size.y * 0.5f, 0.0f);
								glm::vec3 bottomRight = glm::vec3(size.x * 0.5f, -size.y * 0.5f, 0.0f);

								//transform the 4 vertices by the world matrix
								bottomLeft = worldTrans * glm::vec4(bottomLeft, 1.0f);
								topLeft = worldTrans * glm::vec4(topLeft, 1.0f);
								topRight = worldTrans * glm::vec4(topRight, 1.0f);
								bottomRight = worldTrans * glm::vec4(bottomRight, 1.0f);

								positions.at(currentVertex) = topLeft;
								texCoords.at(currentVertex) = glm::vec2(0.0f, 1.0f);
								colors.at(currentVertex++)  = color;

								positions.at(currentVertex) = bottomLeft;
								texCoords.at(currentVertex) = glm::vec2(0.0f, 0.0f);
								colors.at(currentVertex++)  = color;

								positions.at(currentVertex) = bottomRight;
								texCoords.at(currentVertex) = glm::vec2(1.0f, 0.0f);
								colors.at(currentVertex++) = color;

								positions.at(currentVertex) = topRight;
								texCoords.at(currentVertex) = glm::vec2(1.0f, 1.0f);
								colors.at(currentVertex++) = color;

								offset += 6;
						}

						//bind the position buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::POSITION]);
						// Orphan the buffer (for speed)
						glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions.at(0)), nullptr, GL_DYNAMIC_DRAW);
						//upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(positions.at(0)), positions.data());
						// Unbind the VBO
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						//bind the uv buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::TEXCOORD]);
						// Orphan the buffer (for speed)
						glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords.at(0)), nullptr, GL_DYNAMIC_DRAW);
						//upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, texCoords.size() * sizeof(texCoords.at(0)), texCoords.data());
						// Unbind the VBO
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						//bind the color buffer
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::COLOR]);
						// Orphan the buffer (for speed)
						glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors.at(0)), nullptr, GL_DYNAMIC_DRAW);
						//upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(colors.at(0)), colors.data());
						// Unbind the VBO
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						//bind the index buffer
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[BufferObjects::INDEX]);
						// Orphan the buffer
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.at(0)), nullptr, GL_DYNAMIC_DRAW);
						// Upload the data
						glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(indices.at(0)), indices.data());
						//unbind the ibo
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
		}
}
