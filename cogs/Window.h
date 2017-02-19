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
				/**
				* \brief Flags with which the window can be created
				*/
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

				/**
				* \brief Window class which handles the creation of the window,
				* and all other window-related functions
				*/
				class Window
				{
				public:
						Window() {}
						~Window() {}
						//Initialize SDL -> Create the window -> Initialize OGL

						/**
						* \brief Creates the Window
						* \param[in] _windowName - the name of the window
						* \param[in] _screenWidth -  the width of the window
						* \param[in] _screenHeight - the height of the window
						* \param[in] _windowFlags - the window creation flags
						*/
						int create(const std::string& _windowName,
																	int _screenWidth,
																	int _screenHeight,
																	const WindowCreationFlags& _windowFlags);

						/**
						* \brief Close the window, destroy OGL context and quit SDL
						*/
						void close();

						/**
						* \brief Handle window specific events
						*/
						void handleEvent(SDL_Event& _event);

						/**
						* \brief Swaps the buffer
						*/
						void swapBuffer();

						/**
						* \brief Clear the current active framebuffer
						*/
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

						/**
						* \brief static getters for width and height,
						* so that the screen size can be gotten from other places that need it
						*/
						inline static int getWidth()																		noexcept { return m_width; }
						inline static int getHeight()																	noexcept { return m_height; }

				private:
						//SDL must be initialized before window creation
						void initSDL();
						//OGL context must be initialized after window creation
						void initGL();

				private:
						SDL_Window* m_sdlWindow = nullptr; ///< the sdl handle of the window
						SDL_GLContext m_glContext; ///< the sdl GL context

						static int m_width; ///< window width
						static int m_height; ///< window height

						std::string m_title{ "default" }; ///< window title
						bool m_mouseFocus{ false };						 ///< has mouse focus flag
						bool m_keyboardFocus{ false };				///< has keyboard focus flag
						bool m_fullscreen{ false };					  ///< is fullscreen flag
						bool m_minimized{ false };								///< is minimized flag
						bool m_wasResized{ false };							///< was resized flag
				};
		}
}
#endif // !WINDOW_H