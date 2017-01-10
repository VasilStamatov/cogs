#ifndef MESH_H
#define MESH_H

#include "GLTexture.h"

#include <vector>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

namespace cogs
{
		namespace graphics
		{
				constexpr unsigned int MESH_POSITION_ATTRIBUTE_INDEX = 0;
				constexpr unsigned int MESH_UV_ATTRIBUTE_INDEX					  = 1;
				constexpr unsigned int MESH_NORMAL_ATTRIBUTE_INDEX			= 2;
				constexpr unsigned int MESH_TANGENT_ATTRIBUTE_INDEX  = 3;

				class Mesh
				{
				public:
						Mesh();
						Mesh(const std::vector<unsigned int>& _indices,
								const std::vector<glm::vec3>& _positions,
								const std::vector<glm::vec2>& _uvs,
								const std::vector<glm::vec3>& _normals,
								const std::vector<glm::vec3>& _tangents,
								const std::vector<GLTexture>& _textures);
						~Mesh();

						void render();

						void dispose();

						void reupload();

						bool isValid() const;

						void addPoint(const glm::vec3& _point);
						void addTexCoord(const glm::vec2& _uv);
						void addNormal(const glm::vec3& _normal);
						void addTangent(const glm::vec3& _tangent);
						void addFace(unsigned int _vertIndex0, unsigned int _vertIndex1, unsigned int _vertIndex2);
						void addTexture(const GLTexture& _texture);

						inline void addVertex  (float x, float y, float z) { addPoint(glm::vec3(x, y, z)); }
						inline void addTexCoord(float u, float v)									 { addTexCoord(glm::vec2(u, v)); }
						inline void addNormal  (float x, float y, float z) { addNormal(glm::vec3(x, y, z)); }
						inline void addTangent (float x, float y, float z) { addTangent(glm::vec3(x, y, z)); }

						inline const std::vector<unsigned int>& getIndices()  const noexcept { return m_indices; }
						inline const std::vector<glm::vec3>& getPositions()   const noexcept { return m_positions; }
						inline const std::vector<glm::vec2>& getTexCoords()   const noexcept { return m_uvs; }
						inline const std::vector<glm::vec3>& getNormals()     const noexcept { return m_normals; }
						inline const std::vector<glm::vec3>& getTangents()    const noexcept { return m_tangents; }
						inline const std::vector<GLTexture>& getTextures()    const noexcept { return m_textures; }

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
						std::vector<glm::vec3> m_positions;
						std::vector<glm::vec2> m_uvs;
						std::vector<glm::vec3> m_normals;
						std::vector<glm::vec3> m_tangents;
						std::vector<unsigned int> m_indices;
						std::vector<GLTexture> m_textures;

						unsigned int m_VAO{ 0 };
						unsigned int m_VBOs[BufferObject::NUM_BUFFERS] = {0};
				};
		}
}
#endif // !MESH_H
