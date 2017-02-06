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
#include <cogs\BulletDebugRenderer.h>
#include <cogs\FPSCameraControl.h>
#include <iostream>

#include "PaddleController.h"
#include "BallBehavior.h"

#define DEBUG_DRAW 0

namespace ce = cogs::ecs;
namespace cg = cogs::graphics;
namespace cu = cogs::utils;
namespace cp = cogs::physics;

int main(int argc, char** argv)
{
		cg::Window window;
		window.create("Test", 1024, 576, cg::WindowCreationFlags::RESIZABLE);
		window.setRelativeMouseMode(true);
		window.setClearColor(&cg::Color::white);
		bool quit = false;

		cu::FpsLimiter fpsLimiter(60.0f);

		std::shared_ptr<cp::Physics> physicsWorld = std::make_shared<cp::Physics>(0.0f, -9.81f, 0.0f);

		std::shared_ptr<ce::Entity> root = ce::Entity::create("Root");

		std::shared_ptr<cg::Framebuffer> test = cg::Framebuffer::create(window.getWidth(), window.getHeight());

		std::weak_ptr<ce::Entity> mainCamera = root->addChild("MainCamera");
		mainCamera.lock()->addComponent<ce::Camera>(window.getWidth(), window.getHeight(), ce::ProjectionType::PERSPECTIVE);
		mainCamera.lock()->addComponent<ce::FPSCameraControl>(1.0f);
		mainCamera.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, 0.0f, 55.0f));
		//mainCamera.lock()->getComponent<ce::Camera>().lock()->setRenderTarget(test);

		/*std::weak_ptr<ce::Entity> camera2 = root->addChild("Camera2");
		camera2.lock()->addComponent<ce::Camera>(ce::ProjectionType::PERSPECTIVE, window.getWidth(), window.getHeight());
		camera2.lock()->getComponent<ce::Transform>()->translate(glm::vec3(0.0f, 0.0f, 25.0f));*/

		std::weak_ptr<ce::Entity> paddle = root->addChild("PlayerPaddle");
		paddle.lock()->addComponent<ce::MeshRenderer>(std::make_unique<cg::Model>("paddle", "Models/TestModels/cube.obj"),
				std::make_unique<cg::Material>("paddle_mtl", cg::GLSLProgram("Basic3DShader", "Shaders/Basic3DShader.vert", "Shaders/Basic3DShader.frag")));
		paddle.lock()->getComponent<ce::Transform>().lock()->setWorldScale(glm::vec3(2.0f, 0.5f, 1.0f));
		paddle.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, -4.0f, 0.0f));
		paddle.lock()->addComponent<ce::Collider>(ce::ColliderShape::BOX, 1.0f, glm::vec3(2.0f, 0.5f, 1.0f));
		paddle.lock()->addComponent<ce::RigidBody>(physicsWorld, 1.0f);
		paddle.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(4);
		paddle.lock()->getComponent<ce::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 0.0f, 0.0f));
		paddle.lock()->getComponent<ce::RigidBody>().lock()->setAngularFactor(glm::vec3(0.0f, 0.0f, 0.0f));
		paddle.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
		paddle.lock()->addComponent<PaddleController>(1.0f);

		std::weak_ptr<ce::Entity> groundBound = root->addChild("GroundBoundary");
		groundBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(-2.5f, -5.0f, 0.0f));
		groundBound.lock()->addComponent<ce::Collider>(ce::ColliderShape::BOX, 1.0f, glm::vec3(32.5f, 0.0f, 1.0f));
		groundBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);

		std::weak_ptr<ce::Entity> ceilingBound = root->addChild("CeilingBoundary");
		ceilingBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(-2.5f, 30.0f, 0.0f));
		ceilingBound.lock()->addComponent<ce::Collider>(ce::ColliderShape::BOX, 1.0f, glm::vec3(32.5f, 0.0f, 1.0f));
		ceilingBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
		ceilingBound.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<ce::Entity> leftBound = root->addChild("LeftBoundary");
		leftBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(-35.0f, 12.5f, 0.0f));
		leftBound.lock()->addComponent<ce::Collider>(ce::ColliderShape::BOX, 1.0f, glm::vec3(0.0f, 17.0f, 1.0f));
		leftBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
		leftBound.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<ce::Entity> rightBound = root->addChild("RightBoundary");
		rightBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(30, 12.5f, 0.0f));
		rightBound.lock()->addComponent<ce::Collider>(ce::ColliderShape::BOX, 1.0f, glm::vec3(0.0f, 17.0f, 1.0f));
		rightBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
		rightBound.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<ce::Entity> ball = root->addChild("Ball");
		ball.lock()->addComponent<ce::MeshRenderer>(std::make_unique<cg::Model>("ball", "Models/TestModels/sphere.obj"),
				std::make_unique<cg::Material>("ball_mtl", cg::GLSLProgram("Basic3DShader", "Shaders/Basic3DShader.vert", "Shaders/Basic3DShader.frag")));
		ball.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, 3.0f, 0.0f));
		ball.lock()->addComponent<ce::Collider>(ce::ColliderShape::SPHERE, 1.0);
		ball.lock()->addComponent<ce::RigidBody>(physicsWorld, 1.0f);
		ball.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(5);
		ball.lock()->getComponent<ce::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 1.0f, 0.0f));
		ball.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
		ball.lock()->addComponent<BallBehavior>();

		for (int i = -30; i < 30; i += 4)
		{
				for (int j = -10; j < 0; j += 4)
				{
						std::weak_ptr<ce::Entity> brick = root->addChild("Brick");
						brick.lock()->addComponent<ce::MeshRenderer>(std::make_unique<cg::Model>("brick", "Models/TestModels/cube.obj"),
								std::make_unique<cg::Material>("bricks_mtl", cg::GLSLProgram("Basic3DShader", "Shaders/Basic3DShader.vert", "Shaders/Basic3DShader.frag")));
						brick.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f + i, 30.0f + j, 0.0f));
						brick.lock()->addComponent<ce::Collider>(ce::ColliderShape::BOX, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
						brick.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
						brick.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(5);
						brick.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
				}
		}

		cg::BulletDebugRenderer debugRenderer;

		debugRenderer.setDebugMode(debugRenderer.DBG_DrawWireframe);
		physicsWorld->setDebugDrawer(&debugRenderer);

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
								cu::Input::setMouseCoords(static_cast<float>(evnt.motion.x), static_cast<float>(evnt.motion.y));
								cu::Input::setMouseMotion(static_cast<float>(evnt.motion.xrel), static_cast<float>(evnt.motion.yrel));
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
										//camera.lock()->getComponent<ce::Camera>()->resize(window.getWidth(), window.getHeight());
										window.resizeHandled();
								}
						}
						}
				}

				if (cu::Input::isKeyPressed(cu::KeyCode::ESC))
				{
						quit = true;
				}

				//Update
				root->refreshAll();
				root->updateAll(fpsLimiter.deltaTime());

				physicsWorld->stepSimulation();

				std::vector<std::weak_ptr<ce::Camera>> cameras = root->getComponentsInChildren<ce::Camera>();
				//Render
				for (std::weak_ptr<ce::Camera> camera : cameras)
				{
						cg::Framebuffer::setActive(camera.lock()->getRenderTarget());

						root->renderAll(camera);

						cg::Framebuffer::setActive(std::weak_ptr<cg::Framebuffer>());
#if  DEBUG_DRAW
						//use the debug renderer to draw the debug physics world
						physicsWorld->debugDrawWorld();
						debugRenderer.end();
						debugRenderer.render(camera->getViewMatrix(), camera->getProjectionMatrix(), 1.0f);
#endif //  DEBUG_DRAW
				}

				window.swapBuffer();

				fpsLimiter.endFrame();

				window.setWindowTitle("FPS: " + std::to_string(fpsLimiter.fps()) + " DT: " + std::to_string(fpsLimiter.deltaTime()));
		}
		window.close();
		return 0;
}