#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include <cogs\Component.h>
#include <cogs\ResourceManager.h>
#include <cogs\Camera.h>

#include <GL\glew.h>
#include <iostream>

namespace ce = cogs::ecs;
namespace cg = cogs::graphics;
namespace cu = cogs::utils;

/**
* \brief Primitive postprocessing component
* quite hardcoded at the moment but will be changed in the future
*/
class PostProcessTest : public ce::Component
{
public:
		/**
		* \brief PConstruct the component and set the shader
		*/
		PostProcessTest(std::weak_ptr<cg::GLSLProgram> _postProcessShader)
				: m_postProcessShader(_postProcessShader)
		{
		}
		~PostProcessTest() {}

		/**
		* The initialize component function
		*/
		void init() override 
		{
				m_quad = cu::ResourceManager::getMesh("Models/TestModels/ScreenQuad.obj");
		}

		/**
		* The update component function
		*/
		void update(float _deltaTime) override {}

		/**
		* \brief called after rendering has passed 
		* at the moment takes the final camera's render target
		* and uses its texture to draw a quad on the screen with it
		*/
		void postProcess() override
		{
				std::weak_ptr<ce::Camera> finalCam = ce::Camera::getCurrent();

				if (finalCam.lock()->getRenderTarget().expired())
				{
						return;
				}
				glClear(GL_COLOR_BUFFER_BIT);

				m_postProcessShader.lock()->use();
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_BLEND);
				glBindTexture(GL_TEXTURE_2D, finalCam.lock()->getRenderTarget().lock()->getTextureID());
				m_quad.lock()->render();
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				m_postProcessShader.lock()->unUse();
		}
private:
		std::weak_ptr<cg::Mesh> m_quad; ///< the screen quad 
		std::weak_ptr<cg::GLSLProgram> m_postProcessShader; ///< the postprocess shader
};

#endif // !POSTPROCESS_H