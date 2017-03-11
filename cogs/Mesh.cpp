#include "Mesh.h"

#include "Utils.h"

#include <glm\glm.hpp>
#include <GL\glew.h>

namespace cogs
{
		namespace graphics
		{
				Mesh::Mesh(const std::string & _filePath)
				{
						load(_filePath);
				}

				Mesh::~Mesh()
				{
						dispose();
				}

				void Mesh::render() const
				{
						glBindVertexArray(m_VAO);

						glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

						glBindVertexArray(0);
				}

				void Mesh::dispose()
				{
						if (m_VAO != 0)
						{
								glDeleteVertexArrays(1, &m_VAO);
								m_VAO = 0;
						}

						if (m_VBOs[0] != 0)
						{
								glDeleteBuffers(NUM_BUFFERS, m_VBOs);
								for (size_t i = 0; i < 5; i++)
								{
										m_VBOs[i] = 0;
								}
						}
				}

				bool Mesh::isValid() const
				{
						return m_positions.size() == m_uvs.size()
								&& m_uvs.size() == m_normals.size()
								&& m_normals.size() == m_tangents.size();
				}

				void Mesh::load(const std::string & _filePath)
				{
						utils::loadMesh(_filePath, m_subMeshes, m_positions, m_uvs, m_normals, m_tangents, m_indices, m_materials);

						createBuffers();
				}

				void Mesh::calcBoundingSphere()
				{
						float maxX{ 0.0f }, maxY{ 0.0f }, maxZ{ 0.0f };
						float minX{ 0.0f }, minY{ 0.0f }, minZ{ 0.0f };

						for (size_t i = 0; i < m_positions.size(); i++)
						{
								if (m_positions.at(i).x > maxX)
								{
										maxX = m_positions.at(i).x;
								}
								if (m_positions.at(i).x < minX)
								{
										minX = m_positions.at(i).x;
								}

								if (m_positions.at(i).y > maxY)
								{
										maxY = m_positions.at(i).y;
								}
								if (m_positions.at(i).y < minY)
								{
										minY = m_positions.at(i).y;
								}

								if (m_positions.at(i).z > maxZ)
								{
										maxZ = m_positions.at(i).z;
								}
								if (m_positions.at(i).z < minZ)
								{
										minZ = m_positions.at(i).z;
								}
						}

						m_center = glm::vec3(
								minX + (maxX - minX) * 0.5f,
								minY + (maxY - minY) * 0.5f,
								minZ + (maxZ - minZ) * 0.5f);
						m_radius = fmaxf((maxX - minX) * 0.5f, fmaxf((maxY - minY) * 0.5f, (maxZ - minZ) * 0.5f));
				}

				void Mesh::calcNormals()
				{
						m_normals.clear();
						m_normals.reserve(m_positions.size());

						for (unsigned int i = 0; i < m_positions.size(); i++)
						{
								m_normals.push_back(glm::vec3(0, 0, 0));
						}

						for (unsigned int i = 0; i < m_indices.size(); i += 3)
						{
								int A = m_indices.at(i);
								int B = m_indices.at(i + 1);
								int C = m_indices.at(i + 2);

								glm::vec3 v1 = m_positions.at(B) - m_positions.at(A);
								glm::vec3 v2 = m_positions.at(C) - m_positions.at(A);

								glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

								m_normals.at(A) += normal;
								m_normals.at(B) += normal;
								m_normals.at(C) += normal;
						}

						for (unsigned int i = 0; i < m_normals.size(); i++)
						{
								m_normals.at(i) = glm::normalize(m_normals.at(i));
						}
				}

				void Mesh::calcTangents()
				{
						m_tangents.clear();
						m_tangents.reserve(m_positions.size());

						for (unsigned int i = 0; i < m_positions.size(); i++)
						{
								m_tangents.push_back(glm::vec3(0, 0, 0));
						}

						for (unsigned int i = 0; i < m_indices.size(); i += 3)
						{
								int A = m_indices.at(i);
								int B = m_indices.at(i + 1);
								int C = m_indices.at(i + 2);

								glm::vec3 edge1 = m_positions.at(B) - m_positions.at(A);
								glm::vec3 edge2 = m_positions.at(C) - m_positions.at(A);

								float deltaU1 = m_uvs.at(B).x - m_uvs.at(A).x;
								float deltaU2 = m_uvs.at(C).x - m_uvs.at(A).x;
								float deltaV1 = m_uvs.at(B).y - m_uvs.at(A).y;
								float deltaV2 = m_uvs.at(C).y - m_uvs.at(A).y;

								float dividend = (deltaU1 * deltaV2 - deltaU2 * deltaV1);
								float f = dividend == 0.0f ? 0.0f : 1.0f / dividend;

								glm::vec3 tangent(0, 0, 0);

								tangent.x = (f * (deltaV2 * edge1.x - deltaV1 * edge2.x));
								tangent.y = (f * (deltaV2 * edge1.y - deltaV1 * edge2.y));
								tangent.z = (f * (deltaV2 * edge1.z - deltaV1 * edge2.z));

								m_tangents.at(A) += tangent;
								m_tangents.at(B) += tangent;
								m_tangents.at(C) += tangent;
						}

						for (unsigned int i = 0; i < m_tangents.size(); i++)
						{
								m_tangents.at(i) = glm::normalize(m_tangents.at(i));
						}
				}

				void Mesh::finalize()
				{
						calcBoundingSphere();

						if (isValid())
						{
								//already valid
								return;
						}

						if (m_uvs.size() == 0)
						{
								for (unsigned int i = m_uvs.size(); i < m_positions.size(); i++)
								{
										m_uvs.push_back(glm::vec2(0.0f, 0.0f));
								}
						}

						if (m_normals.size() == 0)
						{
								calcNormals();
						}

						if (m_tangents.size() == 0)
						{
								calcTangents();
						}

						if (!isValid())
						{
								printf("Mesh cannot be set up properly");
								assert(false);
						}
				}

				void Mesh::createBuffers()
				{
						finalize();

						glGenVertexArrays(1, &m_VAO);
						glBindVertexArray(m_VAO);

						glGenBuffers(BufferObject::NUM_BUFFERS, m_VBOs);

						// Upload position data
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObject::POSITION]);
						glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(m_positions.at(0)), m_positions.data(), GL_STATIC_DRAW);

						glEnableVertexAttribArray(BufferObject::POSITION);
						glVertexAttribPointer(BufferObject::POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

						// Upload UV data
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObject::TEXCOORD]);
						glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(m_uvs.at(0)), m_uvs.data(), GL_STATIC_DRAW);

						glEnableVertexAttribArray(BufferObject::TEXCOORD);
						glVertexAttribPointer(BufferObject::TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

						// Upload normals data
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObject::NORMAL]);
						glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(m_normals.at(0)), m_normals.data(), GL_STATIC_DRAW);

						glEnableVertexAttribArray(BufferObject::NORMAL);
						glVertexAttribPointer(BufferObject::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

						// Upload tangents data
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObject::TANGENT]);
						glBufferData(GL_ARRAY_BUFFER, m_tangents.size() * sizeof(m_tangents.at(0)), m_tangents.data(), GL_STATIC_DRAW);

						glEnableVertexAttribArray(BufferObject::TANGENT);
						glVertexAttribPointer(BufferObject::TANGENT, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

						// Upload index data for indexed rendering
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[BufferObject::INDEX]);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(m_indices.at(0)), m_indices.data(), GL_STATIC_DRAW);

						// bind the buffer for world matrices
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObject::WORLDMAT]);
						// cannot upload mat4's all at once, so upload them as 4 vec4's
						for (size_t i = 0; i < 4; i++)
						{
								//enable the channel of the current matrix row (4,5,6,7)
								glEnableVertexAttribArray(BufferObject::WORLDMAT + i);
								//tell opengl how to read it
								glVertexAttribPointer(BufferObject::WORLDMAT + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
										(const void*)(sizeof(float) * i * 4));

								/** This function is what makes it per-instance data rather than per vertex
								* The first parameter is the attribute channel as above (4,5,6,7)
								* The second parameter tells the rate at which the attribute advanced during the rendering
								* 1 means that this data is updated after 1 instance has been rendered
								* by default it's 0 which makes it per-vertex and if it's over 1 than more than 1 instances will use this data
								*/
								glVertexAttribDivisor(BufferObject::WORLDMAT + i, 1);
						}
					
						glBindVertexArray(0);
				}
		}
}