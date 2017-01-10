#ifndef MATERIAL_DATA_H
#define MATERIAL_DATA_H

#include "GLSLProgram.h"
#include "GLTexture.h"

namespace cogs
{
		namespace graphics
		{
				class MaterialData
				{
				public:
						MaterialData() {}
						MaterialData(const GLSLProgram& _shader) : m_shader(_shader) {}
						~MaterialData() { dispose(); }

						inline void dispose() { m_shader.dispose(); }
						 
						inline void bind() { m_shader.use(); }
						inline void unbind() { m_shader.unUse(); }
						 
						inline void uploadUniforms()
						{
								for (auto& var : m_floatMap)
								{
										m_shader.uploadValue(var.first, var.second);
								}
								for (auto& var : m_vec3Map)
								{
										m_shader.uploadValue(var.first, var.second);
								}
								for (auto& var : m_vec2Map)
								{
										m_shader.uploadValue(var.first, var.second);
								}
								for (auto& var : m_matrixMap)
								{
										m_shader.uploadValue(var.first, var.second);
								}
						}
						inline void uploadTextures(const std::vector<GLTexture>& _textures)
						{
								unsigned int diffuseNr = 1;
								unsigned int specularNr = 1;
								unsigned int ambientNr = 1;
								unsigned int normalNr = 1;

								for (unsigned int i = 0; i < _textures.size(); i++)
								{
										unsigned int number;
										const std::string& textureName = _textures.at(i).getName();

										if (textureName == "texture_diffuse")
										{
												number = diffuseNr++;
										}
										else if (textureName == "texture_specular")
										{
												number = specularNr++;
										}
										else if (textureName == "texture_ambient")
										{
												number = ambientNr++;
										}
										else if (textureName == "texture_normal")
										{
												number = normalNr++;
										}
										glUniform1i(m_shader.getUniformLocation("material." + textureName + std::to_string(number)), i);
										glActiveTexture(GL_TEXTURE0 + i);
										glBindTexture(GL_TEXTURE_2D, _textures.at(i).getID());
								}
								glActiveTexture(GL_TEXTURE0); // Always good practice to set everything back to defaults once configured.
						}

						inline const GLSLProgram& getShader() const { return m_shader; }
						inline void setShader(const GLSLProgram& _shader) { m_shader = _shader; }
						 
						inline void addFloat(const std::string& _name, float _value)											{ m_floatMap[_name] = _value; }
						inline void addVec3(const std::string& _name, const glm::vec3& _value) { m_vec3Map[_name] = _value; }
						inline void addVec2(const std::string& _name, const glm::vec2& _value) { m_vec2Map[_name] = _value; }
						inline void addMat4(const std::string& _name, const glm::mat4& _value) { m_matrixMap[_name] = _value; }
						 
						inline void removeFloat(const std::string& _name) { m_floatMap.erase(_name); }
						inline void removeVec3(const std::string& _name) { m_vec3Map.erase(_name); }
						inline void removeVec2(const std::string& _name) { m_vec2Map.erase(_name); }
						inline void removeMat4(const std::string& _name) { m_matrixMap.erase(_name); }

				private:
						GLSLProgram m_shader;
						std::unordered_map<std::string, float> m_floatMap;
						std::unordered_map<std::string, glm::vec3> m_vec3Map;
						std::unordered_map<std::string, glm::vec2> m_vec2Map;
						std::unordered_map<std::string, glm::mat4> m_matrixMap;
				};
		}
}
#endif // !MATERIAL_DATA_H