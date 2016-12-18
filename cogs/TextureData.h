#ifndef TEXTURE_DATA_H
#define TEXTURE_DATA_H

#include <GL\glew.h>

namespace cogs
{
		namespace graphics
		{
				class TextureData
				{
				public:
						TextureData(GLenum _target, GLuint _id, int _width, int _height) :
								m_target(_target), m_id(_id), m_width(_width), m_height(_height) { }

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

						inline unsigned int getWidth()  const { return m_width; }
						inline unsigned int getHeight() const { return m_height; }
						inline unsigned int getID()					const { return m_id; }

				private:
						// no copy or assignments
						TextureData(TextureData& other) {}
						void operator=(TextureData& other) {}

				private:
						GLenum m_target = GL_TEXTURE_2D;
						GLuint m_id{ 0 };
						unsigned int m_width{ 0 };
						unsigned int m_height{ 0 };
				};
		}
}
#endif // !TEXTURE_DATA_H
