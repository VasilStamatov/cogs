#include "Skybox.h"
#include "ResourceManager.h"
#include "Camera.h"

#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>

namespace cogs
{
		namespace graphics
		{
				Skybox::Skybox(std::weak_ptr<GLSLProgram> _shader, std::weak_ptr<GLCubemapTexture> _cubemapTex, bool _isBox) :
						m_skyboxShader(_shader), m_cubemapTex(_cubemapTex)
				{
						if (_isBox)
						{
								m_model = utils::ResourceManager::getModel("SkyBox", "Models/TestModels/cube.obj");
						}
						else
						{
								m_model = utils::ResourceManager::getModel("SkySphere", "Models/TestModels/sphere.obj");
						}
				}

				Skybox::~Skybox()
				{
				}

				std::shared_ptr<Skybox> Skybox::create(std::weak_ptr<GLSLProgram> _shader,
						std::weak_ptr<GLCubemapTexture> _cubemapTex, bool _isBox)
				{
						std::shared_ptr<Skybox> newSkybox = std::make_shared<Skybox>(_shader, _cubemapTex, _isBox);

						return newSkybox;
				}

				void Skybox::render()
				{
						m_skyboxShader.lock()->use();

						GLint oldCullMode{ 0 };
						glGetIntegerv(GL_CULL_FACE_MODE, &oldCullMode);
						GLint oldDepthFunc{ 0 };
						glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);

						glCullFace(GL_FRONT);
						glDepthFunc(GL_LEQUAL);

						std::weak_ptr<ecs::Camera> currentCamera = ecs::Camera::getCurrent();

						const glm::mat4& view = glm::mat4(glm::mat3(currentCamera.lock()->getViewMatrix())); // Remove any translation component of the view matrix

						m_skyboxShader.lock()->uploadValue("view", view);
						m_skyboxShader.lock()->uploadValue("projection", currentCamera.lock()->getProjectionMatrix());
						m_skyboxShader.lock()->uploadValue("skybox", 0, m_cubemapTex);

						m_model.lock()->render();

						glCullFace(oldCullMode);
						glDepthFunc(oldDepthFunc);

						m_skyboxShader.lock()->unUse();
				}

		}
}