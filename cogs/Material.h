#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>

namespace cogs
{
		class GLTexture2D;

		/**
		* \brief The material class to represent the material which
		* contains different textures to bind before rendering
		*/
		class Material
		{
		public:
				Material(const std::string& _name) : m_name(_name) {}
				~Material() {}

				/*void setShader(std::weak_ptr<GLSLProgram> _shader) { m_shader = _shader; }
				std::weak_ptr<GLSLProgram> getShader() { return m_shader; }*/

				// diffuse map getter and setter
				void setDiffuseMap(std::weak_ptr<GLTexture2D> _diffuseMap) { m_diffuseMap = _diffuseMap; }
				std::weak_ptr<GLTexture2D> getDiffuseMap() { return m_diffuseMap; }

				// normal map getter and setter
				void setNormalMap(std::weak_ptr<GLTexture2D> _normalMap) { m_normalMap = _normalMap; }
				std::weak_ptr<GLTexture2D> getNormalMap() { return m_normalMap; }

				// specular map getter and setter
				void setSpecularMap(std::weak_ptr<GLTexture2D> _specularMap) { m_specularMap = _specularMap; }
				std::weak_ptr<GLTexture2D> getSpecularMap() { return m_specularMap; }

				// reflection map getter and setter
				void setReflectionMap(std::weak_ptr<GLTexture2D> _reflectionMap) { m_reflectionMap = _reflectionMap; }
				std::weak_ptr<GLTexture2D> getReflectionMap() { return m_reflectionMap; }

				// name getter and setter
				void setName(const std::string& _name) { m_name = _name; }
				const std::string& getName() { return m_name; }

				// shininess getter and setter
				void setShininess(float _shininess) { m_shininess = _shininess; }
				float getShininess() { return m_shininess; }

		private:
				std::string m_name{ "" }; ///< the name of the material
				float m_shininess{ 8.0f }; ///< shininess value of the material

				//std::weak_ptr<GLSLProgram> m_shader;

				std::weak_ptr<GLTexture2D> m_diffuseMap; ///< the diffuse map texture
				std::weak_ptr<GLTexture2D> m_normalMap; ///< the normal map texture
				std::weak_ptr<GLTexture2D> m_specularMap; ///< the specular map texture
				std::weak_ptr<GLTexture2D> m_reflectionMap; ///< the reflection map texture
		};
}
#endif // !MATERIAL_H