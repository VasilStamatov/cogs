#ifndef MATERIAL_H
#define MATERIAL_H

#include "GLSLProgram.h"

namespace cogs
{
		namespace graphics
		{
				class Material
				{
				public:
						Material(const std::string& _name) : m_name(_name) {}
						~Material() {}

						/*void setShader(std::weak_ptr<GLSLProgram> _shader) { m_shader = _shader; }
						std::weak_ptr<GLSLProgram> getShader() { return m_shader; }*/

						void setDiffuseMap(std::weak_ptr<GLTexture2D> _diffuseMap) { m_diffuseMap = _diffuseMap; }
						std::weak_ptr<GLTexture2D> getDiffuseMap()																	{ return m_diffuseMap; }

						void setNormalMap(std::weak_ptr<GLTexture2D> _normalMap) { m_normalMap = _normalMap; }
						std::weak_ptr<GLTexture2D> getNormalMap()                { return m_normalMap; }

						void setSpecularMap(std::weak_ptr<GLTexture2D> _specularMap) { m_specularMap = _specularMap; }
						std::weak_ptr<GLTexture2D> getSpecularMap()																	 { return m_specularMap; }

						void setReflectionMap(std::weak_ptr<GLTexture2D> _reflectionMap) { m_reflectionMap = _reflectionMap; }
						std::weak_ptr<GLTexture2D> getReflectionMap()																			 { return m_reflectionMap; }

						void setName(const std::string& _name) { m_name = _name; }
						const std::string& getName()										 { return m_name; }

						void setName(float _shininess) { m_shininess = _shininess; }
					 float getShininess()											{ return m_shininess; }

				private:
						std::string m_name{ "" };
						float m_shininess{ 8.0f };

						//std::weak_ptr<GLSLProgram> m_shader;

						std::weak_ptr<GLTexture2D> m_diffuseMap;
						std::weak_ptr<GLTexture2D> m_normalMap;
						std::weak_ptr<GLTexture2D> m_specularMap;
						std::weak_ptr<GLTexture2D> m_reflectionMap;
				};
		}
}
#endif // !MATERIAL_H