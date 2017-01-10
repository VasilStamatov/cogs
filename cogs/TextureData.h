#ifndef TEXTURE_DATA_H
#define TEXTURE_DATA_H

#include <GL\glew.h>
#include <string>

namespace cogs
{
		namespace graphics
		{
				class TextureData
				{
				public:
						TextureData(GLenum _target, GLuint _id, int _width, int _height, const std::string& _filepath) :
								m_target(_target), m_id(_id), m_width(_width), m_height(_height), m_filePath(_filepath) { }

						~TextureData() { dispose(); }

						inline void bind()		 const { glBindTexture(m_target, m_id); }
						inline void unbind() const { glBindTexture(m_target, 0); }
						inline void dispose()
						{
								if (m_id != 0)
								{
										glDeleteTextures(1, &m_id);
										m_id = 0;
								}
						}

						inline bool operator> (const TextureData& _rhs) const
						{
								return m_id > _rhs.m_id;
						}

						inline bool operator!= (const TextureData& _rhs) const
						{
								return m_id != _rhs.m_id;
						}

						inline unsigned int getWidth()										const noexcept { return m_width; }
						inline unsigned int getHeight()									const noexcept { return m_height; }
						inline unsigned int getID()													const noexcept { return m_id; }
					 const std::string& getFilePath()								const noexcept { return m_filePath; }
				private:
						// no copy or assignments
						TextureData(TextureData& other) {}
						void operator=(TextureData& other) {}

				private:
						GLenum m_target = GL_TEXTURE_2D;
						GLuint m_id{ 0 };
						unsigned int m_width{ 0 };
						unsigned int m_height{ 0 };
						std::string m_filePath;
				};
		}
}
#endif // !TEXTURE_DATA_H
