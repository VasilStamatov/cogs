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

				extern bool loadTexture(const char* _filePath, bool _alpha, int* _width, int* _height, unsigned int* _id);

				/**
				* \brief Loads a model from a file using ASSIMP, 
				*  returns a vector of all the meshes the model is constructed from
				*/
				extern std::vector<graphics::Mesh> loadModel(const std::string& _filePath);
		}
}

#endif // !UTILS_H
