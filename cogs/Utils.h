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
				extern bool loadCubemap(const char** _fileNames, int* _width, int* _height, unsigned int* _id);

				/**
				* \brief Loads a model from a file using ASSIMP, 
				* \param _filePath - the filepath of the model
				* \return a vector of all the meshes the model is constructed from
				*/
				extern std::vector<graphics::Mesh> loadModel(const std::string& _filePath);
		}
}

#endif // !UTILS_H
