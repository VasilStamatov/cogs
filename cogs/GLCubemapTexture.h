#ifndef GLCUBEMAPTEXTURE_H
#define GLCUBEMAPTEXTURE_H

#include "Texture.h"
#include <string>
#include <vector>

namespace cogs
{
		namespace graphics
		{
				class GLCubemapTexture : public Texture
				{
				public:
						GLCubemapTexture(const std::string& _name, const std::vector<std::string>& _fileNames);
						GLCubemapTexture();
						~GLCubemapTexture();

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

						inline const std::string& getName() const noexcept { return m_name; }

				private:
						std::vector<std::string> m_fileNames;
						std::string m_name{ "" };
				};
		}
}
#endif