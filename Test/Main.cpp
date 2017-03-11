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
#include <cogs\ParticleRenderer.h>
#include <cogs\Renderer2D.h>
#include <cogs\ParticleSystem.h>
#include <iostream>

#include "PaddleController.h"
#include "BallBehavior.h"
#include "PostProcessTest.h"

namespace ce = cogs::ecs;
namespace cg = cogs::graphics;
namespace cu = cogs::utils;
namespace cp = cogs::physics;

int main(int argc, char** argv)
{
		cg::Window window;
		window.create("Test", 1024, 576, cg::WindowCreationFlags::NONE);
		window.setRelativeMouseMode(true);
		bool quit{ false };
		bool debugMode{ false };
		cu::FpsLimiter fpsLimiter(600.0f);

		const glm::vec3 gravity(0.0f, -9.81f, 0.0f);

		std::shared_ptr<cp::Physics> physicsWorld = std::make_shared<cp::Physics>(gravity.x, gravity.y, gravity.z);

		std::shared_ptr<ce::Entity> root = ce::Entity::create("Root");

		std::shared_ptr<cg::Framebuffer> testFramebuffer = cg::Framebuffer::create(window.getWidth(), window.getHeight());

		std::vector<std::string> skyboxFilenames(6);
		skyboxFilenames.at(0) = "Textures/space/cwd_rt.jpg";
		skyboxFilenames.at(1) = "Textures/space/cwd_lf.jpg";
		skyboxFilenames.at(2) = "Textures/space/cwd_up.jpg";
		skyboxFilenames.at(3) = "Textures/space/cwd_dn.jpg";
		skyboxFilenames.at(4) = "Textures/space/cwd_bk.jpg";
		skyboxFilenames.at(5) = "Textures/space/cwd_ft.jpg";

		std::shared_ptr<cg::Skybox> testSkybox = cg::Skybox::create(
				cu::ResourceManager::getGLSLProgram("SkyboxShader", "Shaders/Skybox.vert", "Shaders/Skybox.frag"),
				cu::ResourceManager::getGLCubemap("skyboxTexture", skyboxFilenames), true);

		std::weak_ptr<ce::Entity> mainCamera = root->addChild("Camera");
		mainCamera.lock()->addComponent<ce::Camera>(window.getWidth(), window.getHeight(), ce::ProjectionType::PERSPECTIVE);
		mainCamera.lock()->getComponent<ce::Camera>().lock()->setBackgroundColor(cg::Color::black);
		mainCamera.lock()->addComponent<ce::FPSCameraControl>(50.0f);
		mainCamera.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, 0.0f, 55.0f));
		mainCamera.lock()->getComponent<ce::Camera>().lock()->setSkybox(testSkybox);
		mainCamera.lock()->addComponent<ce::Light>();
		mainCamera.lock()->getComponent<ce::Light>().lock()->setLightType(ce::LightType::SPOT);
		mainCamera.lock()->getComponent<ce::Light>().lock()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		mainCamera.lock()->getComponent<ce::Light>().lock()->setAmbientIntensity(0.0f);
		mainCamera.lock()->getComponent<ce::Light>().lock()->setDiffuseIntensity(1.0f);
		mainCamera.lock()->getComponent<ce::Light>().lock()->setSpecularIntensity(1.0f);
		mainCamera.lock()->getComponent<ce::Light>().lock()->setCutOff(glm::cos(glm::radians(12.5f)));
		mainCamera.lock()->getComponent<ce::Light>().lock()->setOuterCutOff(glm::cos(glm::radians(15.0f)));
		mainCamera.lock()->getComponent<ce::Light>().lock()->setAttenuation(ce::Attenuation(1.0f, 0.09f, 0.032f));
		//mainCamera.lock()->getComponent<ce::Camera>().lock()->setRenderTarget(testFramebuffer);
		/*mainCamera.lock()->addComponent<PostProcessTest>(cu::ResourceManager::getGLSLProgram(
				"PostProcessShader", "Shaders/PostProcess.vert", "Shaders/PostProcess.frag"));*/

		std::weak_ptr<ce::Entity> camera2 = root->addChild("MainCamera");
		camera2.lock()->addComponent<ce::Camera>(window.getWidth(), window.getHeight(), ce::ProjectionType::PERSPECTIVE);
		camera2.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, 0.0f, 55.0f));

		std::shared_ptr<cg::Renderer2D> renderer2D = std::make_shared<cg::Renderer2D>(
				cu::ResourceManager::getGLSLProgram("BasicShader", "Shaders/BasicShader.vert", "Shaders/BasicShader.frag"));

		std::shared_ptr<cg::Renderer3D> renderer3D = std::make_shared<cg::Renderer3D>(
				cu::ResourceManager::getGLSLProgram("Basic3DShader", "Shaders/Basic3DShader.vert", "Shaders/Basic3DShader.frag"));

		/*std::shared_ptr<cg::BillboardRenderer> billboardRenderer = std::make_shared<cg::BillboardRenderer>(
				cu::ResourceManager::getGLSLProgram("BillboardShader", "Shaders/Billboard.vert", "Shaders/Billboard.frag", "Shaders/Billboard.geo"));
*/
		std::shared_ptr<cg::ParticleRenderer> particleRenderer = std::make_shared<cg::ParticleRenderer>(
				cu::ResourceManager::getGLSLProgram("ParticleShader", "Shaders/ParticleShader.vert", "Shaders/ParticleShader.frag", "Shaders/ParticleShader.geo"));

		/*std::weak_ptr<ce::Entity> testSprite = root->addChild("testSprite");
		testSprite.lock()->addComponent<ce::SpriteRenderer>(
				cu::ResourceManager::getSprite("TestSprite",
						cu::ResourceManager::getGLTexture2D("Textures/img_test.png","texture_diffuse"),
						glm::vec2(10.0f, 10.0f), cg::Color::white), billboardRenderer);
		testSprite.lock()->getComponent<ce::Transform>().lock()->translate(10.0f, 10.0f, 10.0f);*/

		//std::weak_ptr<ce::Entity> particleSystem = root->addChild("PSTEST");
		/*particleSystem.lock()->addComponent<ce::ParticleSystem>(particleRenderer, gravity, cg::Color::red,
				100, 50.0f, 1.0f, 1.0f, 1.0f, 0.0f,
				cu::ResourceManager::getGLTexture2D("Textures/particle.png", "texture_diffuse"));*/
		/*particleSystem.lock()->addComponent<ce::ParticleSystem>(particleRenderer, 100, 10.0f, 10.0f, 1.0f,
				gravity, cg::Color::red, 4.0f,
				cu::ResourceManager::getGLTexture2D("Textures/particle.png", "texture_diffuse"));*/

		/*std::weak_ptr<ce::Entity> nanosuit = root->addChild("nanosuit");
		nanosuit.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(10.0f, 20.0f, 10.0f));
		nanosuit.lock()->addComponent<ce::MeshRenderer>(cu::ResourceManager::getMesh("Models/nanosuit/nanosuit.obj"), renderer3D);
		nanosuit.lock()->addComponent<ce::SphereCollider>(3.0f);
		nanosuit.lock()->addComponent<ce::RigidBody>(physicsWorld, 1.0f);
		nanosuit.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(4);
		nanosuit.lock()->getComponent<ce::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 0.0f, 0.0f));
		nanosuit.lock()->getComponent<ce::RigidBody>().lock()->setAngularFactor(glm::vec3(0.0f, 0.0f, 0.0f));
		nanosuit.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
		nanosuit.lock()->addComponent<PaddleController>(150000.0f);*/

		std::weak_ptr<ce::Entity> directionalLight = root->addChild("PointLight");
		directionalLight.lock()->addComponent<ce::Light>();
		directionalLight.lock()->getComponent<ce::Light>().lock()->setLightType(ce::LightType::DIRECTIONAL);
		directionalLight.lock()->getComponent<ce::Light>().lock()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		directionalLight.lock()->getComponent<ce::Light>().lock()->setAmbientIntensity(0.5f);
		directionalLight.lock()->getComponent<ce::Light>().lock()->setDiffuseIntensity(0.8f);
		directionalLight.lock()->getComponent<ce::Light>().lock()->setSpecularIntensity(0.8f);
		directionalLight.lock()->getComponent<ce::Transform>().lock()->setLocalOrientation(glm::vec3(-0.2f, -1.0f, -0.3f));

		std::weak_ptr<ce::Entity> paddle = root->addChild("PlayerPaddle");
		paddle.lock()->setTag("Paddle");
		paddle.lock()->addComponent<ce::MeshRenderer>(cu::ResourceManager::getMesh("Models/TestModels/cube.obj"), renderer3D);
		paddle.lock()->getComponent<ce::Transform>().lock()->setWorldScale(glm::vec3(2.0f, 0.5f, 1.0f));
		paddle.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, -3.0f, 0.0f));
		paddle.lock()->addComponent<ce::BoxCollider>(glm::vec3(2.0f, 0.5f, 1.0f));
		paddle.lock()->addComponent<ce::RigidBody>(physicsWorld, 1.0f);
		paddle.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(4);
		paddle.lock()->getComponent<ce::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 0.0f, 0.0f));
		paddle.lock()->getComponent<ce::RigidBody>().lock()->setAngularFactor(glm::vec3(0.0f, 0.0f, 0.0f));
		paddle.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
		paddle.lock()->addComponent<PaddleController>(150000.0f);
		//paddle.lock()->addComponent<ce::ParticleSystem>(particleRenderer, 100, 1.0f, 10.0f, 1.0f,
		//		gravity, cg::Color::white, 0.1f,
		//		cu::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
		//		[](ce::Particle& _particle, float _gravity, float _deltaTime)
		//{
		//		//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
		//		_particle.m_velocity.y += _gravity * _deltaTime;
		//		_particle.m_position += _particle.m_velocity * _deltaTime;
		//});

		std::weak_ptr<ce::Entity> groundBound = root->addChild("GroundBoundary");
		groundBound.lock()->setTag("ground");
		groundBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(-2.5f, -5.0f, 0.0f));
		groundBound.lock()->addComponent<ce::BoxCollider>(glm::vec3(32.5f, 1.0f, 1.0f));
		groundBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);

		std::weak_ptr<ce::Entity> ceilingBound = root->addChild("CeilingBoundary");
		ceilingBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(-2.5f, 30.0f, 0.0f));
		ceilingBound.lock()->addComponent<ce::BoxCollider>(glm::vec3(32.5f, 1.0f, 1.0f));
		ceilingBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
		ceilingBound.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<ce::Entity> leftBound = root->addChild("LeftBoundary");
		leftBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(-36.0f, 12.5f, 0.0f));
		leftBound.lock()->addComponent<ce::BoxCollider>(glm::vec3(1.0f, 16.0f, 1.0f));
		leftBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
		leftBound.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<ce::Entity> rightBound = root->addChild("RightBoundary");
		rightBound.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(31, 12.5f, 0.0f));
		rightBound.lock()->addComponent<ce::BoxCollider>(glm::vec3(1.0f, 16.0f, 1.0f));
		rightBound.lock()->addComponent<ce::RigidBody>(physicsWorld, 0.0f);
		rightBound.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<ce::Entity> ball = root->addChild("Ball");
		ball.lock()->setTag("ball");
		ball.lock()->addComponent<ce::MeshRenderer>(cu::ResourceManager::getMesh("Models/TestModels/sphere.obj"), renderer3D);
		ball.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f, 3.0f, 0.0f));
		ball.lock()->addComponent<ce::SphereCollider>(1.0f);
		ball.lock()->addComponent<ce::RigidBody>(physicsWorld, 1.0f);
		ball.lock()->getComponent<ce::RigidBody>().lock()->setActivationState(5);
		ball.lock()->getComponent<ce::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 1.0f, 0.0f));
		ball.lock()->getComponent<ce::RigidBody>().lock()->setRestitution(1.0f);
		ball.lock()->addComponent<BallBehavior>();
		ball.lock()->addComponent<ce::Light>();
		ball.lock()->getComponent<ce::Light>().lock()->setLightType(ce::LightType::POINT);
		ball.lock()->getComponent<ce::Light>().lock()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		ball.lock()->getComponent<ce::Light>().lock()->setAmbientIntensity(0.05f);
		ball.lock()->getComponent<ce::Light>().lock()->setDiffuseIntensity(0.8f);
		ball.lock()->getComponent<ce::Light>().lock()->setSpecularIntensity(1.0f);
		ball.lock()->getComponent<ce::Light>().lock()->setAttenuation(ce::Attenuation(1.0f, 0.09f, 0.032f));
		//ball.lock()->addComponent<ce::ParticleSystem>(particleRenderer, 100, 1.0f, 10.0f, 1.0f,
		//		gravity, cg::Color::white, 1.0f,
		//		cu::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
		//		[](ce::Particle& _particle, float _gravity, float _deltaTime)
		//{
		//		//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
		//		_particle.m_velocity.y += _gravity * _deltaTime;
		//		_particle.m_position += _particle.m_velocity * _deltaTime;
		//});

		for (int i = -30; i < 30; i += 4)
		{
				for (int j = -10; j < 0; j += 4)
				{
								std::weak_ptr<ce::Entity> brick = root->addChild("Brick");
								brick.lock()->setTag("brick");
								brick.lock()->addComponent<ce::MeshRenderer>(cu::ResourceManager::getMesh("Models/TestModels/cube.obj"), renderer3D);
								brick.lock()->getComponent<ce::Transform>().lock()->translate(glm::vec3(0.0f + i, 30.0f + j, 0.0f));
								brick.lock()->addComponent<ce::BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f));
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
								//if (window.wasResized())
								//{
								//		//camera.lock()->getComponent<ce::Camera>()->resize(window.getWidth(), window.getHeight());
								//		window.resizeHandled();
								//}
						}
						}
				}

				if (cu::Input::isKeyPressed(cu::KeyCode::ESC))
				{
						quit = true;
				}
				if (cu::Input::isKeyPressed(cu::KeyCode::TAB))
				{
						debugMode = !debugMode;
				}
				if (cu::Input::isKeyPressed(cu::KeyCode::ALPHA9))
				{
						camera2.lock()->getComponent<ce::Transform>().lock()->setWorldPosition(mainCamera.lock()->getComponent<ce::Transform>().lock()->worldPosition());
						camera2.lock()->getComponent<ce::Transform>().lock()->setWorldOrientation(mainCamera.lock()->getComponent<ce::Transform>().lock()->worldOrientationRaw());
				}

				//Update
				root->refreshAll();
				root->updateAll(fpsLimiter.deltaTime());

				physicsWorld->stepSimulation(fpsLimiter.deltaTime());

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

						particleRenderer->begin();
						//billboardRenderer->begin();
						renderer2D->begin();
						renderer3D->begin();

						// call the render function of all entities (submits all entities with sprite and mesh renderers)
						root->renderAll();

						particleRenderer->end();
						//billboardRenderer->end();
						renderer2D->end();
						renderer3D->end();

						//billboardRenderer->flush();
						renderer2D->flush();
						renderer3D->flush();

						if (debugMode)
						{
								//use the debug renderer to draw the debug physics world
								physicsWorld->debugDrawWorld();
								camera2.lock()->getComponent<ce::Camera>().lock()->renderFrustum(&debugRenderer);
								//debugRenderer.drawMeshSphereBounds(nanosuit);
								debugRenderer.end();
								debugRenderer.render(camera.lock()->getViewMatrix(), camera.lock()->getProjectionMatrix(), 5.0f);
						}
						
						//render the camera's skybox if it has one
						camera.lock()->renderSkybox();

						particleRenderer->flush();
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