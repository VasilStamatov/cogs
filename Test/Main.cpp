#include <cogs\Window.h>
#include <cogs\Camera.h>
#include <cogs\Timing.h>
#include <cogs\MeshRenderer.h>
#include <cogs\KeyCode.h>
#include <cogs\Input.h>
#include <cogs\Random.h>
#include <cogs\Physics.h>
#include <cogs\BoxCollider.h>
#include <cogs\SphereCollider.h>
#include <cogs\RigidBody.h>
#include <cogs\BulletDebugRenderer.h>
#include <cogs\FPSCameraControl.h>
#include <cogs\Entity.h>
#include <cogs\SpriteRenderer.h>
#include <cogs\ResourceManager.h>
#include <cogs\Utils.h>
#include <iostream>

#include "PaddleController.h"
#include "BallBehavior.h"
#include "PostProcessTest.h"

#define DEBUG_DRAW 0

namespace ce = cogs::ecs;
namespace cg = cogs::graphics;
namespace cu = cogs::utils;
namespace cp = cogs::physics;

int main(int argc, char** argv)
{
		cg::Window window;
		window.create("Test", 1024, 576, cg::WindowCreationFlags::NONE);
		window.setRelativeMouseMode(true);
		bool quit = false;

		cu::FpsLimiter fpsLimiter(60.0f);

		std::shared_ptr<cp::Physics> physicsWorld = std::make_shared<cp::Physics>(0.0f, -9.81f, 0.0f);

		std::shared_ptr<ce::Entity> root = ce::Entity::create("Root");

		std::shared_ptr<cg::Framebuffer> testFramebuffer = cg::Framebuffer::create(window.getWidth(), window.getHeight());

		std::vector<std::string> skyboxFilenames(6);
		skyboxFilenames.at(0) = "Textures/skybox/right.png";
		skyboxFilenames.at(1) = "Textures/skybox/left.png";
		skyboxFilenames.at(2) = "Textures/skybox/top.png";
		skyboxFilenames.at(3) = "Textures/skybox/bottom.png";
		skyboxFilenames.at(4) = "Textures/skybox/back.png";
		skyboxFilenames.at(5) = "Textures/skybox/front.png";

		std::shared_ptr<cg::Skybox> testSkybox = cg::Skybox::create(
				cu::ResourceManager::getGLSLProgram("SkyboxShader", "Shaders/Skybox.vert", "Shaders/Skybox.frag"),
				cu::ResourceManager::getGLCubemap("skyboxTexture", skyboxFilenames), true);

		std::weak_ptr<ce::Entity> mainCamera = root->addChild("MainCamera");
		mainCamera.lock()->addComponent<ce::Camera>(window.getWidth(), window.getHeight(), ce::ProjectionType::PERSPECTIVE);
		mainCamera.lock()->addComponent<ce::FPSCameraControl>(1.0f);
		mainCamera.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, 0.0f, 55.0f));
		mainCamera.lock()->getComponent<ce::Camera>().lock()->setSkybox(testSkybox);
		mainCamera.lock()->addComponent<ce::Light>();
		mainCamera.lock()->getComponent<ce::Light>().lock()->setLightType(ce::LightType::SPOT);
		mainCamera.lock()->getComponent<ce::Light>().lock()->setColor(glm::vec3(1.0f, 0.0f, 1.0f));
		mainCamera.lock()->getComponent<ce::Light>().lock()->setAmbientIntensity(0.0f);
		mainCamera.lock()->getComponent<ce::Light>().lock()->setDiffuseIntensity(1.0f);
		mainCamera.lock()->getComponent<ce::Light>().lock()->setSpecularIntensity(1.0f);
		mainCamera.lock()->getComponent<ce::Light>().lock()->setCutOff(glm::cos(glm::radians(12.5f)));
		mainCamera.lock()->getComponent<ce::Light>().lock()->setOuterCutOff(glm::cos(glm::radians(15.0f)));
		mainCamera.lock()->getComponent<ce::Light>().lock()->setAttenuation(ce::Attenuation(1.0f, 0.09f, 0.032f));
		mainCamera.lock()->getComponent<ce::Camera>().lock()->setRenderTarget(testFramebuffer);
		mainCamera.lock()->addComponent<PostProcessTest>(cu::ResourceManager::getGLSLProgram(
				"PostProcessShader", "Shaders/PostProcess.vert", "Shaders/PostProcess.frag"));

		/*std::weak_ptr<ce::Entity> camera2 = root->addChild("Camera2");
		camera2.lock()->addComponent<ce::Camera>(window.getWidth(), window.getHeight(), ce::ProjectionType::PERSPECTIVE);
		camera2.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, 0.0f, 25.0f));*/

		std::shared_ptr<cg::Renderer2D> renderer2D = std::make_shared<cg::Renderer2D>(
				cu::ResourceManager::getGLSLProgram("BasicShader", "Shaders/BasicShader.vert", "Shaders/BasicShader.frag"));

		std::shared_ptr<cg::Renderer3D> renderer3D = std::make_shared<cg::Renderer3D>(
				cu::ResourceManager::getGLSLProgram("Basic3DShader", "Shaders/Basic3DShader.vert", "Shaders/Basic3DShader.frag"));

		/*std::weak_ptr<ce::Entity> testSprite = root->addChild("testSprite");
		testSprite.lock()->addComponent<ce::SpriteRenderer>(
				cu::ResourceManager::getSprite("TestSprite",
						cu::ResourceManager::getGLTexture2D("Textures/img_test.png","texture_diffuse"),
						glm::vec2(10.0f, 10.0f), cg::Color::white), renderer2D);*/

		/*std::weak_ptr<ce::Entity> nanosuit = root->addChild("nanosuit");
		nanosuit.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(10.0f, 20.0f, 10.0f));
		cu::loadMeshesToEntity(nanosuit, "Models/nanosuit/nanosuit.obj", renderer3D);*/
		/*nanosuit.lock()->addComponent<ce::SphereCollider>(3.0f);
		nanosuit.lock()->addComponent<ce::RigidBody>(physicsWorld, 1.0f);
		nanosuit.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(4);
		nanosuit.lock()->getComponent<ce::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 0.0f, 0.0f));
		nanosuit.lock()->getComponent<ce::RigidBody>().lock()->setAngularFactor(glm::vec3(0.0f, 0.0f, 0.0f));
		nanosuit.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
		nanosuit.lock()->addComponent<PaddleController>(1.0f);*/

		std::weak_ptr<ce::Entity> directionalLight = root->addChild("PointLight");
		directionalLight.lock()->addComponent<ce::Light>();
		directionalLight.lock()->getComponent<ce::Light>().lock()->setLightType(ce::LightType::DIRECTIONAL);
		directionalLight.lock()->getComponent<ce::Light>().lock()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		directionalLight.lock()->getComponent<ce::Light>().lock()->setAmbientIntensity(0.05f);
		directionalLight.lock()->getComponent<ce::Light>().lock()->setDiffuseIntensity(0.4f);
		directionalLight.lock()->getComponent<ce::Light>().lock()->setSpecularIntensity(0.5f);
		directionalLight.lock()->getComponent<ce::Transform>().lock()->setLocalOrientation(glm::vec3(-0.2f, -1.0f, -0.3f));

		std::shared_ptr<ce::Entity> paddle = cu::loadEntityWithMeshes("Models/TestModels/cube.obj", renderer3D);
		paddle->setName("PlayerPaddle");
		std::weak_ptr<ce::Entity> pPaddle = root->addChild(std::move(paddle));
		pPaddle.lock()->getComponent<ce::Transform>().lock()->setWorldScale(glm::vec3(2.0f, 0.5f, 1.0f));
		pPaddle.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, -4.0f, 0.0f));
		pPaddle.lock()->addComponent<ce::BoxCollider>(glm::vec3(2.0f, 0.5f, 1.0f));
		pPaddle.lock()->addComponent<ce::RigidBody>(physicsWorld, 1.0f);
		pPaddle.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(4);
		pPaddle.lock()->getComponent<ce::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 0.0f, 0.0f));
		pPaddle.lock()->getComponent<ce::RigidBody>().lock()->setAngularFactor(glm::vec3(0.0f, 0.0f, 0.0f));
		pPaddle.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
		pPaddle.lock()->addComponent<PaddleController>(1.0f);

		std::weak_ptr<ce::Entity> groundBound = root->addChild("GroundBoundary");
		groundBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(-2.5f, -5.0f, 0.0f));
		groundBound.lock()->addComponent<ce::BoxCollider>(glm::vec3(32.5f, 0.0f, 1.0f));
		groundBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);

		std::weak_ptr<ce::Entity> ceilingBound = root->addChild("CeilingBoundary");
		ceilingBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(-2.5f, 30.0f, 0.0f));
		ceilingBound.lock()->addComponent<ce::BoxCollider>(glm::vec3(32.5f, 0.0f, 1.0f));
		ceilingBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
		ceilingBound.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<ce::Entity> leftBound = root->addChild("LeftBoundary");
		leftBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(-35.0f, 12.5f, 0.0f));
		leftBound.lock()->addComponent<ce::BoxCollider>(glm::vec3(0.0f, 17.0f, 1.0f));
		leftBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
		leftBound.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<ce::Entity> rightBound = root->addChild("RightBoundary");
		rightBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(30, 12.5f, 0.0f));
		rightBound.lock()->addComponent<ce::BoxCollider>(glm::vec3(0.0f, 17.0f, 1.0f));
		rightBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
		rightBound.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);

		std::shared_ptr<ce::Entity> ball = cu::loadEntityWithMeshes("Models/TestModels/sphere.obj", renderer3D);
		ball->setName("Ball");
		std::weak_ptr<ce::Entity> bBall = root->addChild(std::move(ball));
		bBall.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, 3.0f, 0.0f));
		bBall.lock()->addComponent<ce::SphereCollider>(1.0);
		bBall.lock()->addComponent<ce::RigidBody>(physicsWorld, 1.0f);
		bBall.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(5);
		bBall.lock()->getComponent<ce::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 1.0f, 0.0f));
		bBall.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
		bBall.lock()->addComponent<BallBehavior>();
		bBall.lock()->addComponent<ce::Light>();
		bBall.lock()->getComponent<ce::Light>().lock()->setLightType(ce::LightType::POINT);
		bBall.lock()->getComponent<ce::Light>().lock()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		bBall.lock()->getComponent<ce::Light>().lock()->setAmbientIntensity(0.05f);
		bBall.lock()->getComponent<ce::Light>().lock()->setDiffuseIntensity(0.8f);
		bBall.lock()->getComponent<ce::Light>().lock()->setSpecularIntensity(1.0f);
		bBall.lock()->getComponent<ce::Light>().lock()->setAttenuation(ce::Attenuation(1.0f, 0.09f, 0.032f));

		for (int i = -30; i < 30; i += 4)
		{
				for (int j = -10; j < 0; j += 4)
				{
						std::shared_ptr<ce::Entity> brick = cu::loadEntityWithMeshes("Models/TestModels/cube.obj", renderer3D);
						brick->setName("Brick");
						std::weak_ptr<ce::Entity> bBrick = root->addChild(std::move(brick));
						bBrick.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f + i, 30.0f + j, 0.0f));
						bBrick.lock()->addComponent<ce::BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f));
						bBrick.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
						bBrick.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(5);
						bBrick.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
				}
		}

		cg::BulletDebugRenderer debugRenderer;

		debugRenderer.setDebugMode(debugRenderer.DBG_DrawWireframe);
		physicsWorld->setDebugDrawer(&debugRenderer);

		while (!quit)
		{
				fpsLimiter.beginFrame();

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

				//Render

				//main camera
				/*std::weak_ptr<ce::Camera> mainCam = ce::Camera::getMain();

				assert(!mainCam.expired());

				cg::Framebuffer::setActive(mainCam.lock()->getRenderTarget());
				window.setClearColor(mainCam.lock()->getBackgroundColor());
				window.clear(true, true);

				root->renderAll(mainCam);*/

				//other cameras
				for (std::weak_ptr<ce::Camera> camera : ce::Camera::getAllCameras())
				{
						if (!camera.lock()->getEntity().lock()->isActive() || camera.expired())
						{
								continue;
						}
						// set the current camera
						ce::Camera::setCurrent(camera);

						// set the render target
						cg::Framebuffer::setActive(camera.lock()->getRenderTarget());

						// clear the window with the camera's background color
						window.setClearColor(camera.lock()->getBackgroundColor());
						window.clear(true, true);

						renderer2D->begin();
						renderer3D->begin();

						// call the render function of all entities (submits all entities with sprite and mesh renderers)
						root->renderAll();

						renderer2D->end();
						renderer3D->end();

						renderer2D->flush();
						renderer3D->flush();

#if  DEBUG_DRAW
						//use the debug renderer to draw the debug physics world
						physicsWorld->debugDrawWorld();
						debugRenderer.end();
						debugRenderer.render(camera.lock()->getViewMatrix(), camera.lock()->getProjectionMatrix(), 5.0f);
#endif //  DEBUG_DRAW

						//render the camera's skybox if it has one
						camera.lock()->renderSkybox();

						//set the render target to the default window
						cg::Framebuffer::setActive(std::weak_ptr<cg::Framebuffer>());
				}

				root->postProcessAll();

				window.swapBuffer();

				fpsLimiter.endFrame();

				window.setWindowTitle("FPS: " + std::to_string(fpsLimiter.fps()) + " DT: " + std::to_string(fpsLimiter.deltaTime()));
		}
		cu::ResourceManager::clear();
		window.close();
		return 0;
}