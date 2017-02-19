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
				/**
				* Mesh class, for storing mesh data and handling rendering
				*/
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

						/**
						* \brief render this mesh
						*/
						void render() const;

						/**
						* \brief dispose of the vao and vbo's
						*/
						void dispose();

						/**
						* \brief disposes of the vbo's and vao and reuploads the mesh data
						*/
						void reupload();

						/**
						* \brief Checks if the mesh is valid
						*/
						bool isValid() const;

						/**
						* \brief mesh name getter and setter
						*/
						void setName(const std::string& _name) { m_name = _name; }
						const std::string& getName()											{ return m_name; }

						/**
						* \brief Utility functions to add data to the mesh and possibly build a whole mesh
						*/

						/** \brief adds a vertex/position point to the mesh */
						void addPoint(const glm::vec3& _point);
						/** \brief adds a texcoord/uv to the mesh */
						void addTexCoord(const glm::vec2& _uv);
						/** \brief adds a normal point to the mesh */
						void addNormal(const glm::vec3& _normal);
						/** \brief adds a tangent point to the mesh */
						void addTangent(const glm::vec3& _tangent);
						/** \brief adds a face/triangle face to the mesh */
						void addFace(unsigned int _vertIndex0, unsigned int _vertIndex1, unsigned int _vertIndex2);

						inline void addVertex  (float x, float y, float z) { addPoint(glm::vec3(x, y, z)); }
						inline void addTexCoord(float u, float v)									 { addTexCoord(glm::vec2(u, v)); }
						inline void addNormal  (float x, float y, float z) { addNormal(glm::vec3(x, y, z)); }
						inline void addTangent (float x, float y, float z) { addTangent(glm::vec3(x, y, z)); }

						/** \brief mesh getters */
						inline const std::vector<unsigned int>& getIndices()  const noexcept { return m_indices; }
						inline const std::vector<glm::vec3>& getPositions()   const noexcept { return m_positions; }
						inline const std::vector<glm::vec2>& getTexCoords()   const noexcept { return m_uvs; }
						inline const std::vector<glm::vec3>& getNormals()     const noexcept { return m_normals; }
						inline const std::vector<glm::vec3>& getTangents()    const noexcept { return m_tangents; }

						/** \brief mesh setters */
						inline void setIndices(std::vector<unsigned int>& _indices)  { m_indices = _indices; }
						inline void setPositions(std::vector<glm::vec3>& _positions) { m_positions = _positions; }
						inline void	setTexCoords(std::vector<glm::vec2>& _texCoords) { m_uvs = _texCoords; }
						inline void	setNormals(std::vector<glm::vec3>& _normals)				 { m_normals = _normals; }
						inline void	setTangents(std::vector<glm::vec3>& _tangents)		 { m_tangents = _tangents; }


				private:
						//enum for all the buffer objects
						enum BufferObject
						{
								POSITION,
								TEXCOORD,
								NORMAL,
								TANGENT,

								INDEX,

								NUM_BUFFERS
						};

						/* internal utility functions */
						void calcNormals();
						void calcTangents();
						void finalize();
						void createBuffers();

				private:
						std::string m_name; ///< the name of the mesh 

						std::vector<glm::vec3> m_positions; ///< position data
						std::vector<glm::vec2> m_uvs; ///< texcoordinates
						std::vector<glm::vec3> m_normals; ///< normal data
						std::vector<glm::vec3> m_tangents; ///< tangent data
						std::vector<unsigned int> m_indices; ///< indices

						unsigned int m_VAO{ 0 }; ///< the vao of this mesh
						unsigned int m_VBOs[BufferObject::NUM_BUFFERS] = {0}; ///< the vbo's of this mesh
				};
		}
}
#endif // !MESH_H
