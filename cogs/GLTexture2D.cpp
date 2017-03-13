#include "GLTexture2D.h"
#include "Utils.h"

#include <SOIL2\SOIL2.h>
#include <GL\glew.h>

namespace cogs
{
		GLTexture2D::GLTexture2D(const std::string & _name, const std::string & _filePath)
		{
				load(_name, _filePath);
		}

		GLTexture2D::~GLTexture2D()
		{
				if (m_id != 0)
				{
						glDeleteTextures(1, &m_id);
						m_id = 0;
				}
		}

		void GLTexture2D::load(const std::string& _name, const std::string & _filePath)
		{
				m_name = _name;
				m_filePath = _filePath;

				m_id = SOIL_load_OGL_texture(m_filePath.c_str(), SOIL_LOAD_AUTO, m_id,
						SOIL_FLAG_POWER_OF_TWO
						| SOIL_FLAG_MIPMAPS
						//| SOIL_FLAG_TEXTURE_REPEATS
						| SOIL_FLAG_MULTIPLY_ALPHA
						| SOIL_FLAG_INVERT_Y);

				if (m_id == 0)
				{
						printf("SOIL loading error: '%s'\n", SOIL_last_result());
				}

				/*if (!loadTexture(m_filePath.c_str(), _alpha, &m_width, &m_height, &m_id))
				{
						throw std::runtime_error("Texture failed to load");
				}*/

		}
		void GLTexture2D::bind() const
		{
				glBindTexture(GL_TEXTURE_2D, m_id);
		}
		void GLTexture2D::unbind() const
		{
				glBindTexture(GL_TEXTURE_2D, 0);
		}

		glm::vec4 GLTexture2D::getTexCoords(int _index)
		{
				//the glm::vec4 represents the texcoords of the texture.
				//x,y represent the bottom left coordinates
				//z,w represent the top right coordinates (or width and height of the uv)
				//for single textures it's always 0,0, 1,1

				switch (m_type)
				{
				case TextureType::SINGLE:
				{
						return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
				}
				case TextureType::MULTIPLE:
				{
						//make sure the xTile isn't bigger than dims.x by using mod operator
						int xTile = _index % m_tileDims.x;
						int yTile = _index / m_tileDims.x;

						glm::vec4 uvs;
						uvs.x = (float)xTile / (float)m_tileDims.x;
						uvs.y = (float)yTile / (float)m_tileDims.y;
						uvs.z = 1.0f / (float)m_tileDims.x;
						uvs.w = 1.0f / (float)m_tileDims.y;
						return uvs;
				}
				default: break;
				}
				return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		glm::vec2 GLTexture2D::getTexOffsets(int _index)
		{
				switch (m_type)
				{
				case TextureType::SINGLE:
				{
						return glm::vec2(0.0f, 0.0f);
				}
				case TextureType::MULTIPLE:
				{
						//make sure the xTile isn't bigger than dims.x by using mod operator
						int xTile = _index % m_tileDims.x;
						int yTile = _index / m_tileDims.x;

						glm::vec2 texOffsets;
						texOffsets.x = (float)xTile / (float)m_tileDims.x;
						texOffsets.y = (float)yTile / (float)m_tileDims.y;

						return texOffsets;
				}
				default: break;
				}
				return glm::vec2(0.0f, 0.0f);
		}
}
