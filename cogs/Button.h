#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "GUI.h"

#include <functional>

namespace cogs
{
		class Button : public Component
		{
		public:
				Button(const std::string& _name, float _x, float _y, float _width, float _height);
				~Button();

				void init() override;
				void update(float _deltaTime) override;

				void setText(const std::string& _text);
				void addEvent(std::function<bool(const CEGUI::EventArgs&)> _funcPtr);

				void destroy();

		private:
				CEGUI::PushButton* m_button;
		};
}

#endif // !BUTTON_H