#include <cogs\Window.h>
#include <cogs\Camera.h>
#include <cogs\GLSLProgram.h>
#include <cogs\Sprite.h>
#include <iostream>

namespace ce = cogs::ecs;
namespace cg = cogs::graphics;

int main(int argc, char** argv)
{
		cg::Window window;
		window.create("Test", 1024, 576, cg::WindowCreationFlags::RESIZABLE);
		bool quit = false;

		std::shared_ptr<ce::Entity> root = std::make_shared<ce::Entity>("Root");
		std::weak_ptr<ce::Entity> camera = root->addChild("Camera");
		camera.lock()->addComponent<ce::Camera>(ce::ProjectionType::PERSPECTIVE, window.getWidth(), window.getHeight());
		camera.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 0.0f, -5.0f));

		cg::SpriteRenderer spriteRenderer;

		std::weak_ptr<ce::Entity> sprite = root->addChild("testSprite");
		sprite.lock()->addComponent<ce::Sprite>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), &spriteRenderer);
		sprite.lock()->getComponent<ce::Transform>()->setLocalScale(glm::vec3(200.0f));

		std::weak_ptr<ce::Entity> sprite2 = sprite.lock()->addChild("testSpriteChild");
		sprite2.lock()->addComponent<ce::Sprite>(glm::vec4(0.5f, 1.0f, 0.5f, 1.0f), &spriteRenderer);
		sprite2.lock()->getComponent<ce::Transform>()->setLocalPosition(glm::vec3(-300.0f, 300.0f, 0.0f));

		cg::GLSLProgram basicShader;
		basicShader.compileShaders("BasicShader.vert", "BasicShader.frag");

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		while (!quit)
		{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
								if (evnt.key.keysym.sym == SDLK_SPACE)
								{
										std::cout << "Setting sprite world pos to -500, 500" << std::endl;
										sprite2.lock()->getComponent<ce::Transform>()->setWorldPosition(glm::vec3(-500.0f, 500.0f, 0.0f));
										std::cout << "Sprite transform x: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().x << std::endl;
										std::cout << "Sprite transform y: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().y << std::endl;
										std::cout << "Sprite transform z: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().z << std::endl;
								}
								if (evnt.key.keysym.sym == SDLK_w)
								{
										std::cout << "Translating the child sprite by 50.1f to the y" << std::endl;
										sprite2.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 50.1f, 0.0f));
										std::cout << "Sprite transform x: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().x << std::endl;
										std::cout << "Sprite transform y: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().y << std::endl;
										std::cout << "Sprite transform z: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().z << std::endl;
								}
								if (evnt.key.keysym.sym == SDLK_s)
								{
										std::cout << "Translating the child sprite by -50.1f to the y" << std::endl;
										sprite2.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, -50.1f, 0.0f));
										std::cout << "Sprite transform x: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().x << std::endl;
										std::cout << "Sprite transform y: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().y << std::endl;
										std::cout << "Sprite transform z: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().z << std::endl;
								}
								if (evnt.key.keysym.sym == SDLK_a)
								{
										std::cout << "Translating the child sprite by 50.1f to the x" << std::endl;
										sprite2.lock()->getComponent<ce::Transform>()->translate(glm::vec3(50.1f, 0.0f, 0.0f));
										std::cout << "Sprite transform x: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().x << std::endl;
										std::cout << "Sprite transform y: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().y << std::endl;
										std::cout << "Sprite transform z: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().z << std::endl;
								}
								if (evnt.key.keysym.sym == SDLK_d)
								{
										std::cout << "Translating the child sprite by -50.1f to the x" << std::endl;
										sprite2.lock()->getComponent<ce::Transform>()->translate(glm::vec3(-50.1f, 0.0f, 0.0f));
										std::cout << "Sprite transform x: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().x << std::endl;
										std::cout << "Sprite transform y: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().y << std::endl;
										std::cout << "Sprite transform z: " << sprite2.lock()->getComponent<ce::Transform>()->worldPosition().z << std::endl;
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
										std::cout << "Rotating the sprite by 1 degree on the z axis" << std::endl;
										sprite.lock()->getComponent<ce::Transform>()->rotate(glm::vec3(0.0f, 0.0f, 1.0f));
										std::cout << "sprite orientation x: " << sprite.lock()->getComponent<ce::Transform>()->localOrientation().x << std::endl;
										std::cout << "sprite orientation y: " << sprite.lock()->getComponent<ce::Transform>()->localOrientation().y << std::endl;
										std::cout << "sprite orientation z: " << sprite.lock()->getComponent<ce::Transform>()->localOrientation().z << std::endl;
								}
								if (evnt.key.keysym.sym == SDLK_e)
								{
										std::cout << "Rotating the camera by -1 degree on the z axis" << std::endl;
										sprite.lock()->getComponent<ce::Transform>()->rotate(glm::vec3(0.0f, 0.0f, -1.0f));
										std::cout << "sprite orientation x: " << sprite.lock()->getComponent<ce::Transform>()->localOrientation().x << std::endl;
										std::cout << "sprite orientation y: " << sprite.lock()->getComponent<ce::Transform>()->localOrientation().y << std::endl;
										std::cout << "sprite orientation z: " << sprite.lock()->getComponent<ce::Transform>()->localOrientation().z << std::endl;
								}
								if (evnt.key.keysym.sym == SDLK_LEFT)
								{
										std::cout << "Translating the sprite by 50.1f to the x" << std::endl;
										sprite.lock()->getComponent<ce::Transform>()->translate(glm::vec3(50.1f, 0.0f, 0.0f));
										std::cout << "Sprite transform x: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().x << std::endl;
										std::cout << "Sprite transform y: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().y << std::endl;
										std::cout << "Sprite transform z: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().z << std::endl;
								}
								if (evnt.key.keysym.sym == SDLK_RIGHT)
								{
										std::cout << "Translating the sprite by -50.1f to the x" << std::endl;
										sprite.lock()->getComponent<ce::Transform>()->translate(glm::vec3(-50.1f, 0.0f, 0.0f));
										std::cout << "Sprite transform x: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().x << std::endl;
										std::cout << "Sprite transform y: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().y << std::endl;
										std::cout << "Sprite transform z: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().z << std::endl;
								}
								if (evnt.key.keysym.sym == SDLK_UP)
								{
										std::cout << "Translating the sprite by 50.1f to the y" << std::endl;
										sprite.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 50.1f, 0.0f));
										std::cout << "Sprite transform x: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().x << std::endl;
										std::cout << "Sprite transform y: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().y << std::endl;
										std::cout << "Sprite transform z: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().z << std::endl;
								}
								if (evnt.key.keysym.sym == SDLK_DOWN)
								{
										std::cout << "Translating the sprite by -50.1f to the y" << std::endl;
										sprite.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, -50.1f, 0.0f));
										std::cout << "Sprite transform x: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().x << std::endl;
										std::cout << "Sprite transform y: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().y << std::endl;
										std::cout << "Sprite transform z: " << sprite.lock()->getComponent<ce::Transform>()->worldPosition().z << std::endl;
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
						}
						}
				}

				//Update
				root->refreshAll();
				root->updateAll(1.0f);

				//Render
				basicShader.use();
				basicShader.uploadValue("projection", camera.lock()->getComponent<ce::Camera>()->getProjectionMatrix());
				basicShader.uploadValue("view", camera.lock()->getComponent<ce::Camera>()->getViewMatrix());

				spriteRenderer.begin();
				root->renderAll();
				spriteRenderer.end();
				spriteRenderer.flush();

				basicShader.unUse();

				window.swapBuffer();
		}
		window.close();
		return 0;
}