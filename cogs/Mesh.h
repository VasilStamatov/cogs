#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

namespace cogs
{
		namespace graphics
		{
				class Mesh
				{
				public:
						Mesh() {}
						Mesh(const std::string& _name);
						Mesh(const std::string& _name,
								const std::vector<unsigned int>& _indices,
								const std::vector<glm::vec3>& _positions,
								const std::vector<glm::vec2>& _uvs,
								const std::vector<glm::vec3>& _normals,
								const std::vector<glm::vec3>& _tangents);
						Mesh(const Mesh& _other);

						~Mesh();

						void render() const;

						void dispose();

						void reupload();

						bool isValid() const;

						void setName(const std::string& _name) { m_name = _name; }
						const std::string& getName()											{ return m_name; }

						void addPoint(const glm::vec3& _point);
						void addTexCoord(const glm::vec2& _uv);
						void addNormal(const glm::vec3& _normal);
						void addTangent(const glm::vec3& _tangent);
						void addFace(unsigned int _vertIndex0, unsigned int _vertIndex1, unsigned int _vertIndex2);

						inline void addVertex  (float x, float y, float z) { addPoint(glm::vec3(x, y, z)); }
						inline void addTexCoord(float u, float v)									 { addTexCoord(glm::vec2(u, v)); }
						inline void addNormal  (float x, float y, float z) { addNormal(glm::vec3(x, y, z)); }
						inline void addTangent (float x, float y, float z) { addTangent(glm::vec3(x, y, z)); }

						inline const std::vector<unsigned int>& getIndices()  const noexcept { return m_indices; }
						inline const std::vector<glm::vec3>& getPositions()   const noexcept { return m_positions; }
						inline const std::vector<glm::vec2>& getTexCoords()   const noexcept { return m_uvs; }
						inline const std::vector<glm::vec3>& getNormals()     const noexcept { return m_normals; }
						inline const std::vector<glm::vec3>& getTangents()    const noexcept { return m_tangents; }

						inline void setIndices(std::vector<unsigned int>& _indices)  { m_indices = _indices; }
						inline void setPositions(std::vector<glm::vec3>& _positions) { m_positions = _positions; }
						inline void	setTexCoords(std::vector<glm::vec2>& _texCoords) { m_uvs = _texCoords; }
						inline void	setNormals(std::vector<glm::vec3>& _normals)				 { m_normals = _normals; }
						inline void	setTangents(std::vector<glm::vec3>& _tangents)		 { m_tangents = _tangents; }


				private:
						enum BufferObject
						{
								POSITION,
								TEXCOORD,
								NORMAL,
								TANGENT,

								INDEX,

								NUM_BUFFERS
						};

						void calcNormals();
						void calcTangents();
						void finalize();
						void createBuffers();

				private:
						std::string m_name;

						std::vector<glm::vec3> m_positions;
						std::vector<glm::vec2> m_uvs;
						std::vector<glm::vec3> m_normals;
						std::vector<glm::vec3> m_tangents;
						std::vector<unsigned int> m_indices;

						unsigned int m_VAO{ 0 };
						unsigned int m_VBOs[BufferObject::NUM_BUFFERS] = {0};
				};
		}
}
#endif // !MESH_H
