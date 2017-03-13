#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <glm/vec2.hpp>
#include "KeyCode.h"

namespace cogs
{
		/**
		* Static class for the inputs
		*/
		class Input
		{
		public:
				/**
				* Upates the previous key map to be the same as the current
				*/
				static void update();

				/**
				* Sets a key id in the map to true
				*/
				static void pressKey(unsigned int _keyID);

				/**
				* sets a key id in the map to false
				*/
				static void releaseKey(unsigned int _keyID);

				/**
				* Checks if a key is held down
				*/
				static bool isKeyDown(const KeyCode& _keyID);

				/**
				* Checks if a key has been pressed this frame
				*/
				static bool isKeyPressed(const KeyCode& _keyID);

				/**
				* Returns the screen space coordinates of the mouse
				*/
				static const glm::vec2& getMouseCoords() noexcept { return m_mouseCoords; }

				/**
				* sets the mouse coordinates in screen space
				*/
				static void setMouseCoords(float _x, float _y);

				/**
				* Returns the screen space coordinates of the mouse
				*/
				static const glm::vec2& getMouseMotion() noexcept { return m_mouseMotion; }

				/**
				* sets the mouse coordinates in screen space
				*/
				static void setMouseMotion(float _x, float _y);

		private:
				Input() {}
				~Input() {}

				/**
				* Checks ia a key has been pressed last frame
				*/
				static bool wasKeyDown(const KeyCode& _keyID);

		private:
				static std::unordered_map<KeyCode, bool> m_keyMap; ///< the map of the keys pressed this frame
				static std::unordered_map<KeyCode, bool> m_previousKeyMap; ///< the map of the keys pressed last frame
				static glm::vec2 m_mouseCoords; ///< mouse coordinates in screen space
				static glm::vec2 m_mouseMotion; ///< mouse motion
		};
}
#endif // !INPUT_H
