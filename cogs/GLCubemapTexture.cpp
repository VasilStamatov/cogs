#include "GLCubemapTexture.h"
#include "Utils.h"

#include <GL\glew.h>

namespace cogs
{
		namespace graphics
		{
				GLCubemapTexture::GLCubemapTexture(const std::string & _name, const std::vector<std::string>& _fileNames)
				{
						load(_name, _fileNames);
				}
				GLCubemapTexture::GLCubemapTexture()
				{
				}
				GLCubemapTexture::~GLCubemapTexture()
				{
						if (m_id != 0)
						{
								glDeleteTextures(1, &m_id);
								m_id = 0;
						}
				}
				void GLCubemapTexture::bind() const
				{
						glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
				}
				void GLCubemapTexture::unbind() const
				{
						glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				}
				void GLCubemapTexture::load(const std::string & _name, const std::vector<std::string>& _fileNames)
				{
						m_name = _name;
						m_fileNames = _fileNames;

						std::vector<const char*> cstrings{};

						for (auto& fileName : m_fileNames)
						{
								cstrings.push_back(fileName.c_str());
						}

						if (!utils::loadCubemap(cstrings.data(), &m_width, &m_height, &m_id))
						{
								throw std::runtime_error("Texture failed to load");
						}
				}
		}
}