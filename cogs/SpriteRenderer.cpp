#include "SpriteRenderer.h"
#include "Sprite.h"

namespace cogs
{
		namespace graphics
		{
				SpriteRenderer::SpriteRenderer() : Renderer()
				{
						init();
				}

				SpriteRenderer::~SpriteRenderer()
				{
						dispose();
						m_buffer = nullptr;
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

						//Allocate the memory space needed for the buffer
						glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);

						//enable the position attribute in the shader (index 0)
						glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
						glVertexAttribPointer(POSITION_ATTRIBUTE_INDEX, // the index of the attribute in the shader
								3,								// the size of the attribute (3 as for 3 floats in vec3's)
								GL_FLOAT, // the type of the attribute (vec3 = 3 floats => float type)
								GL_FALSE, // flag whether it should be normalized
								sizeof(SpriteVertex), // the stride/size of the vertex, which lets opengl know the "position" in the buffer of the next vertex
								(const GLvoid*)offsetof(SpriteVertex, SpriteVertex::position)); // the offset of bytes in the current vertex to get the desired value

																																																																								//enable the color attribute in the shader (index 1)
						glEnableVertexAttribArray(COLOR_ATTRIBUTE_INDEX);
						glVertexAttribPointer(COLOR_ATTRIBUTE_INDEX,
								4,
								GL_FLOAT,
								GL_FALSE,
								sizeof(SpriteVertex),
								(const GLvoid*)offsetof(SpriteVertex, SpriteVertex::color));

						//bind the index buffer object
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

						//Array to store all the indices' values
						GLushort indices[INDICES_SIZE];

						//Add all the indices' values
						int offset = 0;
						for (size_t i = 0; i < INDICES_SIZE; i += 6)
						{
								indices[i] = offset + 0;
								indices[i + 1] = offset + 1;
								indices[i + 2] = offset + 2;

								indices[i + 3] = offset + 2;
								indices[i + 4] = offset + 3;
								indices[i + 5] = offset + 0;

								offset += 4;
						}

						//Allocate and fill the index buffer with the created indices
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES_SIZE * sizeof(GLushort), indices, GL_STATIC_DRAW);

						//unbind the vao
						glBindVertexArray(0);
				}

				void SpriteRenderer::submit(ecs::Entity* _entity)
				{
						//The sprite to submit from the component (only has color for now)
						ecs::Sprite*				sprite				= _entity->getComponent<ecs::Sprite>();

						//The transform values of the sprite
						ecs::Transform* transform = _entity->getComponent<ecs::Transform>();

						//the components needed for the 4 vertices
						const glm::vec3& position = transform->worldPosition();
						const glm::quat& rotation = transform->worldOrientation();
						const glm::vec2& scale		  = transform->worldScale();
						const glm::vec4& color				= sprite->getColor();

						glm::vec3 halfDims = glm::vec3(scale.x / 2.0f, scale.y / 2.0f, 0.0f);
						// Get points centered at origin
						glm::vec3 tL(-halfDims.x, halfDims.y, 0.0f);
						glm::vec3 bL(-halfDims.x, -halfDims.y, 0.0f);
						glm::vec3 bR(halfDims.x, -halfDims.y, 0.0f);
						glm::vec3 tR(halfDims.x, halfDims.y, 0.0f);

						// Rotate the points
						tL = (rotation * tL) + halfDims;
						bL = (rotation * bL) + halfDims;
						bR = (rotation * bR) + halfDims;
						tR = (rotation * tR) + halfDims;

						/* submit the sprite by passing the 4 new vertices to the mapped buffer
						the vec3 position of the sprite is its bottom left, so the other 3 must be calculated */

						//bottom left
						m_buffer->position = glm::vec3(position.x + bL.x, position.y + bL.y, position.z);
						m_buffer->color = color;
						m_buffer++;

						//top left
						m_buffer->position = glm::vec3(position.x + tL.x, position.y + tL.y, position.z);
						m_buffer->color = color;
						m_buffer++;

						//top right
						m_buffer->position = glm::vec3(position.x + tR.x, position.y + tR.y, position.z);
						m_buffer->color = color;
						m_buffer++;

						//bottom right
						m_buffer->position = glm::vec3(position.x + bR.x, position.y + bR.y, position.z);
						m_buffer->color = color;
						m_buffer++;

						//increment the indices count by 6 (3 per triangle)
						m_indicesCount += 6;
				}

				void SpriteRenderer::flush()
				{
						// Render all the submitted data

						// Bind the vao to set up opengl in the state it's needed
						glBindVertexArray(m_vao);

						// Draw all the triangles with the indices data
						glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_SHORT, nullptr);

						// Unbind the vao
						glBindVertexArray(0);
				}

				void SpriteRenderer::begin()
				{
						// bind the vbo (which will contain all the vertex data)
						glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

						//map the sprite vertex data from the opengl buffer
						m_buffer = (SpriteVertex*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
				}

				void SpriteRenderer::end()
				{
						//unmap the buffer
						glUnmapBuffer(GL_ARRAY_BUFFER);

						//and unbind the vbo that's been written to
						glBindBuffer(GL_ARRAY_BUFFER, 0);
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
		}
}