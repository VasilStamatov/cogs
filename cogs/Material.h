#ifndef MATERIAL_H
#define MATERIAL_H

#include "MaterialData.h"

namespace cogs
{
		namespace graphics
		{
				class Material
				{
				public:
						Material(const std::string& _name);
						Material(const std::string& _name, const GLSLProgram& _shader);
						Material(const Material& _other);
						~Material();
						
						inline void bind() { m_materialData.lock()->bind(); }
						inline void unbind() { m_materialData.lock()->unbind(); }

						inline void uploadUniforms() { m_materialData.lock()->uploadUniforms(); }
						inline void uploadTextures(const std::vector<GLTexture>& _textures)
						{ m_materialData.lock()->uploadTextures(_textures); }

						inline const GLSLProgram& getShader() const { return m_materialData.lock()->getShader(); }
						inline void setShader(const GLSLProgram& _shader) { m_materialData.lock()->setShader(_shader); }

						inline void addFloat(const std::string& _name, float _value) { m_materialData.lock()->addFloat(_name, _value); }
						inline void addVec3(const std::string& _name, const glm::vec3& _value) { m_materialData.lock()->addVec3(_name, _value); }
						inline void addVec2(const std::string& _name, const glm::vec2& _value) { m_materialData.lock()->addVec2(_name, _value); }
						inline void addMat4(const std::string& _name, const glm::mat4& _value) { m_materialData.lock()->addMat4(_name, _value); }

						inline void removeFloat(const std::string& _name) { m_materialData.lock()->removeFloat(_name); }
						inline void removeVec3(const std::string& _name) { m_materialData.lock()->removeVec3(_name); }
						inline void removeVec2(const std::string& _name) { m_materialData.lock()->removeVec2(_name); }
						inline void removeMat4(const std::string& _name) { m_materialData.lock()->removeMat4(_name); }

				private:
						static std::map<std::string, std::shared_ptr<MaterialData>> s_resourceMap;

						std::weak_ptr<MaterialData> m_materialData;
						std::string m_name{ "" };
				};
		}
}
#endif // !MATERIAL_H