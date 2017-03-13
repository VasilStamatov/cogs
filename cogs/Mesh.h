#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include	<memory>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

namespace cogs
{
		class Material;

		//Structure of submeshes the mesh is composed of
		struct SubMesh
		{
				unsigned int m_baseVertex{ 0 };
				unsigned int m_baseIndex{ 0 };
				unsigned int m_numIndices{ 0 };
				unsigned int m_materialIndex{ 9999 };
		};
		/* Structure to represent the bounding sphere around the mesh (for culling) */
		struct MeshBoundingSphere
		{
				glm::vec3 m_center;
				float m_radius;
		};
		/* Structure to represent the bounding box around the mesh (for culling) */
		struct MeshBoundingBox
		{
				glm::vec3 m_min;
				glm::vec3 m_max;
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
				bool isValid(const std::vector<glm::vec3>& _positions,
						const std::vector<glm::vec2>& _uvs,
						const std::vector<glm::vec3>& _normals,
						const std::vector<glm::vec3>& _tangents) const;

				/**
				* \brief loads mesh data into this object (buffer objects, vertex data etc.)
				*/
				void load(const std::string& _filePath);

				inline const MeshBoundingSphere& getSphereBounds() const noexcept { return m_boundingSphere; }
				inline const MeshBoundingBox& getBoxBounds()						 const noexcept { return m_boundingBox; }
				inline const std::vector<SubMesh>& getSubMeshes()					const noexcept { return m_subMeshes; }
				inline const std::vector<std::weak_ptr<Material>>& getMaterials()	const noexcept { return m_materials; }

		private:
				/* internal utility functions */
				void calcBounds(const std::vector<glm::vec3>& _positions);

				void calcNormals(const std::vector<glm::vec3>& _positions,
						std::vector<glm::vec3>& _normals,
						std::vector<unsigned int>& _indices);

				void calcTangents(const std::vector<glm::vec3>& _positions,
						std::vector<glm::vec2>& _uvs,
						std::vector<glm::vec3>& _normals,
						std::vector<glm::vec3>& _tangents,
						std::vector<unsigned int>& _indices);

				void finalize(const std::vector<glm::vec3>& _positions,
						std::vector<glm::vec2>& _uvs,
						std::vector<glm::vec3>& _normals,
						std::vector<glm::vec3>& _tangents,
						std::vector<unsigned int>& _indices);

				void createBuffers(const std::vector<glm::vec3>& _positions,
						std::vector<glm::vec2>& _uvs,
						std::vector<glm::vec3>& _normals,
						std::vector<glm::vec3>& _tangents,
						std::vector<unsigned int>& _indices);

				//enum for all the buffer objects
				enum BufferObject
				{
						POSITION,
						TEXCOORD,
						NORMAL,
						TANGENT,
						WORLDMAT,

						INDEX,

						NUM_BUFFERS
				};

		private:
				MeshBoundingBox m_boundingBox;
				MeshBoundingSphere m_boundingSphere;

				unsigned int m_numIndices{ 0 };
				unsigned int m_VAO{ 0 }; ///< the vao of this mesh
				unsigned int m_VBOs[BufferObject::NUM_BUFFERS] = { 0 }; ///< the vbo's of this mesh

				std::vector<SubMesh> m_subMeshes;
				std::vector<std::weak_ptr<Material>> m_materials;
		};
}
#endif // !MESH_H
