#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>

namespace cogs
{
		namespace graphics
		{
				class Framebuffer
				{
				public:
						Framebuffer() {}
						~Framebuffer();

						unsigned int getWidth()					const noexcept { return m_width; }
						unsigned int getHeight()				const noexcept { return m_height; }
						unsigned int getFBO()							const noexcept { return m_fboID; }
						unsigned int getRBO()							const noexcept { return m_rboID; }
						unsigned int getTextureID() const noexcept { return m_textureID; }

						static std::shared_ptr<Framebuffer> create(unsigned int _width, unsigned int _height);
						static std::weak_ptr<Framebuffer> getCurrentActive() { return s_currentActive; }
						static void setActive(std::weak_ptr<Framebuffer> _fb);

				private:
						static std::weak_ptr<Framebuffer> s_currentActive;

						unsigned int m_width				{ 0 };
						unsigned int m_height			{ 0 };
						unsigned int m_fboID				{ 0 };
						unsigned int m_rboID				{ 0 };
						unsigned int m_textureID{ 0 };
				};
		}
}

#endif // !FRAMEBUFFER_H