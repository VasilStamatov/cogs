#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <glm/vec2.hpp>

namespace cogs
{
		namespace utils
		{
				using KeyID = unsigned int;

				class Input
				{
				public:
						static void update();

						static void pressKey(KeyID _keyID);
						static void releaseKey(KeyID _keyID);
						static void setMouseCoords(float _x, float _y);

						static bool isKeyDown(KeyID _keyID);

						static bool isKeyPressed(KeyID _keyID);

						static const glm::vec2& getMouseCoords() noexcept { return m_mouseCoords; }

				private:
						Input() {}
						~Input() {}

						static bool wasKeyDown(KeyID _keyID);

				private:
						static std::unordered_map<KeyID, bool> m_keyMap;
						static std::unordered_map<KeyID, bool> m_previousKeyMap;
						static glm::vec2 m_mouseCoords;
				};
		}
}
#endif // !INPUT_H
