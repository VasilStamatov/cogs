#include "Material.h"

namespace cogs
{
		namespace graphics
		{
				Material::Material()
				{
				}

				Material::Material(const std::string & _materialName, std::unique_ptr<GLSLProgram> _shader)
						: ecs::Object(_materialName)
				{
						m_shader = std::move(_shader);
				}

				Material::Material(const std::string & _materialName,
						const std::string & _vsFilePath, const std::string & _fsFilePath, const std::string & _gsFilePath)
						: ecs::Object(_materialName)
				{
						m_shader = std::make_unique<GLSLProgram>(_vsFilePath, _fsFilePath, _gsFilePath);
				}

				Material::~Material()
				{
						m_shader->dispose();
				}

				void Material::bind()
				{
						m_shader->use();
				}

				void Material::unbind()
				{
						m_shader->unUse();
				}

				void Material::setShader(std::unique_ptr<GLSLProgram> _shader)
				{
						m_shader = std::move(_shader);
				}

				void Material::setShader(const std::string & _vsFilePath, const std::string & _fsFilePath, const std::string & _gsFilePath)
				{
						m_shader->dispose();
						m_shader->compileShaders(_vsFilePath, _fsFilePath, _gsFilePath);
				}

				void Material::registerAttribute(const std::string & _attrib)
				{
						m_shader->registerAttribute(_attrib);
				}

				void Material::registerUniform(const std::string & _uniform)
				{
						m_shader->registerUniform(_uniform);
				}

				AttribLocation Material::getAttribLocation(const std::string & _attrib)
				{
						return m_shader->getAttribLocation(_attrib);
				}

				UniformLocation Material::getUniformLocation(const std::string & _uniform)
				{
						return m_shader->getUniformLocation(_uniform);
				}

				void Material::uploadValue(const std::string & _uniformName, const glm::mat4 & _matrix)
				{
						m_shader->uploadValue(_uniformName, _matrix);
				}

				void Material::uploadValue(const std::string & _uniformName, const float & _float)
				{
						m_shader->uploadValue(_uniformName, _float);
				}

				void Material::uploadValue(const std::string & _uniformName, const int & _int)
				{
						m_shader->uploadValue(_uniformName, _int);
				}

				void Material::uploadValue(const std::string & _uniformName, const glm::vec2 & _vec2)
				{
						m_shader->uploadValue(_uniformName, _vec2);
				}

				void Material::uploadValue(const std::string & _uniformName, const glm::vec3 & _vec3)
				{
						m_shader->uploadValue(_uniformName, _vec3);
				}

				void Material::uploadValue(const std::string & _uniformName, const glm::vec4 & _vec4)
				{
						m_shader->uploadValue(_uniformName, _vec4);
				}

		}
}