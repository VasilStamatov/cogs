#include "SpriteRenderer.h"
#include "Sprite.h"

namespace cogs
{
		namespace graphics
		{
				SpriteRenderer::SpriteRenderer() : Renderer()
				{
						init();
						m_shader.compileShaders("BasicShader", "Shaders/BasicShader.vert", "Shaders/BasicShader.frag");
				}

				SpriteRenderer::SpriteRenderer(const std::string & _name, const std::string & _vsFilePath, const std::string & _fsFilePath, const std::string & _gsFilePath)
				{
						init();
						m_shader.compileShaders(_name, _vsFilePath, _fsFilePath, _gsFilePath);
				}

				SpriteRenderer::~SpriteRenderer()
				{
						dispose();
						m_shader.dispose();
				}

				void SpriteRenderer::init()
				{
						// Generate the buffers
						glGenVertexArrays(1, &m_vao);
						glGenBuffers(1, &m_vbo);
						glGenBuffers(1, &m_ibo);

						//bind the vertex array object
						glBindVertexArray(m_vao);

						//bind the vertex buffer object
						glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

						//enable the position attribute in the shader (index 0)
						glEnableVertexAttribArray(SPITE_POSITION_ATTRIBUTE_INDEX);
						glVertexAttribPointer(SPITE_POSITION_ATTRIBUTE_INDEX, // the index of the attribute in the shader
								3,								// the size of the attribute (3 as for 3 floats in vec3's)
								GL_FLOAT, // the type of the attribute (vec3 = 3 floats => float type)
								GL_FALSE, // flag whether it should be normalized
								sizeof(SpriteVertex), // the stride/size of the vertex, which lets opengl know the "position" in the buffer of the next vertex
								(const GLvoid*)offsetof(SpriteVertex, SpriteVertex::position)); // the offset of bytes in the current vertex to get the desired value

						// enable the uv attribute in the shader (index 1)
						glEnableVertexAttribArray(SPITE_UV_ATTRIBUTE_INDEX);
						glVertexAttribPointer(SPITE_UV_ATTRIBUTE_INDEX, 2, GL_FLOAT, GL_FALSE,	sizeof(SpriteVertex),
								(const GLvoid*)offsetof(SpriteVertex, SpriteVertex::uv));

						//enable the color attribute in the shader (index 2)
						glEnableVertexAttribArray(SPITE_COLOR_ATTRIBUTE_INDEX);
						glVertexAttribPointer(SPITE_COLOR_ATTRIBUTE_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SpriteVertex),
								(const GLvoid*)offsetof(SpriteVertex, SpriteVertex::color));

						//bind the index buffer object
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

						//unbind the vao
						glBindVertexArray(0);
				}

				void SpriteRenderer::submit(ecs::Entity* _entity)
				{
						m_entities.push_back(_entity);
				}

				void SpriteRenderer::flush(const glm::mat4& _view, const glm::mat4& _projection)
				{
						m_shader.use();
						m_shader.uploadValue("projection", _projection);
						m_shader.uploadValue("view", _view);
						/* Bind the VAO. This sets up the opengl state we need, including the
						vertex attribute pointers and it binds the VBO */
						glBindVertexArray(m_vao);

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

						m_shader.unUse();
				}

				void SpriteRenderer::begin(const SpriteSortType& _sortType /* = SpriteSortType::TEXTURE */)
				{
						m_sortType = _sortType;
						m_spriteBatches.clear();
						m_entities.clear();
				}

				void SpriteRenderer::end()
				{
						SortSprites();
						CreateSpriteBatches();
				}

				void SpriteRenderer::dispose()
				{
						//Dispose of all the buffest if they have't been disposed already

						if (m_vao != 0)
						{
								glDeleteVertexArrays(1, &m_vao);
								m_vao = 0;
						}

						if (m_vbo != 0)
						{
								glDeleteBuffers(1, &m_vbo);
								m_vbo = 0;
						}

						if (m_ibo != 0)
						{
								glDeleteBuffers(1, &m_ibo);
								m_ibo = 0;
						}
				}

				void SpriteRenderer::setShader(const std::string & _name, const std::string & _vsFilePath, const std::string & _fsFilePath, const std::string & _gsFilePath)
				{
						m_shader.compileShaders(_name, _vsFilePath, _fsFilePath, _gsFilePath);
				}

				void SpriteRenderer::SortSprites()
				{
						switch (m_sortType)
						{
						case SpriteSortType::FRONT_TO_BACK:
						{
								std::stable_sort(m_entities.begin(), m_entities.end(),
										[](ecs::Entity* _a, ecs::Entity* _b)
								{
										return (_a->getComponent<ecs::Transform>()->worldPosition().z < _b->getComponent<ecs::Transform>()->worldPosition().z);
								});
								break;
						}
						case SpriteSortType::BACK_TO_FRONT:
						{
								std::stable_sort(m_entities.begin(), m_entities.end(),
										[](ecs::Entity* _a, ecs::Entity* _b)
								{
										return (_a->getComponent<ecs::Transform>()->worldPosition().z > _b->getComponent<ecs::Transform>()->worldPosition().z);
								});
								break;
						}
						case SpriteSortType::TEXTURE:
						{
								std::stable_sort(m_entities.begin(), m_entities.end(),
										[](ecs::Entity* _a, ecs::Entity* _b)
								{
										return (_a->getComponent<ecs::Sprite>()->getTexture() > _b->getComponent<ecs::Sprite>()->getTexture());
								});
								break;
						}
						default:
								break;
						}
				}

				void SpriteRenderer::CreateSpriteBatches()
				{
						// This will store all the vertices that we need to upload
						std::vector<SpriteVertex> vertices;
						std::vector<GLuint> indices;

						//1 entity = 1 sprite = 4 vertices
						vertices.resize(m_entities.size() * 4);
						//1 sprite = 6 indices to connect the 4 verts (0, 1, 2 - 2, 3, 0)
						indices.resize(m_entities.size() * 6);

						if (m_entities.empty())
						{
								return;
						}

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
								ecs::Sprite*				sprite = m_entities.at(currentSprite)->getComponent<ecs::Sprite>();

								//The transform values of the sprite
								ecs::Transform* transform = m_entities.at(currentSprite)->getComponent<ecs::Transform>();

								if (currentSprite == 0)
								{
										m_spriteBatches.emplace_back(offset, 6, sprite->getTexture().getID());
								}
								else if (sprite->getTexture() !=
										m_entities.at(currentSprite - 1)->getComponent<ecs::Sprite>()->getTexture())
								{
										m_spriteBatches.emplace_back(offset, 6, sprite->getTexture().getID());
								}
								else
								{
										// If its part of the current batch, just increase numVertices
										m_spriteBatches.back().m_numIndices += 6;
								}

								//the components needed for the 4 vertices
								const graphics::Color& color = sprite->getColor();
								const glm::vec2& size = sprite->getSize();
								const std::array<glm::vec2, 4>& uv = sprite->getUV();

								glm::mat4 worldTrans = transform->worldTransform();

								//get the 4 vertices around the center 
								glm::vec3 bottomLeft	 = glm::vec3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
								glm::vec3 topLeft				 = glm::vec3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
								glm::vec3 topRight				= glm::vec3(size.x * 0.5f, size.y * 0.5f, 0.0f);
								glm::vec3 bottomRight = glm::vec3(size.x * 0.5f, -size.y * 0.5f, 0.0f);

								//transform the 4 vertices by the world matrix
								bottomLeft		= worldTrans * glm::vec4(bottomLeft, 1.0f);
								topLeft					= worldTrans * glm::vec4(topLeft, 1.0f);
								topRight				= worldTrans * glm::vec4(topRight, 1.0f);
								bottomRight = worldTrans * glm::vec4(bottomRight, 1.0f);

								vertices.at(currentVertex).position = bottomLeft;
								vertices.at(currentVertex).uv = uv.at(0);
								vertices.at(currentVertex++).color = color;

								vertices.at(currentVertex).position = topLeft;
								vertices.at(currentVertex).uv = uv.at(1);
								vertices.at(currentVertex++).color = color;

								vertices.at(currentVertex).position = topRight;
								vertices.at(currentVertex).uv = uv.at(2);
								vertices.at(currentVertex++).color = color;

								vertices.at(currentVertex).position = bottomRight;
								vertices.at(currentVertex).uv = uv.at(3);
								vertices.at(currentVertex++).color = color;

								offset += 6;
						}
						// Bind our VBO
						glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
						// Orphan the buffer (for speed)
						glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SpriteVertex), nullptr, GL_DYNAMIC_DRAW);
						//upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(SpriteVertex), vertices.data());
						// Unbind the VBO
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
						// Orphan the buffer
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
						// Upload the data
						glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());
						//unbind the ibo
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
		}
}