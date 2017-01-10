#include "Input.h"

namespace cogs
{
		namespace utils
		{
				std::unordered_map<KeyID, bool> Input::m_keyMap;
				std::unordered_map<KeyID, bool> Input::m_previousKeyMap;
				glm::vec2 Input::m_mouseCoords;

				void Input::update()
				{
						for (auto& it : m_keyMap)
						{
								m_previousKeyMap[it.first] = it.second;
						}
				}

				void Input::pressKey(KeyID _keyID)
				{
						// Here we are treating _keyMap as an associative array.
						// if keyID doesn't already exist in _keyMap, it will get added
						m_keyMap[_keyID] = true;
				}

				void Input::releaseKey(KeyID _keyID)
				{
						// Here we are treating _keyMap as an associative array.
						// switch the pressed button from true (being pressed) to false (released)
						m_keyMap[_keyID] = false;
				}

				void Input::setMouseCoords(float _x, float _y)
				{
						m_mouseCoords.x = _x;
						m_mouseCoords.y = _y;
				}

				bool Input::isKeyDown(KeyID _keyID)
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

				bool Input::isKeyPressed(KeyID _keyID)
				{
						//check if it was pressed this frame, and wasn't pressed last frame
						if (isKeyDown(_keyID) == true && wasKeyDown(_keyID) == false)
						{
								return true;
						}
						return false;
				}

				bool Input::wasKeyDown(KeyID _keyID)
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