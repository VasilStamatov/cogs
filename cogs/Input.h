#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <glm/vec2.hpp>
#include "KeyCode.h"

namespace cogs
{
		namespace utils
		{
				class Input
				{
				public:
						static void update();

						static void pressKey(unsigned int _keyID);
						static void releaseKey(unsigned int _keyID);
						static void setMouseCoords(float _x, float _y);

						static bool isKeyDown(const KeyCode& _keyID);
						static bool isKeyPressed(const KeyCode& _keyID);

						static const glm::vec2& getMouseCoords() noexcept { return m_mouseCoords; }

				private:
						Input() {}
						~Input() {}

						static bool wasKeyDown(const KeyCode& _keyID);

				private:
						static std::unordered_map<KeyCode, bool> m_keyMap;
						static std::unordered_map<KeyCode, bool> m_previousKeyMap;
						static glm::vec2 m_mouseCoords;
				};
		}
}
#endif // !INPUT_H
