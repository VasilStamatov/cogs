#include "Input.h"

namespace cogs
{
		namespace utils
		{
				std::unordered_map<KeyCode, bool> Input::m_keyMap;
				std::unordered_map<KeyCode, bool> Input::m_previousKeyMap;
				glm::vec2 Input::m_mouseCoords = glm::vec2(0.0f, 0.0f);
				glm::vec2 Input::m_mouseMotion = glm::vec2(0.0f, 0.0f);

				void Input::update()
				{
						for (auto& it : m_keyMap)
						{
								m_previousKeyMap[it.first] = it.second;
						}
				}

				void Input::pressKey(unsigned int _keyID)
				{
						// Here we are treating _keyMap as an associative array.
						// if keyID doesn't already exist in _keyMap, it will get added
						m_keyMap[(KeyCode)_keyID] = true;
				}

				void Input::releaseKey(unsigned int _keyID)
				{
						// Here we are treating _keyMap as an associative array.
						// switch the pressed button from true (being pressed) to false (released)
						m_keyMap[(KeyCode)_keyID] = false;
				}

				bool Input::isKeyDown(const KeyCode& _keyID)
				{
						// We dont want to use the associative array approach here
						// because we don't want to create a key if it doesnt exist.
						// So we do it manually
						auto it = m_keyMap.find(_keyID);
						if (it != m_keyMap.end())
						{
								// Found the key
								return it->second;
						}
						else
						{
								// Didn't find the key
								return false;
						}
				}

				bool Input::isKeyPressed(const KeyCode& _keyID)
				{
						//check if it was pressed this frame, and wasn't pressed last frame
						if (isKeyDown(_keyID) == true && wasKeyDown(_keyID) == false)
						{
								return true;
						}
						return false;
				}

				void Input::setMouseCoords(float _x, float _y)
				{
						m_mouseCoords.x = _x;
						m_mouseCoords.y = _y;
				}

				void Input::setMouseMotion(float _x, float _y)
				{
						m_mouseMotion.x = _x;
						m_mouseMotion.y = _y;
				}

				bool Input::wasKeyDown(const KeyCode& _keyID)
				{
						// We dont want to use the associative array approach here
						// because we don't want to create a key if it doesnt exist.
						// So we do it manually
						auto it = m_previousKeyMap.find(_keyID);
						if (it != m_previousKeyMap.end())
						{
								// Found the key
								return it->second;
						}
						else
						{
								// Didn't find the key
								return false;
						}
				}

		}
}