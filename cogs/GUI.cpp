#include <GL/glew.h> // Include BEFORE GUI.h

#include "GUI.h"

namespace cogs
{
		CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;
		CEGUI::GUIContext* GUI::m_context = nullptr;
		CEGUI::Window* GUI::m_root = nullptr;
		std::string GUI::m_schemeName = "";
		bool GUI::m_freed = false;
		HRTimer GUI::m_timer;

		GUI::GUI()
		{
		}
		GUI::~GUI()
		{
				if (!m_freed)
				{
						destroy();
				}
		}
		void GUI::init(const std::string & _resourceDirectory)
		{
				// Check if the renderer and system were not already initialized
				if (m_renderer == nullptr)
				{
						m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
				}
				//set up the resource group directories
				CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
				rp->setResourceGroupDirectory("imagesets", _resourceDirectory + "/imagesets/");
				rp->setResourceGroupDirectory("schemes", _resourceDirectory + "/schemes/");
				rp->setResourceGroupDirectory("fonts", _resourceDirectory + "/fonts/");
				rp->setResourceGroupDirectory("layouts", _resourceDirectory + "/layouts/");
				rp->setResourceGroupDirectory("looknfeels", _resourceDirectory + "/looknfeel/");
				rp->setResourceGroupDirectory("lua_scripts", _resourceDirectory + "/lua_scripts/");
				//set the default resource groups
				CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
				CEGUI::Scheme::setDefaultResourceGroup("schemes");
				CEGUI::Font::setDefaultResourceGroup("fonts");
				CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
				CEGUI::WindowManager::setDefaultResourceGroup("layouts");
				CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
				//set up the context
				m_context = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());
				m_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
				m_context->setRootWindow(m_root);

				m_timer.start();
		}
		void GUI::destroy()
		{
				CEGUI::System::getSingleton().destroyGUIContext(*m_context);
				CEGUI::WindowManager::getSingleton().destroyWindow(m_root);
				CEGUI::System::destroy();
				CEGUI::OpenGL3Renderer::destroy(static_cast<CEGUI::OpenGL3Renderer&>(*m_renderer));
				m_context = nullptr;
				m_root = nullptr;
				m_renderer = nullptr;
				m_freed = true;
		}
		void GUI::render()
		{
				//Render the GUI
				glDisable(GL_DEPTH_TEST);
				m_renderer->beginRendering();
				m_context->draw();
				m_renderer->endRendering();
				// Clean up after CEGUI
				glBindVertexArray(0);
				glDisable(GL_SCISSOR_TEST);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		void GUI::update()
		{
				m_context->injectTimePulse(m_timer.seconds());
				m_timer.start();
		}
		void GUI::setMouseCursor(const std::string & _imageFile)
		{
				//set the image for the mouse cursor
				m_context->getMouseCursor().setDefaultImage(_imageFile);
		}
		void GUI::showMouseCursor()
		{
				//show the mouse cursor
				m_context->getMouseCursor().show();
		}
		void GUI::hideMouseCursor()
		{
				//hide the mouse cursor
				m_context->getMouseCursor().hide();
		}
		void GUI::onSDLEvent(SDL_Event & _evnt)
		{
				switch (_evnt.type)
				{
						//inject the mouse position
				case SDL_MOUSEMOTION:
				{
						m_context->injectMousePosition((float)_evnt.motion.x, (float)_evnt.motion.y);
						break;
				}
				//inject the key down
				case SDL_KEYDOWN:
				{
						m_context->injectKeyDown(SDLKeyToCEGUIKey(_evnt.key.keysym.sym));
						break;
				}
				//inject the key up
				case SDL_KEYUP:
				{
						m_context->injectKeyUp(SDLKeyToCEGUIKey(_evnt.key.keysym.sym));
						break;
				}
				//inject the text input
				case SDL_TEXTINPUT:
				{
						auto cs = CEGUI::String((CEGUI::utf8*) _evnt.text.text);
						for (unsigned int i = 0; i < cs.size(); ++i)
						{
								m_context->injectChar(cs[i]);
						}
						break;
				}
				//inject the mouse button down
				case SDL_MOUSEBUTTONDOWN:
				{
						m_context->injectMouseButtonDown(SDLButtonToCEGUIButton(_evnt.button.button));
						break;
				}
				//inject the mouse button up
				case SDL_MOUSEBUTTONUP:
				{
						m_context->injectMouseButtonUp(SDLButtonToCEGUIButton(_evnt.button.button));
						break;
				}
				}
		}
		void GUI::loadScheme(const std::string & _schemeFile)
		{
				//load the scheme file
				CEGUI::SchemeManager::getSingleton().createFromFile(_schemeFile);
				m_schemeName = _schemeFile.substr(0, _schemeFile.find_last_of('.'));
		}
		void GUI::setFont(const std::string & _fontFile)
		{
				//set the font
				CEGUI::FontManager::getSingleton().createFromFile(_fontFile + ".font");
				m_context->setDefaultFont(_fontFile);
		}

		CEGUI::Window* GUI::createWidget(const std::string& _type, float _x, float _y, float _width, float _height, const std::string& _name)
		{
				return createWidget(m_root, _type, _x, _y, _width, _height, _name);
		}

		CEGUI::Window* GUI::createWidget(CEGUI::Window* _parent, const std::string& _type, float _x, float _y, float _width, float _height, const std::string& _name)
		{
				CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(_type, _name);
				_parent->addChild(newWindow);
				setWidgetDestRect(newWindow, _x, _y, _width, _height);
				return newWindow;
		}

		void GUI::setWidgetDestRect(CEGUI::Window* _widget, float _x, float _y, float _width, float _height)
		{
				//set the widget destination rectangle
				_widget->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, _x), CEGUI::UDim(0.0f, _y)));
				_widget->setSize(CEGUI::USize(CEGUI::UDim(0.0f, _width), CEGUI::UDim(0.0f, _height)));
		}

		void GUI::destroyWidget(CEGUI::Window * _widget)
		{
				CEGUI::WindowManager::getSingleton().destroyWindow(_widget);
		}

		CEGUI::Key::Scan GUI::SDLKeyToCEGUIKey(SDL_Keycode _key)
		{
				//Convert the SDL keys to CEGUI keys
				//Reference: http://cegui.org.uk/wiki/SDL_to_CEGUI_keytable
				using namespace CEGUI;
				switch (_key)
				{
				case SDLK_BACKSPACE:    return Key::Backspace;
				case SDLK_TAB:          return Key::Tab;
				case SDLK_RETURN:       return Key::Return;
				case SDLK_PAUSE:        return Key::Pause;
				case SDLK_ESCAPE:       return Key::Escape;
				case SDLK_SPACE:        return Key::Space;
				case SDLK_COMMA:        return Key::Comma;
				case SDLK_MINUS:        return Key::Minus;
				case SDLK_PERIOD:       return Key::Period;
				case SDLK_SLASH:        return Key::Slash;
				case SDLK_0:            return Key::Zero;
				case SDLK_1:            return Key::One;
				case SDLK_2:            return Key::Two;
				case SDLK_3:            return Key::Three;
				case SDLK_4:            return Key::Four;
				case SDLK_5:            return Key::Five;
				case SDLK_6:            return Key::Six;
				case SDLK_7:            return Key::Seven;
				case SDLK_8:            return Key::Eight;
				case SDLK_9:            return Key::Nine;
				case SDLK_COLON:        return Key::Colon;
				case SDLK_SEMICOLON:    return Key::Semicolon;
				case SDLK_EQUALS:       return Key::Equals;
				case SDLK_LEFTBRACKET:  return Key::LeftBracket;
				case SDLK_BACKSLASH:    return Key::Backslash;
				case SDLK_RIGHTBRACKET: return Key::RightBracket;
				case SDLK_a:            return Key::A;
				case SDLK_b:            return Key::B;
				case SDLK_c:            return Key::C;
				case SDLK_d:            return Key::D;
				case SDLK_e:            return Key::E;
				case SDLK_f:            return Key::F;
				case SDLK_g:            return Key::G;
				case SDLK_h:            return Key::H;
				case SDLK_i:            return Key::I;
				case SDLK_j:            return Key::J;
				case SDLK_k:            return Key::K;
				case SDLK_l:            return Key::L;
				case SDLK_m:            return Key::M;
				case SDLK_n:            return Key::N;
				case SDLK_o:            return Key::O;
				case SDLK_p:            return Key::P;
				case SDLK_q:            return Key::Q;
				case SDLK_r:            return Key::R;
				case SDLK_s:            return Key::S;
				case SDLK_t:            return Key::T;
				case SDLK_u:            return Key::U;
				case SDLK_v:            return Key::V;
				case SDLK_w:            return Key::W;
				case SDLK_x:            return Key::X;
				case SDLK_y:            return Key::Y;
				case SDLK_z:            return Key::Z;
				case SDLK_DELETE:       return Key::Delete;
				case SDLK_KP_PERIOD:    return Key::Decimal;
				case SDLK_KP_DIVIDE:    return Key::Divide;
				case SDLK_KP_MULTIPLY:  return Key::Multiply;
				case SDLK_KP_MINUS:     return Key::Subtract;
				case SDLK_KP_PLUS:      return Key::Add;
				case SDLK_KP_ENTER:     return Key::NumpadEnter;
				case SDLK_KP_EQUALS:    return Key::NumpadEquals;
				case SDLK_UP:           return Key::ArrowUp;
				case SDLK_DOWN:         return Key::ArrowDown;
				case SDLK_RIGHT:        return Key::ArrowRight;
				case SDLK_LEFT:         return Key::ArrowLeft;
				case SDLK_INSERT:       return Key::Insert;
				case SDLK_HOME:         return Key::Home;
				case SDLK_END:          return Key::End;
				case SDLK_PAGEUP:       return Key::PageUp;
				case SDLK_PAGEDOWN:     return Key::PageDown;
				case SDLK_F1:           return Key::F1;
				case SDLK_F2:           return Key::F2;
				case SDLK_F3:           return Key::F3;
				case SDLK_F4:           return Key::F4;
				case SDLK_F5:           return Key::F5;
				case SDLK_F6:           return Key::F6;
				case SDLK_F7:           return Key::F7;
				case SDLK_F8:           return Key::F8;
				case SDLK_F9:           return Key::F9;
				case SDLK_F10:          return Key::F10;
				case SDLK_F11:          return Key::F11;
				case SDLK_F12:          return Key::F12;
				case SDLK_F13:          return Key::F13;
				case SDLK_F14:          return Key::F14;
				case SDLK_F15:          return Key::F15;
				case SDLK_RSHIFT:       return Key::RightShift;
				case SDLK_LSHIFT:       return Key::LeftShift;
				case SDLK_RCTRL:        return Key::RightControl;
				case SDLK_LCTRL:        return Key::LeftControl;
				case SDLK_RALT:         return Key::RightAlt;
				case SDLK_LALT:         return Key::LeftAlt;
				case SDLK_SYSREQ:       return Key::SysRq;
				case SDLK_MENU:         return Key::AppMenu;
				case SDLK_POWER:        return Key::Power;
				default:                return Key::Unknown;
				}
		}

		CEGUI::MouseButton GUI::SDLButtonToCEGUIButton(Uint8 _sdlButton)
		{
				//convert SDL mouse buttons to CEGUI mouse buttons
				switch (_sdlButton)
				{
				case SDL_BUTTON_LEFT:   return CEGUI::MouseButton::LeftButton;
				case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
				case SDL_BUTTON_RIGHT:  return CEGUI::MouseButton::RightButton;
				case SDL_BUTTON_X1:     return CEGUI::MouseButton::X1Button;
				case SDL_BUTTON_X2:     return CEGUI::MouseButton::X2Button;
				}
				return CEGUI::MouseButton::NoButton;
		}
}