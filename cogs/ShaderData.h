#ifndef SHADER_DATA_H
#define SHADER_DATA_H

#include <string>
#include <GL/glew.h>
#include <unordered_map>

namespace cogs
{
		namespace graphics
		{
				using AttribLocation = GLuint;
				using UniformLocation = GLuint;
				using ProgramID = GLuint;
				using ShaderID = GLuint;

				class ShaderData
				{
				public:
						ShaderData(ProgramID _programID, ShaderID _vertID, ShaderID _fragID, ShaderID _geoID = 0)
								: m_programID(_programID), m_vertexShaderID(_vertID), m_fragmentShaderID(_fragID), m_geometryShaderID(_geoID)
						{}

						~ShaderData()
						{
								dispose();
						}

						/** Dispose of the shader*/
						void dispose()
						{
								//deletes the program ID if there is one (not 0)
								if (m_programID != 0) 
								{
										glDeleteProgram(m_programID);
										m_programID = 0;
								}
						}

						ProgramID getProgramID() const noexcept { return m_programID; }

						/* Registers the location of an attribute in this shader (must be called after linking) */
						void registerAttribute(const std::string& _attrib)
						{
								m_attribList[_attrib] = getAttribLoc(_attrib);
						}

						/* Registers the location of a uniform in this shader (must be called after linking) */
						void registerUniform(const std::string& _uniform)
						{
								m_unifLocationList[_uniform] = getUniformLoc(_uniform);
						}

						//accesses elements : attributes/uniforms;
						AttribLocation getAttribLocation(const std::string& _attrib)
						{
								auto it = m_attribList.find(_attrib);
								if (it != m_attribList.end())
								{
										// Found the location
										return it->second;
								}
								else
								{
										// Didn't find the location
										// Try to register it if it just wasn't registered
										registerAttribute(_attrib);
										return m_attribList.at(_attrib);
								}
						}

						UniformLocation getUniformLocation(const std::string& _uniform)
						{
								auto it = m_unifLocationList.find(_uniform);
								if (it != m_unifLocationList.end())
								{
										// Found the location
										return it->second;
								}
								else
								{
										// Didn't find the location
										// Try to see if it just wasn't registered
										registerUniform(_uniform);
										return m_unifLocationList.at(_uniform);
								}
						}

				private:
						/**
						* \brief Gets the location of a specific attribute in the shader program
						* \param[in] _attributeName The name of the searched attribute
						* \return the GLint location of the specified attribute
						*/
						AttribLocation getAttribLoc(const std::string& _attributeName)
						{
								AttribLocation location = glGetAttribLocation(m_programID, _attributeName.c_str());
								//error check
								if (location == GL_INVALID_INDEX)
								{
										throw std::runtime_error("Attribute " + _attributeName + " not found in shader!");
								}
								//return it if successful
								return location;
						}

						/**
						* \brief Get the uniform location
						* \param[in] _uniformName The name of the requested uniform
						* \return output is the GLint location of the uniform in the shader
						*/
						UniformLocation getUniformLoc(const std::string& _uniformName)
						{
								//get the uniform location
								UniformLocation location = glGetUniformLocation(m_programID, _uniformName.c_str());
								//error check
								if (location == GL_INVALID_INDEX)
								{
										throw std::runtime_error("Uniform " + _uniformName + " not found in shader!");
								}
								//return it if successful
								return location;
						}

				private:
						/* The program ID of the whole shader program */
						ProgramID m_programID{ 0 };

						/* The Id's of each shader */
						ShaderID m_vertexShaderID{ 0 };
						ShaderID m_fragmentShaderID{ 0 };
						ShaderID m_geometryShaderID{ 0 };

						/* a map of the locations in the shader for ease of access */
						std::unordered_map<std::string, AttribLocation> m_attribList;
						std::unordered_map<std::string, UniformLocation> m_unifLocationList;
				};
		}
}
#endif // !SHADER_DATA_H
