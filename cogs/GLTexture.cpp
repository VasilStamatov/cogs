#include "GLTexture.h"
#include <SOIL\SOIL.h>

namespace cogs
{
		namespace graphics
		{
				std::map<std::string, std::shared_ptr<TextureData>> GLTexture::s_resourceMap;

				bool GLTexture::load(const std::string & _filePath, bool _alpha)
				{
						auto iter = s_resourceMap.find(_filePath);

						//check if it's not in the map
						if (iter == s_resourceMap.end())
						{
								int width, height;
								GLuint texID = 0;

								unsigned char* image = SOIL_load_image(_filePath.c_str(), &width, &height, 0, _alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

								if (image == nullptr)
								{
										return false;
								}

								//Generate the openGL texture object
								glGenTextures(1, &texID);

								//Bind the texture object
								glBindTexture(GL_TEXTURE_2D, texID);

								//Upload the pixels to the texture
								glTexImage2D(GL_TEXTURE_2D, 0, _alpha ? GL_RGBA : GL_RGB, width, height, 0, _alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
								//Generate the mipmaps
								glGenerateMipmap(GL_TEXTURE_2D);

								//Set some texture parameters
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

								//Unbind the texture
								glBindTexture(GL_TEXTURE_2D, 0);

								SOIL_free_image_data(image);

								std::shared_ptr<TextureData> newTexture = std::make_shared<TextureData>(GL_TEXTURE_2D, texID, width, height);
								s_resourceMap.insert(std::make_pair(_filePath, std::move(newTexture)));
								m_textureData = s_resourceMap.at(_filePath);
								m_filePath = _filePath;
						}
						else
						{
								//if the texture is in the map, then
								//return the already existing (cached) texture
								m_textureData = iter->second;
						}

						//return success
						return true;
				}
		}
}
