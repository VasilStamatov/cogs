#ifndef SKYBOX_H
#define SKYBOX_H

#include <memory>

namespace cogs
{
		namespace graphics
		{
				class GLCubemapTexture;
				class Mesh;
				class GLSLProgram;

				/**
				* \brief Skybox class to handle creation of a skybox and its rendering
				*/
				class Skybox
				{
				public:
						Skybox(std::weak_ptr<GLSLProgram> _shader, std::weak_ptr<GLCubemapTexture> _cubemapTex, bool _isBox);
						~Skybox();

						/**
						* \brief construct a shared pointer skybox and return it with move semantics
						* \param[in] _shader - the shader the skybox will be rendered with
						* \param[in] _cubemapTex - the shader the skybox will be rendered with
						* \param[in] _isBox - the flag whether the skybox should be rendered
						* as a box or sphere around the player
						*/
						static std::shared_ptr<Skybox> create(std::weak_ptr<GLSLProgram> _shader, std::weak_ptr<GLCubemapTexture> m_cubemapTex, bool _isBox);

						/** \brief renders the skybox */
						void render();

						/** \brief shader and texture setters */
						void setShader(std::weak_ptr<GLSLProgram> _shader) { m_skyboxShader = _shader; }
						void setCubemap(std::weak_ptr<GLCubemapTexture> _texture) { m_cubemapTex = _texture; }

				private:
						std::weak_ptr<GLSLProgram> m_skyboxShader; ///< the skybox shader
						std::weak_ptr<GLCubemapTexture> m_cubemapTex; ///< the cubemap texture
						std::weak_ptr<Mesh> m_mesh; ///< the box/sphere mesh
				};
		}
}

#endif // !SKYBOX_H