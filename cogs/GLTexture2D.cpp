#include "GLTexture2D.h"
#include "Utils.h"

#include <GL\glew.h>

namespace cogs
{
		namespace graphics
		{
				GLTexture2D::GLTexture2D(const std::string & _name, const std::string & _filePath, bool _alpha)
				{
						load(_name, _filePath, _alpha);
				}

				GLTexture2D::~GLTexture2D()
				{
						if (m_id != 0)
						{
								glDeleteTextures(1, &m_id);
								m_id = 0;
						}
				}

				void GLTexture2D::load(const std::string& _name, const std::string & _filePath, bool _alpha)
				{
						m_name = _name;
						m_filePath = _filePath;

						if (!utils::loadTexture(m_filePath.c_str(), _alpha, &m_width, &m_height, &m_id))
						{
								throw std::runtime_error("Texture failed to load");
						}

				}
				void GLTexture2D::bind() const
				{
						glBindTexture(GL_TEXTURE_2D, m_id);
				}
				void GLTexture2D::unbind() const
				{
						glBindTexture(GL_TEXTURE_2D, 0);
				}
		}
}
