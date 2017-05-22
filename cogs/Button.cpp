#include "Button.h"

namespace cogs
{
		Button::Button(const std::string& _name, float _x, float _y, float _width, float _height)
		{
				m_button = static_cast<CEGUI::PushButton*>(GUI::createWidget(GUI::getSchemeName() + "/Button", _x, _y, _width, _height, _name));
				m_button->setText(_name);
		}
		Button::~Button()
		{
				if (!GUI::isFreed())
				{
						destroy();
				}
				m_button = nullptr;
		}
		void Button::init()
		{
		}
		void Button::update(float _deltaTime)
		{
		}
		void Button::setText(const std::string & _text)
		{
				m_button->setText(_text);
		}
		void Button::addEvent(std::function<bool(const CEGUI::EventArgs&)> _funcPtr)
		{
				m_button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(_funcPtr));
		}
		void Button::destroy()
		{
				GUI::destroyWidget(m_button);
		}
}