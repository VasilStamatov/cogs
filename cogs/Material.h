#ifndef MATERIAL_H
#define MATERIAL_H

#include <unordered_map>

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

namespace cogs
{
		namespace graphics
		{
				class Material
				{
				public:
						Material(const std::string& _name) : m_name(_name) {}
						~Material() {}
						
						inline void addFloat(const std::string& _name, float _value)											{ m_floatMap[_name] = _value; }
						inline void addVec3(const std::string& _name, const glm::vec3& _value) { m_vec3Map[_name] = _value; }
						inline void addVec2(const std::string& _name, const glm::vec2& _value) { m_vec2Map[_name] = _value; }
						inline void addMat4(const std::string& _name, const glm::mat4& _value) { m_matrixMap[_name] = _value; }

						inline void removeFloat(const std::string& _name) { m_floatMap.erase(_name); }
						inline void removeVec3(const std::string& _name)  { m_vec3Map.erase(_name); }
						inline void removeVec2(const std::string& _name)  { m_vec2Map.erase(_name); }
						inline void removeMat4(const std::string& _name)  { m_matrixMap.erase(_name); }

						inline std::unordered_map<std::string, float>     getFloatMap() { return m_floatMap; }
						inline std::unordered_map<std::string, glm::vec3> getVec3Map()  { return m_vec3Map; }
						inline std::unordered_map<std::string, glm::vec2> getVec2Map()  { return m_vec2Map; }
						inline std::unordered_map<std::string, glm::mat4> getMat4Map()  { return m_matrixMap; }

				private:
						std::string m_name{ "" };

						std::unordered_map<std::string, float> m_floatMap;
						std::unordered_map<std::string, glm::vec3> m_vec3Map;
						std::unordered_map<std::string, glm::vec2> m_vec2Map;
						std::unordered_map<std::string, glm::mat4> m_matrixMap;
				};
		}
}
#endif // !MATERIAL_H