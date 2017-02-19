#ifndef TEXTURE_H
#define TEXTURE_H

namespace cogs
{
		namespace graphics
		{
				using uint = unsigned int;

				/**
				* \brief Base texture class for other textures to derive from
				*/
				class Texture
				{
				public:
						/**
						* \brief constructor that sets the basic texture data
						*/
						Texture(uint _id, int _width, int _height) : m_id(_id), m_width(_width), m_height(_height) {}
						Texture() {}
						virtual ~Texture() {}

						/**
						* \brief pure virtual function for binding and unbinding the texture
						*/
						virtual void bind() const = 0;
						virtual void unbind() const = 0;

						/**
						* \brief Texture getters
						*/
						inline int getWidth()	     const noexcept { return m_width; }
						inline int getHeight()     const noexcept { return m_height; }
						inline uint getTextureID() const noexcept { return m_id; }

				protected:
						uint m_id{ 0 }; ///< the ID/handle of the texture
						int m_width{ 0 }; ///< the width of the texture
						int m_height{ 0 }; ///< the height of the texture
				};
		}
}
#endif // !TEXTURE_H
