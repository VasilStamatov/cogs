#ifndef TEXTURE_H
#define TEXTURE_H

namespace cogs
{
		namespace graphics
		{
				using uint = unsigned int;

				class Texture
				{
				public:
						Texture(uint _id, int _width, int _height) : m_id(_id), m_width(_width), m_height(_height) {}
						Texture() {}
						virtual ~Texture() {}

						virtual void bind() const = 0;
						virtual void unbind() const = 0;

						inline int getWidth()	  const noexcept { return m_width; }
						inline int getHeight()  const noexcept { return m_height; }
						inline uint getID()					const noexcept { return m_id; }

				protected:
						uint m_id{ 0 };
						int m_width{ 0 };
						int m_height{ 0 };
				};
		}
}
#endif // !TEXTURE_H
