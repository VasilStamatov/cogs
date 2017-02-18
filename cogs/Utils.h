#ifndef UTILS_H
#define UTILS_H

#include "Mesh.h"
#include "Material.h"
#include "Entity.h"
#include "Renderer3D.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
				extern bool loadCubemap(const char** _fileNames, int* _width, int* _height, unsigned int* _id);

				/**
				* \brief Loads a single mesh file with no materials, just mesh data, 
				* \param _filePath - the filepath of the model
				* \return a single mesh with the loaded data
				*/
				extern graphics::Mesh loadPrimitive(const std::string& _filePath);

				extern std::shared_ptr<ecs::Entity> loadEntityWithMeshes(const std::string& _filePath, std::weak_ptr<graphics::Renderer3D> _renderer);
				extern void loadMeshesToEntity(std::weak_ptr<ecs::Entity> _mainHolder, 
						const std::string& _filePath, std::weak_ptr<graphics::Renderer3D> _renderer);

				namespace internal
				{
						extern void processNode(aiNode* _node, const aiScene* _scene, const std::string& _directory, std::weak_ptr<ecs::Entity> _parent,
								std::weak_ptr<graphics::Renderer3D> _renderer);

						extern void processMesh(aiMesh* _aiMesh, std::weak_ptr<graphics::Mesh> _cogsMesh);
						extern void processMaterial(aiMaterial* _aiMaterial, std::weak_ptr<graphics::Material> _cogsMaterial, const std::string& _directory);
				}
		}
}

#endif // !UTILS_H
