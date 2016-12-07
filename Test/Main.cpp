#include <cogs\Window.h>
#include <cogs\Camera.h>
#include <cogs\GLSLProgram.h>
#include <cogs\Sprite.h>
#include <cogs\Timing.h>

#include <iostream>

namespace ce = cogs::ecs;
namespace cg = cogs::graphics;
namespace cu = cogs::utils;

int main(int argc, char** argv)
{
		cg::Window window;
		window.create("Test", 1024, 576, cg::WindowCreationFlags::RESIZABLE);
		bool quit = false;

		cu::FpsLimiter fpsLimiter(60.0f);

		std::shared_ptr<ce::Entity> root = std::make_shared<ce::Entity>("Root");

		std::weak_ptr<ce::Entity> camera = root->addChild("Camera");
		camera.lock()->addComponent<ce::Camera>(ce::ProjectionType::ORTHOGRAPHIC, window.getWidth(), window.getHeight());
		camera.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 0.0f, 5.0f));
		camera.lock()->getComponent<ce::Transform>()->rotate(glm::vec3(0.0f, 0.0f, -0.0f));

		cg::SpriteRenderer spriteRenderer;

		std::weak_ptr<ce::Entity> sprite = root->addChild("testSprite");
		sprite.lock()->addComponent<ce::Sprite>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), &spriteRenderer);
		sprite.lock()->getComponent<ce::Transform>()->setLocalScale(glm::vec3(200.0f, 200.0f, 1.0f));

		std::weak_ptr<ce::Entity> sprite2 = sprite.lock()->addChild("testSpriteChild");
		sprite2.lock()->addComponent<ce::Sprite>(glm::vec4(0.5f, 1.0f, 0.5f, 1.0f), &spriteRenderer);
		sprite2.lock()->getComponent<ce::Transform>()->setLocalPosition(glm::vec3(300.0f, 300.0f, 0.0f));

		std::weak_ptr<ce::Entity> sprite3 = sprite2.lock()->addChild("testSpriteChild2");
		sprite3.lock()->addComponent<ce::Sprite>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), &spriteRenderer);
		sprite3.lock()->getComponent<ce::Transform>()->setLocalPosition(glm::vec3(300.0f, 300.0f, 0.0f));

		cg::GLSLProgram basicShader;
		basicShader.compileShaders("Shaders/BasicShader.vert", "Shaders/BasicShader.frag");

		window.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		while (!quit)
		{
				fpsLimiter.beginFrame();
				window.clear(true, true);

				//process input
				SDL_Event evnt;
				while (SDL_PollEvent(&evnt))
				{
						switch (evnt.type)
						{
						case SDL_QUIT:
						{
								quit = true;
								break;
						}
						case SDL_MOUSEMOTION:
						{
								break;
						}
						case SDL_KEYDOWN:
						{
							
								if (evnt.key.keysym.sym == SDLK_w)
								{
										sprite2.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 50.0f /* * fpsLimiter.deltaTIme() */, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_s)
								{
										sprite2.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, -50.0f /* * fpsLimiter.deltaTIme() */, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_a)
								{
										sprite2.lock()->getComponent<ce::Transform>()->translate(glm::vec3(-50.0f /* * fpsLimiter.deltaTIme() */, 0.0f, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_d)
								{
										sprite2.lock()->getComponent<ce::Transform>()->translate(glm::vec3(50.0f /* * fpsLimiter.deltaTIme() */, 0.0f, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_1)
								{
										std::cout << "Camera is now perspective projection" << std::endl;
										camera.lock()->getComponent<ce::Camera>()->setProjectionType(ce::ProjectionType::PERSPECTIVE);
								}
								if (evnt.key.keysym.sym == SDLK_2)
								{
										std::cout << "Camera is now orthographic projection" << std::endl;
										camera.lock()->getComponent<ce::Camera>()->setProjectionType(ce::ProjectionType::ORTHOGRAPHIC);
								}
								if (evnt.key.keysym.sym == SDLK_q)
								{
										sprite.lock()->getComponent<ce::Transform>()->rotate(glm::vec3(0.0f, 0.0f, 10.0f /* * fpsLimiter.deltaTIme() */));
								}
								if (evnt.key.keysym.sym == SDLK_e)
								{
										sprite.lock()->getComponent<ce::Transform>()->rotate(glm::vec3(0.0f, 0.0f, -10.0f /* * fpsLimiter.deltaTIme() */));
								}
								if (evnt.key.keysym.sym == SDLK_r)
								{
										sprite.lock()->getComponent<ce::Transform>()->scale(glm::vec3(10.0f /* * fpsLimiter.deltaTIme() */, 0.0f, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_f)
								{
										sprite.lock()->getComponent<ce::Transform>()->scale(glm::vec3(-10.0f /* * fpsLimiter.deltaTIme() */, 0.0f, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_LEFT)
								{
										sprite.lock()->getComponent<ce::Transform>()->translate(glm::vec3(-50.f  /* * fpsLimiter.deltaTIme() */, 0.0f, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_RIGHT)
								{
										sprite.lock()->getComponent<ce::Transform>()->translate(glm::vec3(50.0f /* * fpsLimiter.deltaTIme() */, 0.0f, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_UP)
								{
										sprite.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 50.0f /* * fpsLimiter.deltaTIme() */, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_DOWN)
								{
										sprite.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, -50.0f /* * fpsLimiter.deltaTIme() */, 0.0f));
								}
								if (evnt.key.keysym.sym == SDLK_z)
								{
										std::cout << "Increasing the size of the ortho camera by 0.5f" << std::endl;
										camera.lock()->getComponent<ce::Camera>()->offsetSize(0.5f);
								}
								if (evnt.key.keysym.sym == SDLK_x)
								{
										std::cout << "Increasing the size of the ortho camera by -0.5f" << std::endl;
										camera.lock()->getComponent<ce::Camera>()->offsetSize(-0.5f);
								}
								break;
						}
						case SDL_KEYUP:
						{
								break;
						}
						case SDL_MOUSEBUTTONDOWN:
						{
								break;
						}
						case SDL_MOUSEBUTTONUP:
						{
								break;
						}
						case SDL_MOUSEWHEEL:
						{
								break;
						}
						case SDL_WINDOWEVENT:
						{
								window.handleEvent(evnt);
								if (window.wasResized())
								{
										camera.lock()->getComponent<ce::Camera>()->resize(window.getWidth(), window.getHeight());
										window.resizeHandled();
								}
						}
						}
				}
				
				//Update
				root->refreshAll();
				root->updateAll(fpsLimiter.deltaTime());

				//Render
				spriteRenderer.begin();
				root->renderAll();
				spriteRenderer.end();

				basicShader.use();
				basicShader.uploadValue("projection", camera.lock()->getComponent<ce::Camera>()->getProjectionMatrix());
				basicShader.uploadValue("view", camera.lock()->getComponent<ce::Camera>()->getViewMatrix());

				spriteRenderer.flush();

				basicShader.unUse();

				window.swapBuffer();

				fpsLimiter.endFrame();

				window.setWindowTitle("FPS: " + std::to_string(fpsLimiter.fps()) + " DT: " + std::to_string(fpsLimiter.deltaTime()));
		}
		window.close();
		return 0;
}