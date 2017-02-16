#include "Framebuffer.h"
#include "Window.h"

namespace cogs
{
		namespace graphics
		{
			 std::weak_ptr<Framebuffer> Framebuffer::s_currentActive;

				Framebuffer::~Framebuffer()
				{
						if (m_fboID != 0)
						{
								glDeleteFramebuffers(1, &m_fboID);
								m_fboID = 0;
						}
						if (m_id != 0)
						{
								glDeleteTextures(1, &m_id);
								m_id = 0;
						}
						if (m_rboID != 0)
						{
								glDeleteRenderbuffers(1, &m_rboID);
								m_rboID = 0;
						}
				}

				void Framebuffer::bind() const
				{
						glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
						glViewport(0, 0, m_width, m_height);
				}

				void Framebuffer::unbind() const
				{
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
						glViewport(0, 0, Window::getWidth(), Window::getHeight());
				}

				std::shared_ptr<Framebuffer> Framebuffer::create(unsigned int _width, unsigned int _height)
				{
						std::shared_ptr<Framebuffer> newFramebuffer = std::make_shared<Framebuffer>();

						newFramebuffer->m_width = _width;
						newFramebuffer->m_height = _height;

						glGenFramebuffers(1, &newFramebuffer->m_fboID);
						glBindFramebuffer(GL_FRAMEBUFFER, newFramebuffer->m_fboID);

						if (newFramebuffer->m_id == 0)
						{
								glGenTextures(1, &newFramebuffer->m_id);
						}

						glBindTexture(GL_TEXTURE_2D, newFramebuffer->m_id);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

						glGenRenderbuffers(1, &newFramebuffer->m_rboID);
						glBindRenderbuffer(GL_RENDERBUFFER, newFramebuffer->m_rboID);
						glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);

						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newFramebuffer->m_rboID);

						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newFramebuffer->m_id, 0);

						if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
						{
								printf("Framebuffer not completed properly, check it out!");
						}

						glBindTexture(GL_TEXTURE_2D, 0);
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
						glBindRenderbuffer(GL_RENDERBUFFER, 0);

						return std::move(newFramebuffer);
				}

				void Framebuffer::setActive(std::weak_ptr<Framebuffer> _fb)
				{
						if (s_currentActive.lock() == _fb.lock())
						{
								return;
						}

						if (!_fb.expired())
						{
								_fb.lock()->bind();
						}
						else
						{
								glBindFramebuffer(GL_FRAMEBUFFER, 0);
								glViewport(0, 0, Window::getWidth(), Window::getHeight());
						}

						s_currentActive = _fb;
				}
		}
}