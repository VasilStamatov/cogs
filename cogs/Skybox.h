#ifndef SKYBOX_H
#define SKYBOX_H

#include <memory>

namespace cogs
{
		namespace graphics
		{
				class GLCubemapTexture;
				class Model;
				class GLSLProgram;

				class Skybox
				{
				public:
						Skybox(std::weak_ptr<GLSLProgram> _shader, std::weak_ptr<GLCubemapTexture> _cubemapTex, bool _isBox);
						~Skybox();

						static std::shared_ptr<Skybox> create(std::weak_ptr<GLSLProgram> _shader, std::weak_ptr<GLCubemapTexture> m_cubemapTex, bool _isBox);

						void render();

				private:
						std::weak_ptr<GLSLProgram> m_skyboxShader;
						std::weak_ptr<GLCubemapTexture> m_cubemapTex;
						std::weak_ptr<Model> m_model;
				};
		}
}

#endif // !SKYBOX_H