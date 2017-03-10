#include "GLCubemapTexture.h"
#include "Utils.h"

#include <GL\glew.h>
#include <SOIL2\SOIL2.h>

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

						m_id = SOIL_load_OGL_cubemap
						(
								_fileNames.at(0).c_str(),
								_fileNames.at(1).c_str(),
								_fileNames.at(2).c_str(),
								_fileNames.at(3).c_str(),
								_fileNames.at(4).c_str(),
								_fileNames.at(5).c_str(),
								SOIL_LOAD_AUTO,
								m_id,
								SOIL_FLAG_MIPMAPS
						);
						if (m_id == 0)
						{
								printf("SOIL loading error: '%s'\n", SOIL_last_result());
						}

						/*if (!utils::loadCubemap(m_fileNames, &m_width, &m_height, &m_id))
						{
								throw std::runtime_error("Texture failed to load");
						}*/
				}
		}
}