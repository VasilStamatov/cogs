#ifndef WINDOW_H
#define WINDOW_H

#include <SDL\SDL.h>
#include <GL\glew.h>
#include <glm\vec2.hpp>
#include <string>

#include "Color.h"

namespace cogs
{
		namespace graphics
		{
				struct Color;

				enum WindowCreationFlags : unsigned int
				{
						NONE = 0,
						INVISIBLE = 1,								  //window is not visible
						FULLSCREEN = 2,									//fullscreen window
						FULLSCREEN_DESKTOP = 4, //fullscreen window at the current desktop resolution
						BORDERLESS = 8,							  //no window decoration
						RESIZABLE = 16,							  //window can be resized
						MINIMIZED = 32,							  //window is minimized
						MAXIMIZED = 64,							  //window is maximized
						INPUT_GRABBED = 128,				//window has grabbed input focus
				};

				class Window
				{
				public:
						Window() {}
						~Window() {}
						//Initialize SDL -> Create the window -> Initialize OGL
						int create(const std::string& _windowName,
																	int _screenWidth,
																	int _screenHeight,
																	const WindowCreationFlags& _windowFlags);

						//Close the window, destroy OGL context and quit SDL
						void close();

						//Handle window specific events
						void handleEvent(SDL_Event& _event);

						//swap the buffer
						void swapBuffer();

						void clear(bool _color, bool _depth, bool _stencil = false);

						//Setters
						void setFullscreen (bool _isFullscreen);
						void setBorder				 (bool _hasBorders);
						void setWindowTitle(const std::string& _title);
						void setClearColor (const Color& _color);
						void setRelativeMouseMode(int _enabled);
						inline void resizeHandled()		noexcept { m_wasResized = false; }

						//Getters
						inline float getAspect()                const noexcept { return (float)m_width / (float)m_height; }
						inline const std::string& getTitle()    const noexcept { return m_title; }
						inline glm::vec2 getCenter()            const noexcept { return glm::vec2((float)m_width / 2.0f, (float)m_height / 2.0f); }
						inline SDL_Window* getSDLWindow()				  	const noexcept { return m_sdlWindow; }
						inline bool hasMouseFocus()													const noexcept { return m_mouseFocus; }
						inline bool hasKeyboardFocus()										const noexcept { return m_keyboardFocus; }
						inline bool isMinimized()															const noexcept { return m_minimized; }
						inline bool wasResized()									 						const noexcept { return m_wasResized; }

						inline static int getWidth()																		noexcept { return m_width; }
						inline static int getHeight()																	noexcept { return m_height; }

				private:
						//SDL must be initialized before window creation
						void initSDL();
						//OGL context must be initialized after window creation
						void initGL();

				private:
						SDL_Window* m_sdlWindow = nullptr;
						SDL_GLContext m_glContext;

						static int m_width;
						static int m_height;

						std::string m_title{ "default" };
						bool m_mouseFocus{ false };
						bool m_keyboardFocus{ false };
						bool m_fullscreen{ false };
						bool m_minimized{ false };
						bool m_wasResized{ false };
				};
		}
}
#endif // !WINDOW_H