#ifndef WINDOW_H
#define WINDOW_H

#include <SDL\SDL.h>
#include <GL\glew.h>
#include <glm\vec2.hpp>
#include <string>
namespace cogs
{
		namespace graphics
		{
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
						int create(const std::string& _windowName, int _screenWidth, int _screenHeight, const WindowCreationFlags& _windowFlags);

						//Close the window, destroy OGL context and quit SDL
						void close();

						//Handle window specific events
						void handleEvent(SDL_Event& _event);

						//swap the buffer
						void swapBuffer();

						//Setters
						void setFullscreen(bool _isFullscreen);
						inline void resizeHandled()		noexcept { m_wasResized = false; }

						//Getters
						inline int getWidth()                   const noexcept { return m_width; }
						inline int getHeight()                  const noexcept { return m_height; }
						inline float getAspect()                const noexcept { return (float)m_width / (float)m_height; }
						inline const std::string& getTitle()    const noexcept { return m_title; }
						inline glm::vec2 getCenter()            const noexcept { return glm::vec2((float)m_width / 2.0f, (float)m_height / 2.0f); }
						inline SDL_Window* getSDLWindow()				  	const noexcept { return m_sdlWindow; }
						inline bool hasMouseFocus()													const noexcept { return m_mouseFocus; }
						inline bool hasKeyboardFocus()										const noexcept { return m_keyboardFocus; }
						inline bool isMinimized()															const noexcept { return m_minimized; }
						inline bool wasResized()									 						const noexcept { return m_wasResized; }

				private:
						//SDL must be initialized before window creation
						void initSDL();
						//OGL context must be initialized after window creation
						void initGL();

				private:
						SDL_Window* m_sdlWindow = nullptr;
						SDL_GLContext m_glContext;

						std::string m_title{ "default" };
						int m_width{ 0 };
						int m_height{ 0 };

						bool m_mouseFocus{ false };
						bool m_keyboardFocus{ false };
						bool m_fullscreen{ false };
						bool m_minimized{ false };
						bool m_wasResized{ false };
				};
		}
}
#endif // !WINDOW_H