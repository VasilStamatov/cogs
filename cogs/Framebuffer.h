#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>

#include "Texture.h"

namespace cogs
{
		namespace graphics
		{
				class Framebuffer : public Texture
				{
				public:
						Framebuffer() {}
						~Framebuffer();

						void bind()   const override;
						void unbind() const override;

						uint getFBO()	const noexcept { return m_fboID; }
						uint getRBO()	const noexcept { return m_rboID; }

						static std::shared_ptr<Framebuffer> create(unsigned int _width, unsigned int _height);
						static std::weak_ptr<Framebuffer> getCurrentActive() { return s_currentActive; }
						static void setActive(std::weak_ptr<Framebuffer> _fb);

				private:
						static std::weak_ptr<Framebuffer> s_currentActive;

						uint m_fboID{ 0 };
						uint m_rboID{ 0 };
				};
		}
}

#endif // !FRAMEBUFFER_H