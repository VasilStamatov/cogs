#include <cogs\Window.h>
#include <cogs\Camera.h>
#include <cogs\GLSLProgram.h>
#include <cogs\Sprite.h>
#include <cogs\Timing.h>
#include <cogs\GLTexture.h>
#include <cogs\MeshRenderer.h>
#include <cogs\KeyCode.h>
#include <cogs\Input.h>
#include <cogs\Random.h>
#include <cogs\Physics.h>
#include <cogs\Collider.h>
#include <cogs\RigidBody.h>
#include <cogs\FPSCameraControl.h>
#include <cogs\BulletDebugRenderer.h>
#include <iostream>

namespace ce = cogs::ecs;
namespace cg = cogs::graphics;
namespace cu = cogs::utils;
namespace cp = cogs::physics;

int main(int argc, char** argv)
{
		cp::Physics::init(0.0f, -9.81f, 0.0f);

		cg::Window window;
		window.create("Test", 1024, 576, cg::WindowCreationFlags::RESIZABLE);
		bool quit = false;

		cu::FpsLimiter fpsLimiter(60.0f);

		std::shared_ptr<ce::Entity> root = std::make_shared<ce::Entity>("Root");

		std::weak_ptr<ce::Entity> camera = root->addChild("Camera");
		camera.lock()->addComponent<ce::Camera>(ce::ProjectionType::PERSPECTIVE, window.getWidth(), window.getHeight());
		camera.lock()->addComponent<ce::FPSCameraControl>();
		camera.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 0.0f, 5.0f));

		std::weak_ptr<ce::Entity> model1 = root->addChild("TestModel");
		model1.lock()->addComponent<ce::MeshRenderer>(std::make_unique<cg::Model>("sphere", "Models/TestModels/sphere.obj"),
				std::make_unique<cg::Material>("sphere_mtl", cg::GLSLProgram("Basic3DShader", "Shaders/Basic3DShader.vert", "Shaders/Basic3DShader.frag")));
		model1.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 50.0f, 0.0f));
		model1.lock()->addComponent<ce::Collider>(ce::ColliderShape::SPHERE, 1.0f);
		model1.lock()->addComponent<ce::RigidBody>(1.0f);

		std::weak_ptr<ce::Entity> plane = root->addChild("StaticPlane");
		plane.lock()->addComponent<ce::MeshRenderer>(std::make_unique<cg::Model>("plane", "Models/TestModels/plane2.obj"),
				std::make_unique<cg::Material>("plane_mtl", cg::GLSLProgram("Basic3DShader", "Shaders/Basic3DShader.vert", "Shaders/Basic3DShader.frag")));
		plane.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, -5.0f, 0.0f));
		plane.lock()->addComponent<ce::Collider>(ce::ColliderShape::STATIC_PLANE);
		plane.lock()->addComponent<ce::RigidBody>(0.0f);


		/*cg::SpriteRenderer spriteRenderer("BasicShader", "Shaders/BasicShader.vert", "Shaders/BasicShader.frag");

		std::weak_ptr<ce::Entity> sprite = root->addChild("testSprite");
		sprite.lock()->addComponent<ce::Sprite>(glm::vec2(200.0f, 200.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				"Textures/img_test.png", true,
				&spriteRenderer);

		std::weak_ptr<ce::Entity> sprite2 = sprite.lock()->addChild("testSpriteChild");
		sprite2.lock()->addComponent<ce::Sprite>(glm::vec2(200.0f, 200.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				"Textures/player.png", true,
				&spriteRenderer);
		sprite2.lock()->getComponent<ce::Transform>()->setLocalPosition(glm::vec3(300.0f, 300.0f, 0.0f));

		std::weak_ptr<ce::Entity> sprite3 = sprite2.lock()->addChild("testSpriteChild2");
		sprite3.lock()->addComponent<ce::Sprite>(glm::vec2(200.0f, 200.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				"Textures/red_bricks.png", true,
				&spriteRenderer);
		sprite3.lock()->getComponent<ce::Transform>()->setLocalPosition(glm::vec3(300.0f, 300.0f, 0.0f));*/

		window.setClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		cg::BulletDebugRenderer debugRenderer;

		debugRenderer.setDebugMode(debugRenderer.DBG_DrawWireframe);
		cp::Physics::setDebugDrawer(&debugRenderer);

		while (!quit)
		{
				fpsLimiter.beginFrame();
				window.clear(true, true);
				cu::Input::update();
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
										cu::Input::pressKey(evnt.key.keysym.sym);
										break;
								}
								case SDL_KEYUP:
								{
										cu::Input::releaseKey(evnt.key.keysym.sym);
										break;
								}
								case SDL_MOUSEBUTTONDOWN:
								{
										cu::Input::pressKey(evnt.button.button);
										break;
								}
								case SDL_MOUSEBUTTONUP:
								{
										cu::Input::releaseKey(evnt.button.button);
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

				if (cu::Input::isKeyPressed(cu::KeyCode::SPACE))
				{
						std::cout << "Pos x: " << model1.lock()->getComponent<ce::Transform>()->localPosition().x << std::endl;
						std::cout << "Pos y: " << model1.lock()->getComponent<ce::Transform>()->localPosition().y << std::endl;
						std::cout << "Pos z: " << model1.lock()->getComponent<ce::Transform>()->localPosition().z << std::endl;

						std::cout << "Scale x: " << model1.lock()->getComponent<ce::Transform>()->localScale().x << std::endl;
						std::cout << "Scale y: " << model1.lock()->getComponent<ce::Transform>()->localScale().z << std::endl;
						std::cout << "Scale z: " << model1.lock()->getComponent<ce::Transform>()->localScale().y << std::endl;

						std::cout << "Rotation x: " << model1.lock()->getComponent<ce::Transform>()->localOrientation().x << std::endl;
						std::cout << "Rotation y: " << model1.lock()->getComponent<ce::Transform>()->localOrientation().y << std::endl;
						std::cout << "Rotation z: " << model1.lock()->getComponent<ce::Transform>()->localOrientation().z << std::endl;

						model1.lock()->getComponent<ce::RigidBody>()->applyCentralForce(glm::vec3(0.0f, 25.0f, 0.0f));
				}

				if (cu::Input::isKeyDown(cu::KeyCode::ALPHA1))
				{
						std::cout << "Camera is now perspective projection" << std::endl;
						camera.lock()->getComponent<ce::Camera>()->setProjectionType(ce::ProjectionType::PERSPECTIVE);
				}
				if (cu::Input::isKeyDown(cu::KeyCode::ALPHA2))
				{
						std::cout << "Camera is now orthographic projection" << std::endl;
						camera.lock()->getComponent<ce::Camera>()->setProjectionType(ce::ProjectionType::ORTHOGRAPHIC);
				}
				if (cu::Input::isKeyDown(cu::KeyCode::Q))
				{
						plane.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, -0.1f, 0.0f));
				}
				if (cu::Input::isKeyDown(cu::KeyCode::E))
				{
						plane.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 0.1f, 0.0f));
				}
				if (cu::Input::isKeyDown(cu::KeyCode::R))
				{
						model1.lock()->getComponent<ce::Transform>()->rotate(glm::vec3(10.0f, 0.0f, 0.0f));
				}
				if (cu::Input::isKeyDown(cu::KeyCode::F))
				{
						model1.lock()->getComponent<ce::Transform>()->rotate(glm::vec3(10.0f, 10.0f, 10.0f));
				}
				if (cu::Input::isKeyDown(cu::KeyCode::LEFT))
				{
						model1.lock()->getComponent<ce::RigidBody>()->applyCentralForce(glm::vec3(-25.0f, 0.0f, 0.0f));
				}
				if (cu::Input::isKeyDown(cu::KeyCode::RIGHT))
				{
						model1.lock()->getComponent<ce::RigidBody>()->applyCentralForce(glm::vec3(25.0f, 0.0f, 0.0f));
				}
				if (cu::Input::isKeyDown(cu::KeyCode::UP))
				{
						model1.lock()->getComponent<ce::RigidBody>()->applyCentralForce(glm::vec3(0.0f, 0.0f, -25.0f));
				}
				if (cu::Input::isKeyDown(cu::KeyCode::DOWN))
				{
						model1.lock()->getComponent<ce::RigidBody>()->applyCentralForce(glm::vec3(0.0f, -0.0f, 25.0f));
				}
				if (cu::Input::isKeyDown(cu::KeyCode::Z))
				{
						std::cout << "Increasing the FoV of the camera by 5" << std::endl;
						camera.lock()->getComponent<ce::Camera>()->offsetFoV(5);
				}
				if (cu::Input::isKeyDown(cu::KeyCode::X))
				{
						std::cout << "Increasing the FoV of the camera by -5" << std::endl;
						camera.lock()->getComponent<ce::Camera>()->offsetFoV(-5);
				}


				//Update
				root->refreshAll();
				root->updateAll(fpsLimiter.deltaTime());

				cp::Physics::stepSimulation();

				//Render
				//spriteRenderer.begin();
				root->renderAll();

				//use the debug renderer to draw the debug physics world
				cp::Physics::debugDrawWorld();
				debugRenderer.end();
				debugRenderer.render(camera.lock()->getComponent<ce::Camera>()->getViewMatrix(),
						camera.lock()->getComponent<ce::Camera>()->getProjectionMatrix(), 1.0f);

				//spriteRenderer.end();

				//spriteRenderer.flush(camera.lock()->getComponent<ce::Camera>()->getViewMatrix(), camera.lock()->getComponent<ce::Camera>()->getProjectionMatrix());

				window.swapBuffer();

				fpsLimiter.endFrame();

				window.setWindowTitle("FPS: " + std::to_string(fpsLimiter.fps()) + " DT: " + std::to_string(fpsLimiter.deltaTime()));
		}
		window.close();
		return 0;
}