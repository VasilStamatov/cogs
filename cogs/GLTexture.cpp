#include "GLTexture.h"
#include "Utils.h"

namespace cogs
{
		namespace graphics
		{
				std::map<std::string, std::shared_ptr<TextureData>> GLTexture::s_resourceMap;

				GLTexture::GLTexture(const std::string & _name, const std::string & _filePath, bool _alpha)
				{
						load(_name, _filePath, _alpha);
				}

				GLTexture::~GLTexture()
				{
						dispose();
				}

				void GLTexture::load(const std::string& _name, const std::string & _filePath, bool _alpha)
				{
						m_name = _name;
						auto iter = s_resourceMap.find(_filePath);

						//check if it's not in the map
						if (iter == s_resourceMap.end())
						{
								int width, height;
								GLuint texID = 0;

								if (!utils::loadTexture(_filePath.c_str(), _alpha, &width, &height, &texID))
								{
										throw std::runtime_error("Texture failed to load");
								}

								std::shared_ptr<TextureData> newTexture = std::make_shared<TextureData>(GL_TEXTURE_2D, texID, width, height, _filePath);
								s_resourceMap.insert(std::make_pair(_filePath, std::move(newTexture)));
								m_textureData = s_resourceMap.at(_filePath);
						}
						else
						{
								//if the texture is in the map, then
								//return the already existing (cached) texture
								m_textureData = iter->second;
						}
				}
		}
}
