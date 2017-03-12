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

						glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);

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

				bool Mesh::isValid(const std::vector<glm::vec3>& _positions,
						const std::vector<glm::vec2>& _uvs,
						const std::vector<glm::vec3>& _normals,
						const std::vector<glm::vec3>& _tangents) const
				{
						return _positions.size() == _uvs.size()
								&& _uvs.size() == _normals.size()
								&& _normals.size() == _tangents.size();
				}

				void Mesh::load(const std::string & _filePath)
				{
						std::vector<glm::vec3> positions;
						std::vector<glm::vec2> uvs;
						std::vector<glm::vec3> normals;
						std::vector<glm::vec3> tangents;
						std::vector<unsigned int> indices;
						utils::loadMesh(_filePath, m_subMeshes, positions, uvs, normals, tangents, indices, m_materials);

						createBuffers(positions, uvs, normals, tangents, indices);
				}

				void Mesh::calcBounds(const std::vector<glm::vec3>& _positions)
				{
						float maxX{ 0.0f }, maxY{ 0.0f }, maxZ{ 0.0f };
						float minX{ 0.0f }, minY{ 0.0f }, minZ{ 0.0f };

						for (size_t i = 0; i < _positions.size(); i++)
						{
								if (_positions.at(i).x > maxX)
								{
										maxX = _positions.at(i).x;
								}
								if (_positions.at(i).x < minX)
								{
										minX = _positions.at(i).x;
								}

								if (_positions.at(i).y > maxY)
								{
										maxY = _positions.at(i).y;
								}
								if (_positions.at(i).y < minY)
								{
										minY = _positions.at(i).y;
								}

								if (_positions.at(i).z > maxZ)
								{
										maxZ = _positions.at(i).z;
								}
								if (_positions.at(i).z < minZ)
								{
										minZ = _positions.at(i).z;
								}
						}

						m_boundingBox.m_min = glm::vec3(minX, minY, minZ);
						m_boundingBox.m_max = glm::vec3(maxX, maxY, maxZ);

						m_boundingSphere.m_center = glm::vec3(
								minX + (maxX - minX) * 0.5f,
								minY + (maxY - minY) * 0.5f,
								minZ + (maxZ - minZ) * 0.5f);
						m_boundingSphere.m_radius = fmaxf((maxX - minX) * 0.5f, fmaxf((maxY - minY) * 0.5f, (maxZ - minZ) * 0.5f));
				}

				void Mesh::calcNormals(const std::vector<glm::vec3>& _positions,
						std::vector<glm::vec3>& _normals,
						std::vector<unsigned int>& _indices)
				{
						_normals.clear();
						_normals.reserve(_positions.size());

						for (unsigned int i = 0; i < _positions.size(); i++)
						{
								_normals.push_back(glm::vec3(0, 0, 0));
						}

						for (unsigned int i = 0; i < _indices.size(); i += 3)
						{
								int A = _indices.at(i);
								int B = _indices.at(i + 1);
								int C = _indices.at(i + 2);

								glm::vec3 v1 = _positions.at(B) - _positions.at(A);
								glm::vec3 v2 = _positions.at(C) - _positions.at(A);

								glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

								_normals.at(A) += normal;
								_normals.at(B) += normal;
								_normals.at(C) += normal;
						}

						for (unsigned int i = 0; i < _normals.size(); i++)
						{
								_normals.at(i) = glm::normalize(_normals.at(i));
						}
				}

				void Mesh::calcTangents(const std::vector<glm::vec3>& _positions,
						std::vector<glm::vec2>& _uvs,
						std::vector<glm::vec3>& _normals,
						std::vector<glm::vec3>& _tangents,
						std::vector<unsigned int>& _indices)
				{
						_tangents.clear();
						_tangents.reserve(_positions.size());

						for (unsigned int i = 0; i < _positions.size(); i++)
						{
								_tangents.push_back(glm::vec3(0, 0, 0));
						}

						for (unsigned int i = 0; i < _indices.size(); i += 3)
						{
								int A = _indices.at(i);
								int B = _indices.at(i + 1);
								int C = _indices.at(i + 2);

								glm::vec3 edge1 = _positions.at(B) - _positions.at(A);
								glm::vec3 edge2 = _positions.at(C) - _positions.at(A);

								float deltaU1 = _uvs.at(B).x - _uvs.at(A).x;
								float deltaU2 = _uvs.at(C).x - _uvs.at(A).x;
								float deltaV1 = _uvs.at(B).y - _uvs.at(A).y;
								float deltaV2 = _uvs.at(C).y - _uvs.at(A).y;

								float dividend = (deltaU1 * deltaV2 - deltaU2 * deltaV1);
								float f = dividend == 0.0f ? 0.0f : 1.0f / dividend;

								glm::vec3 tangent(0, 0, 0);

								tangent.x = (f * (deltaV2 * edge1.x - deltaV1 * edge2.x));
								tangent.y = (f * (deltaV2 * edge1.y - deltaV1 * edge2.y));
								tangent.z = (f * (deltaV2 * edge1.z - deltaV1 * edge2.z));

								_tangents.at(A) += tangent;
								_tangents.at(B) += tangent;
								_tangents.at(C) += tangent;
						}

						for (unsigned int i = 0; i < _tangents.size(); i++)
						{
								_tangents.at(i) = glm::normalize(_tangents.at(i));
						}
				}

				void Mesh::finalize(const std::vector<glm::vec3>& _positions,
						 std::vector<glm::vec2>& _uvs,
						 std::vector<glm::vec3>& _normals,
						 std::vector<glm::vec3>& _tangents,
						std::vector<unsigned int>& _indices)
				{
						calcBounds(_positions);

						if (isValid(_positions, _uvs, _normals, _tangents))
						{
								//already valid
								return;
						}

						if (_uvs.size() == 0)
						{
								for (unsigned int i = _uvs.size(); i < _positions.size(); i++)
								{
										_uvs.push_back(glm::vec2(0.0f, 0.0f));
								}
						}

						if (_normals.size() == 0)
						{
								calcNormals(_positions, _normals, _indices);
						}

						if (_tangents.size() == 0)
						{
								calcTangents(_positions, _uvs, _normals, _tangents, _indices);
						}

						if (!isValid(_positions, _uvs, _normals, _tangents))
						{
								printf("Mesh cannot be set up properly");
								assert(false);
						}
				}

				void Mesh::createBuffers(const std::vector<glm::vec3>& _positions,
					std::vector<glm::vec2>& _uvs,
					std::vector<glm::vec3>& _normals,
					std::vector<glm::vec3>& _tangents,
					std::vector<unsigned int>& _indices)
				{
						finalize(_positions, _uvs, _normals, _tangents, _indices);

						m_numIndices = _indices.size();

						glGenVertexArrays(1, &m_VAO);
						glBindVertexArray(m_VAO);

						glGenBuffers(BufferObject::NUM_BUFFERS, m_VBOs);

						// Upload position data
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObject::POSITION]);
						glBufferData(GL_ARRAY_BUFFER, _positions.size() * sizeof(_positions.at(0)), _positions.data(), GL_STATIC_DRAW);

						glEnableVertexAttribArray(BufferObject::POSITION);
						glVertexAttribPointer(BufferObject::POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

						// Upload UV data
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObject::TEXCOORD]);
						glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(_uvs.at(0)), _uvs.data(), GL_STATIC_DRAW);

						glEnableVertexAttribArray(BufferObject::TEXCOORD);
						glVertexAttribPointer(BufferObject::TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

						// Upload normals data
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObject::NORMAL]);
						glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(_normals.at(0)), _normals.data(), GL_STATIC_DRAW);

						glEnableVertexAttribArray(BufferObject::NORMAL);
						glVertexAttribPointer(BufferObject::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

						// Upload tangents data
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObject::TANGENT]);
						glBufferData(GL_ARRAY_BUFFER, _tangents.size() * sizeof(_tangents.at(0)), _tangents.data(), GL_STATIC_DRAW);

						glEnableVertexAttribArray(BufferObject::TANGENT);
						glVertexAttribPointer(BufferObject::TANGENT, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

						// Upload index data for indexed rendering
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[BufferObject::INDEX]);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(_indices.at(0)), _indices.data(), GL_STATIC_DRAW);

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