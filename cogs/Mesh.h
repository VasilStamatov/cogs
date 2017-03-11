#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

#include "Material.h"

namespace cogs
{
		namespace graphics
		{
				//Structure of submeshes the mesh is composed of
				struct SubMesh
				{
						unsigned int m_baseVertex{ 0 };
						unsigned int m_baseIndex{ 0 };
						unsigned int m_numIndices{ 0 };
						unsigned int m_materialIndex{ 9999 };
				};

				/**
				* Mesh class, for storing mesh data and handling rendering
				*/
				class Mesh
				{
						friend class Renderer3D;

				public:
						Mesh() {}
						Mesh(const std::string& _filePath);

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
						* \brief Checks if the mesh is valid
						*/
						bool isValid() const;

						/**
						* \brief loads mesh data into this object (buffer objects, vertex data etc.)
						*/
						void load(const std::string& _filePath);

						inline const glm::vec3& getCenter()																	  const noexcept { return m_center; }
						inline const float& getRadius()																							const noexcept	{ return m_radius; }
						inline const std::vector<SubMesh>& getSubMeshes()					const noexcept { return m_subMeshes; }
						inline const std::vector<std::weak_ptr<Material>>& getMaterials()	const noexcept { return m_materials; }

				private:
						/* internal utility functions */
						void calcBoundingSphere();
						void calcNormals();
						void calcTangents();
						void finalize();
						void createBuffers();

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

				private:
						std::vector<glm::vec3> m_positions; ///< position data
						std::vector<glm::vec2> m_uvs; ///< texcoordinates
						std::vector<glm::vec3> m_normals; ///< normal data
						std::vector<glm::vec3> m_tangents; ///< tangent data
						std::vector<unsigned int> m_indices; ///< indices

						glm::vec3 m_center;
						float m_radius;

						unsigned int m_VAO{ 0 }; ///< the vao of this mesh
						unsigned int m_VBOs[BufferObject::NUM_BUFFERS] = {0}; ///< the vbo's of this mesh

						std::vector<SubMesh> m_subMeshes;
						std::vector<std::weak_ptr<Material>> m_materials;
				};
		}
}
#endif // !MESH_H
