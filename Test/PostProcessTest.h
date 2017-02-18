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

class PostProcessTest : public ce::Component
{
public:
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
				m_quad = cu::ResourceManager::getPrimitive("Models/TestModels/ScreenQuad.obj");
		}

		/**
		* The update component function
		*/
		void update(float _deltaTime) override {}

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
				glBindTexture(GL_TEXTURE_2D, finalCam.lock()->getRenderTarget().lock()->getTextureID());
				m_quad.lock()->render();
				glEnable(GL_DEPTH_TEST);
				m_postProcessShader.lock()->unUse();
		}
private:
		std::weak_ptr<cg::Mesh> m_quad;
		std::weak_ptr<cg::GLSLProgram> m_postProcessShader;
};

#endif // !POSTPROCESS_H