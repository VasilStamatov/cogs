#ifndef UTILS_H
#define UTILS_H

#include "Mesh.h"

namespace cogs
{
		namespace utils
		{
				/**
				* \brief returns the time in milliseconds since the start of the program.
				*/
				extern float getTime();

				/**
				* \brief sets the program to sleep for a certain time in milliseconds
				*/
				extern void sleep(const float _millis);

				/**
				* \brief Loads a texture using SOIL, returns true if successful.
				* \param _filePath - the filepath of the texture
				* \param _alpha			 - flag whether the texture has alpha or not
				* \param _width			 - address of an integer to write to for the texture's width
				* \param _height		 - address of an integer to write to for the texture's height
				* \param _id						 - address of an integer to write to for the texture's opengl texture ID
				*/
				extern bool loadTexture(const char* _filePath, bool _alpha, int* _width, int* _height, unsigned int* _id);

				/**
				* \brief Loads a texture using SOIL, returns true if successful.
				* \param _fileNames- the 6 filepaths of the cubemap (6 textures) order should be:
				*	+X (right)
				*	-X (left)
				*	+Y (top)
				*	-Y (bottom)
				*	-Z (back)
				*	+Z (front)
				* \param _alpha			 - flag whether the texture has alpha or not
				* \param _width			 - address of an integer to write to for the texture's width
				* \param _height		 - address of an integer to write to for the texture's height
				* \param _id						 - address of an integer to write to for the texture's opengl texture ID
				*/
				extern bool loadCubemap(const std::vector<std::string>& _fileNames, int* _width, int* _height, unsigned int* _id);

				/**
				* \brief Loads a mesh data into the vectors using ASSIMP.
				* \param _filePath  - the filepath of the mesh to load
				* \param _subMeshes	- vector of submeshes the main mesh is made of
				* \param _positions	- vector of vec3 positions/vertices to fill
				* \param _uvs							- vector of vec2 texture coordinates to fill
				* \param _normals			- vector of vec3 normals to fill
				* \param _tangents  - vector of vec3 tangents to fill
				* \param _indices	  - vector of unsigned ints indices to fill for indexed rendering
				* \param _materials	- vector of Materials that the mesh has to fill
				*/
				extern void loadMesh(const std::string& _filePath,
						std::vector<graphics::SubMesh>& _subMeshes,
						std::vector<glm::vec3>& _positions,
						std::vector<glm::vec2>& _uvs,
						std::vector<glm::vec3>& _normals,
						std::vector<glm::vec3>& _tangents,
						std::vector<unsigned int>& _indices,
						std::vector<std::weak_ptr<graphics::Material>>& _materials);
		}
}

#endif // !UTILS_H
