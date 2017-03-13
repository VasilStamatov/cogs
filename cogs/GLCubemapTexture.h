#ifndef GLCUBEMAPTEXTURE_H
#define GLCUBEMAPTEXTURE_H

#include "Texture.h"
#include <string>
#include <vector>

namespace cogs
{
		/**
		* \brief class to handle opengl cubemap textures
		*/
		class GLCubemapTexture : public Texture
		{
		public:
				/**
				* \brief construct the texture with a name and the 6 filenames from where to load the 6 faces
				*/
				GLCubemapTexture(const std::string& _name, const std::vector<std::string>& _fileNames);
				GLCubemapTexture();
				~GLCubemapTexture();

				/**
				* \brief overriden bind/unbind functions from the base Texture class
				*/
				void bind()   const override;
				void unbind() const override;

				// Loads a cubemap texture from 6 individual texture faces
				// Order should be:
				// +X (right)
				// -X (left)
				// +Y (top)
				// -Y (bottom)
				// -Z (back)
				// +Z (front)
				void load(const std::string& _name, const std::vector<std::string>& _fileNames);

				//name getter
				inline const std::string& getName() const noexcept { return m_name; }

		private:
				std::vector<std::string> m_fileNames; ///< set of the 6 filenamesof each texture
				std::string m_name{ "" }; ///< the name of the cubemap
		};
}
#endif