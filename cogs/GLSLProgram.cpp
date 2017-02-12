#include "GLSLProgram.h"
#include "IOManager.h"

#include <fstream>
#include <glm\gtc\type_ptr.hpp>

namespace cogs
{
		namespace graphics
		{
				std::map<std::string, std::shared_ptr<ShaderData>> GLSLProgram::s_resourceMap;

				//inoitialize all the variables to 0
				GLSLProgram::GLSLProgram()
				{

				}

				GLSLProgram::GLSLProgram(const std::string& _name, const std::string & _vsFilePath, const std::string & _fsFilePath, const std::string & _gsFilePath)
				{
						compileShaders(_name, _vsFilePath, _fsFilePath, _gsFilePath);
				}

				GLSLProgram::GLSLProgram(const GLSLProgram & _other) :
						m_programName(_other.m_programName), m_shaderData(_other.m_shaderData)
				{
				}

				GLSLProgram::~GLSLProgram()
				{
						dispose();
				}

				//Compiles the shaders into a form that your GPU can understand
				void GLSLProgram::compileShaders(const std::string& _name, const std::string& _vsFilePath, const std::string& _fsFilePath, const std::string& _gsFilePath/* = ""*/)
				{
						auto iter = s_resourceMap.find(_name);

						//check if it's not in the map
						if (iter == s_resourceMap.end())
						{
								m_programName = _name;
								std::string vsSource;
								std::string fsSource;
								std::string gsSource;

								utils::IOManager::readFileToBuffer(_vsFilePath, vsSource);
								utils::IOManager::readFileToBuffer(_fsFilePath, fsSource);
								if (_gsFilePath != "")
								{
										utils::IOManager::readFileToBuffer(_gsFilePath, gsSource);
										compileShadersFromSource(vsSource.c_str(), fsSource.c_str(), gsSource.c_str());
								}
								else
								{
										compileShadersFromSource(vsSource.c_str(), fsSource.c_str());
								}
						}
						else
						{
								m_programName = _name;
								m_shaderData = iter->second;
						}
				}

				void GLSLProgram::compileShadersFromSource(const char* _vertexSource, const char* _fragmentSource, const char* _geometrySource /*= nullptr*/)
				{
						//Create the GLSL program ID
						ProgramID programID = glCreateProgram();

						//Create the vertex shader object, and store its ID
						ShaderID vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
						if (vertexShaderID == 0)
						{
								throw std::runtime_error("Vertex shader failed to be created");
						}

						//Create the fragment shader object, and store its ID
						ShaderID fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
						if (fragmentShaderID == 0)
						{
								throw std::runtime_error("Fragment shader failed to be created");
						}

						ShaderID geometryShaderID{ 0 };
						//Check if there is a geometry shader
						if (_geometrySource != nullptr)
						{
								//Create the geometry shader object, and store its ID
							 geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
								if (geometryShaderID == 0)
								{
										throw std::runtime_error("Geometry shader failed to be created");
								}
						}

						//Compile each shader
						compileShader(_vertexSource, "Vertex Shader", vertexShaderID);
						if (_geometrySource != nullptr)
						{
								compileShader(_geometrySource, "Geometry Shader", geometryShaderID);
						}
						compileShader(_fragmentSource, "Fragment Shader", fragmentShaderID);

						linkShaders(programID, vertexShaderID, fragmentShaderID, geometryShaderID);

						std::shared_ptr<ShaderData> newShaderData = std::make_shared<ShaderData>(programID, vertexShaderID, fragmentShaderID, geometryShaderID);
						s_resourceMap.insert(std::make_pair(m_programName, std::move(newShaderData)));
						m_shaderData = s_resourceMap.at(m_programName);
				}

				void GLSLProgram::linkShaders(ProgramID _programID, ShaderID _vertID, ShaderID _fragID, ShaderID _geoID /* = 0 */)
				{
						//Attach our shaders to our program
						glAttachShader(_programID, _vertID);
						glAttachShader(_programID, _fragID);
						if (_geoID != 0)
						{
								glAttachShader(_programID, _geoID);
						}
						//link our program
						glLinkProgram(_programID);

						//note the different functions here: glGetProgram* instead of glGetShader*
						GLint isLinked = 0;
						glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);

						if (isLinked == GL_FALSE)
						{
								GLint maxLength = 0;
								//get the size of the string (maxlength)
								glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

								//The maxLength includes the null char
								std::vector<char> errorLog(maxLength);
								glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

								//we don't need this program anymore
								glDeleteProgram(_programID);

								//dont leak shaders either
								//Don't leak shaders either.
								glDeleteShader(_vertID);
								glDeleteShader(_fragID);
								glDeleteShader(_geoID);

								//print the error log and quit
								std::printf("%s\n", &errorLog[0]);
								throw std::runtime_error("Shaders failed to link!");
						}
						//Always detach shaders after a successful link.
						glDetachShader(_programID, _vertID);
						glDetachShader(_programID, _fragID);
						glDetachShader(_programID, _geoID);
						glDeleteShader(_vertID);
						glDeleteShader(_fragID);
						glDeleteShader(_geoID);
				}

				GLuint GLSLProgram::getUniformBlockIndex(const std::string& _uniformBlockName)
				{
						GLuint index = glGetUniformBlockIndex(m_shaderData.lock()->getProgramID(), _uniformBlockName.c_str());

						//error check
						if (index == GL_INVALID_INDEX)
						{
								throw std::runtime_error("Uniform block " + _uniformBlockName + " not found in shader!");
						}
						//return if successful
						return index;
				}

				void GLSLProgram::getUniformBlockDataSize(GLuint _index, GLint* _params)
				{
						glGetActiveUniformBlockiv(m_shaderData.lock()->getProgramID(), _index, GL_UNIFORM_BLOCK_DATA_SIZE, _params);
				}

				void GLSLProgram::bindBufferRange(GLenum _target, GLuint _index, GLuint _buffer, GLintptr _offset, GLsizeiptr _size)
				{
						glBindBufferRange(_target, _index, _buffer, _offset, _size);
						if (glGetError() == GL_INVALID_VALUE)
						{
								throw std::runtime_error("Unable to bind the buffer, check the redbook for possible causes (p.64)!");
						}
				}

				void GLSLProgram::blockUniformBinding(GLuint _uniformBlockIndex, GLuint _uniformBlockBinding)
				{
						glUniformBlockBinding(m_shaderData.lock()->getProgramID(), _uniformBlockIndex, _uniformBlockBinding);
				}

				void GLSLProgram::getActiveUniformsIndexValues(GLsizei _numUniforms, GLuint * _uniformIndices, GLenum _pname, GLint * _attribute)
				{
						glGetActiveUniformsiv(m_shaderData.lock()->getProgramID(), _numUniforms, _uniformIndices, _pname, _attribute);
				}

				void GLSLProgram::getUniformIndices(GLsizei _uniformCount, const char ** _uniformNames, GLuint * _uniformIndices)
				{
						glGetUniformIndices(m_shaderData.lock()->getProgramID(), _uniformCount, _uniformNames, _uniformIndices);
				}

				GLint GLSLProgram::getSubroutineUniformLocation(GLenum _shaderType, const std::string& _name)
				{
						GLint location = glGetSubroutineUniformLocation(m_shaderData.lock()->getProgramID(), _shaderType, _name.c_str());
						//error check
						if (location == GL_INVALID_INDEX)
						{
								throw std::runtime_error("Uniform subroutine" + _name + " not found in shader!");
						}
						//return it if successful
						return location;
				}

				GLuint GLSLProgram::getSubroutineIndex(GLenum _shaderType, const std::string& _name)
				{
						GLuint index = glGetSubroutineIndex(m_shaderData.lock()->getProgramID(), _shaderType, _name.c_str());
						//error check
						if (index == GL_INVALID_INDEX)
						{
								throw std::runtime_error("Subroutine index " + _name + " not found in subroutine uniform!");
						}
						//return if successful
						return index;
				}

				void GLSLProgram::uniformSubroutinesuiv(GLenum _shaderType, GLsizei _numSubrUniforms, const GLuint * _indices)
				{
						glUniformSubroutinesuiv(_shaderType, _numSubrUniforms, _indices);

						if (glGetError() == GL_INVALID_VALUE)
						{
								throw std::runtime_error("Invalid value generated for index!! Go see p.80 of OGL-R");
						}
				}

				//enable the shader
				void GLSLProgram::use()  const
				{
						glUseProgram(m_shaderData.lock()->getProgramID());
				}

				//disable the shader
				void GLSLProgram::unUse() const
				{
						glUseProgram(0);
				}

				void GLSLProgram::dispose()
				{
								m_shaderData.reset();
				}

				void GLSLProgram::registerAttribute(const std::string& _attrib)
				{
						m_shaderData.lock()->registerAttribute(_attrib);
				}

				void GLSLProgram::registerUniform(const std::string& _uniform)
				{
						m_shaderData.lock()->registerUniform(_uniform);
				}

				AttribLocation GLSLProgram::getAttribLocation(const std::string& _attrib)
				{
						return m_shaderData.lock()->getAttribLocation(_attrib);
				}

				UniformLocation GLSLProgram::getUniformLocation(const std::string& _uniform)
				{
						return m_shaderData.lock()->getUniformLocation(_uniform);
				}

				void GLSLProgram::uploadValue(const std::string & _uniformName, const glm::mat4 & _matrix)
				{
						glUniformMatrix4fv(getUniformLocation(_uniformName), 1, GL_FALSE, glm::value_ptr(_matrix));
				}

				void GLSLProgram::uploadValue(const std::string & _uniformName, const float & _float)
				{
						glUniform1f(getUniformLocation(_uniformName), _float);
				}

				void GLSLProgram::uploadValue(const std::string & _uniformName, const int & _int)
				{
						glUniform1i(getUniformLocation(_uniformName), _int);
				}

				void GLSLProgram::uploadValue(const std::string & _uniformName, const glm::vec2 & _vec2)
				{
						glUniform2fv(getUniformLocation(_uniformName), 1, glm::value_ptr(_vec2));
				}

				void GLSLProgram::uploadValue(const std::string & _uniformName, const glm::vec3 & _vec3)
				{
						glUniform3fv(getUniformLocation(_uniformName), 1, glm::value_ptr(_vec3));
				}

				void GLSLProgram::uploadValue(const std::string & _uniformName, const glm::vec4 & _vec4)
				{
						glUniform4fv(getUniformLocation(_uniformName), 1, glm::value_ptr(_vec4));
				}

				//Compiles a single shader file
				void GLSLProgram::compileShader(const char* _source, const std::string& _name, GLuint _id)
				{
						//tell opengl that we want to use fileContents as the contents of the shader file
						glShaderSource(_id, 1, &_source, nullptr);

						//compile the shader
						glCompileShader(_id);

						//check for errors
						GLint success = 0;
						glGetShaderiv(_id, GL_COMPILE_STATUS, &success);

						if (success == GL_FALSE)
						{
								GLint maxLength = 0;
								glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &maxLength);

								//The maxLength includes the NULL character
								std::vector<char> errorLog(maxLength);
								glGetShaderInfoLog(_id, maxLength, &maxLength, &errorLog[0]);

								//Provide the infolog in whatever manor you deem best.
								//Exit with failure.
								glDeleteShader(_id); //Don't leak the shader.

																													//Print error log and quit
								std::printf("%s\n", &(errorLog[0]));
								throw std::runtime_error("Shader " + _name + " failed to compile");
						}
				}
		}
}