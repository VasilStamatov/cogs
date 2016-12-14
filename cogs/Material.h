#ifndef MATERIAL_H
#define MATERIAL_H

#include "Object.h"
#include "GLSLProgram.h"

#include <memory>

namespace cogs
{
		namespace graphics
		{
				class Material : public ecs::Object
				{
				public:
						Material();
						Material(const std::string& _materialName, std::unique_ptr<GLSLProgram> _shader);
						Material(const std::string& _materialName,
								const std::string& _vsFilePath, const std::string& _fsFilePath, const std::string& _gsFilePath = "");

						~Material();

						void bind();

						void unbind();

						void setShader(std::unique_ptr<GLSLProgram> _shader);
						void setShader(const std::string& _vsFilePath, const std::string& _fsFilePath, const std::string& _gsFilePath = "");

						/* Registers the location of an attribute in this shader (must be called after linking) */
						void registerAttribute(const std::string& _attrib);

						/* Registers the location of a uniform in this shader (must be called after linking) */
						void registerUniform(const std::string& _uniform);

						//accesses elements : attributes/uniforms;
						AttribLocation getAttribLocation(const std::string& _attrib);
						UniformLocation getUniformLocation(const std::string& _uniform);

						/* Upload values to the shader */
						void uploadValue(const std::string& _uniformName, const glm::mat4& _matrix);
						void uploadValue(const std::string& _uniformName, const float& _float);
						void uploadValue(const std::string& _uniformName, const int& _int);
						void uploadValue(const std::string& _uniformName, const glm::vec2& _vec2);
						void uploadValue(const std::string& _uniformName, const glm::vec3& _vec3);
						void uploadValue(const std::string& _uniformName, const glm::vec4& _vec4);
				private:
						std::unique_ptr<GLSLProgram> m_shader;
						//TODO: Add vector of textures
				};
		}
}
#endif // !MATERIAL_H