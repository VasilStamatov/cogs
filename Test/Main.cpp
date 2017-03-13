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
#include <cogs\Renderer3D.h>
#include <cogs\ParticleSystem.h>
#include <cogs\Skybox.h>
#include <cogs\Light.h>
#include <cogs\GLTexture2D.h>

#include <iostream>

#include "PaddleController.h"
#include "BallBehavior.h"
#include "PostProcessTest.h"
#include "SpriteController.h"

int main(int argc, char** argv)
{
		cogs::Window window;
		window.create("Test", 1024, 576, cogs::WindowCreationFlags::NONE);
		window.setRelativeMouseMode(true);
		bool quit{ false };
		bool debugMode{ false };
		cogs::FpsLimiter fpsLimiter(600.0f);

		const glm::vec3 gravity(0.0f, -9.81f, 0.0f);

		std::shared_ptr<cogs::Physics> physicsWorld = std::make_shared<cogs::Physics>(gravity.x, gravity.y, gravity.z);

		std::shared_ptr<cogs::Entity> root = cogs::Entity::create("Root");

		std::shared_ptr<cogs::Framebuffer> testFramebuffer = cogs::Framebuffer::create(window.getWidth(), window.getHeight());

		std::vector<std::string> skyboxFilenames(6);
		skyboxFilenames.at(0) = "Textures/space/cwd_rt.jpg";
		skyboxFilenames.at(1) = "Textures/space/cwd_lf.jpg";
		skyboxFilenames.at(2) = "Textures/space/cwd_up.jpg";
		skyboxFilenames.at(3) = "Textures/space/cwd_dn.jpg";
		skyboxFilenames.at(4) = "Textures/space/cwd_bk.jpg";
		skyboxFilenames.at(5) = "Textures/space/cwd_ft.jpg";

		std::shared_ptr<cogs::Skybox> testSkybox = cogs::Skybox::create(
				cogs::ResourceManager::getGLSLProgram("SkyboxShader", "Shaders/Skybox.vert", "Shaders/Skybox.frag"),
				cogs::ResourceManager::getGLCubemap("skyboxTexture", skyboxFilenames), true);

		std::weak_ptr<cogs::Entity> mainCamera = root->addChild("Camera");
		mainCamera.lock()->addComponent<cogs::Camera>(window.getWidth(), window.getHeight(), cogs::ProjectionType::PERSPECTIVE);
		mainCamera.lock()->getComponent<cogs::Camera>().lock()->setBackgroundColor(cogs::Color::black);
		mainCamera.lock()->addComponent<cogs::FPSCameraControl>(50.0f);
		mainCamera.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(0.0f, 0.0f, 55.0f));
		mainCamera.lock()->getComponent<cogs::Camera>().lock()->setSkybox(testSkybox);
		mainCamera.lock()->addComponent<cogs::Light>();
		mainCamera.lock()->getComponent<cogs::Light>().lock()->setLightType(cogs::LightType::SPOT);
		mainCamera.lock()->getComponent<cogs::Light>().lock()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		mainCamera.lock()->getComponent<cogs::Light>().lock()->setAmbientIntensity(0.0f);
		mainCamera.lock()->getComponent<cogs::Light>().lock()->setDiffuseIntensity(1.0f);
		mainCamera.lock()->getComponent<cogs::Light>().lock()->setSpecularIntensity(1.0f);
		mainCamera.lock()->getComponent<cogs::Light>().lock()->setCutOff(glm::cos(glm::radians(12.5f)));
		mainCamera.lock()->getComponent<cogs::Light>().lock()->setOuterCutOff(glm::cos(glm::radians(15.0f)));
		mainCamera.lock()->getComponent<cogs::Light>().lock()->setAttenuation(cogs::Attenuation(1.0f, 0.09f, 0.032f));
		//mainCamera.lock()->getComponent<cogs::Camera>().lock()->setRenderTarget(testFramebuffer);
		/*mainCamera.lock()->addComponent<PostProcessTest>(cogs::ResourceManager::getGLSLProgram(
				"PostProcessShader", "Shaders/PostProcess.vert", "Shaders/PostProcess.frag"));*/

		std::weak_ptr<cogs::Entity> camera2 = root->addChild("MainCamera");
		camera2.lock()->addComponent<cogs::Camera>(window.getWidth(), window.getHeight(), cogs::ProjectionType::PERSPECTIVE);
		camera2.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(0.0f, 0.0f, 55.0f));

		std::shared_ptr<cogs::Renderer2D> renderer2D = std::make_shared<cogs::Renderer2D>(
				cogs::ResourceManager::getGLSLProgram("BasicShader", "Shaders/BasicShader.vert", "Shaders/BasicShader.frag"));

		std::shared_ptr<cogs::Renderer3D> renderer3D = std::make_shared<cogs::Renderer3D>(
				cogs::ResourceManager::getGLSLProgram("Basic3DShader", "Shaders/Basic3DShader.vert", "Shaders/Basic3DShader.frag"));

		std::shared_ptr<cogs::ParticleRenderer> particleRenderer = std::make_shared<cogs::ParticleRenderer>(
				cogs::ResourceManager::getGLSLProgram("ParticleShader", "Shaders/ParticleShader.vert", "Shaders/ParticleShader.frag"));
		
		/*for (int i = 0; i < 20; i++)
		{
				std::weak_ptr<cogs::Entity> sprite = root->addChild("testSprite" + std::to_string(i));
				sprite.lock()->addComponent<cogs::SpriteRenderer>(
						cogs::ResourceManager::getSprite("TestSprite",
								cogs::ResourceManager::getGLTexture2D("Textures/img_test.png", "texture_diffuse"),
								glm::vec2(5.0f, 5.0f), cogs::Color::white), renderer2D);

				sprite.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(0.0f + i * 6, 10.0f, 10.0f));

				sprite.lock()->addComponent<SpriteController>();
		}*/

		//std::weak_ptr<cogs::Entity> particleSystem = root->addChild("PSTEST");
		/*particleSystem.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, gravity, cogs::Color::red,
				100, 50.0f, 1.0f, 1.0f, 1.0f, 0.0f,
				cogs::ResourceManager::getGLTexture2D("Textures/particle.png", "texture_diffuse"));*/
		/*particleSystem.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, 100, 10.0f, 10.0f, 1.0f,
				gravity, cogs::Color::red, 4.0f,
				cogs::ResourceManager::getGLTexture2D("Textures/particle.png", "texture_diffuse"));*/

		/*std::weak_ptr<cogs::Entity> nanosuit = root->addChild("nanosuit");
		nanosuit.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(10.0f, 20.0f, 10.0f));
		nanosuit.lock()->addComponent<cogs::MeshRenderer>(cogs::ResourceManager::getMesh("Models/nanosuit/nanosuit.obj"), renderer3D);
		nanosuit.lock()->addComponent<cogs::SphereCollider>(3.0f);
		nanosuit.lock()->addComponent<cogs::RigidBody>(physicsWorld, 1.0f);
		nanosuit.lock()->getComponent<cogs::RigidBody>().lock()->setActivationState(4);
		nanosuit.lock()->getComponent<cogs::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 0.0f, 0.0f));
		nanosuit.lock()->getComponent<cogs::RigidBody>().lock()->setAngularFactor(glm::vec3(0.0f, 0.0f, 0.0f));
		nanosuit.lock()->getComponent<cogs::RigidBody>().lock()->setRestitution(1.0f);
		nanosuit.lock()->addComponent<PaddleController>(150000.0f);*/

		std::weak_ptr<cogs::Entity> directionalLight = root->addChild("PointLight");
		directionalLight.lock()->addComponent<cogs::Light>();
		directionalLight.lock()->getComponent<cogs::Light>().lock()->setLightType(cogs::LightType::DIRECTIONAL);
		directionalLight.lock()->getComponent<cogs::Light>().lock()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		directionalLight.lock()->getComponent<cogs::Light>().lock()->setAmbientIntensity(0.5f);
		directionalLight.lock()->getComponent<cogs::Light>().lock()->setDiffuseIntensity(0.8f);
		directionalLight.lock()->getComponent<cogs::Light>().lock()->setSpecularIntensity(0.8f);
		directionalLight.lock()->getComponent<cogs::Transform>().lock()->setLocalOrientation(glm::vec3(-0.2f, -1.0f, -0.3f));

		std::weak_ptr<cogs::GLTexture2D> textureAtlas = cogs::ResourceManager::getGLTexture2D("Textures/fire.png", "texture_diffuse");
		textureAtlas.lock()->setType(cogs::TextureType::MULTIPLE);
		textureAtlas.lock()->setDims(glm::ivec2(8, 8));

		std::weak_ptr<cogs::Entity> paddle = root->addChild("PlayerPaddle");
		paddle.lock()->setTag("Paddle");
		paddle.lock()->addComponent<cogs::MeshRenderer>(cogs::ResourceManager::getMesh("Models/TestModels/cube.obj"), renderer3D);
		paddle.lock()->getComponent<cogs::Transform>().lock()->setWorldScale(glm::vec3(2.0f, 0.5f, 1.0f));
		paddle.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(0.0f, -3.0f, 0.0f));
		paddle.lock()->addComponent<cogs::BoxCollider>(glm::vec3(2.0f, 0.5f, 1.0f));
		paddle.lock()->addComponent<cogs::RigidBody>(physicsWorld, 1.0f);
		paddle.lock()->getComponent<cogs::RigidBody>().lock()->setActivationState(4);
		paddle.lock()->getComponent<cogs::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 0.0f, 0.0f));
		paddle.lock()->getComponent<cogs::RigidBody>().lock()->setAngularFactor(glm::vec3(0.0f, 0.0f, 0.0f));
		paddle.lock()->getComponent<cogs::RigidBody>().lock()->setRestitution(1.0f);
		paddle.lock()->addComponent<PaddleController>(150000.0f);
		//paddle.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, 200, 15.0f, 1.0f, true,
		//		gravity, cogs::Color::white, 1.0f,
		//		cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
		//		[](cogs::Particle& _particle, float _gravity, float _deltaTime)
		//{
		//		//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
		//		_particle.m_velocity.y += _gravity * _deltaTime;
		//		_particle.m_position += _particle.m_velocity * _deltaTime;
		//});

		std::weak_ptr<cogs::Entity> groundBound = root->addChild("GroundBoundary");
		groundBound.lock()->setTag("ground");
		groundBound.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(-2.5f, -5.0f, 0.0f));
		groundBound.lock()->addComponent<cogs::BoxCollider>(glm::vec3(32.5f, 1.0f, 1.0f));
		groundBound.lock()->addComponent<cogs::RigidBody>(physicsWorld, 0.0f);

		std::weak_ptr<cogs::Entity> ceilingBound = root->addChild("CeilingBoundary");
		ceilingBound.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(-2.5f, 30.0f, 0.0f));
		ceilingBound.lock()->addComponent<cogs::BoxCollider>(glm::vec3(32.5f, 1.0f, 1.0f));
		ceilingBound.lock()->addComponent<cogs::RigidBody>(physicsWorld, 0.0f);
		ceilingBound.lock()->getComponent<cogs::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<cogs::Entity> leftBound = root->addChild("LeftBoundary");
		leftBound.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(-36.0f, 12.5f, 0.0f));
		leftBound.lock()->addComponent<cogs::BoxCollider>(glm::vec3(1.0f, 16.0f, 1.0f));
		leftBound.lock()->addComponent<cogs::RigidBody>(physicsWorld, 0.0f);
		leftBound.lock()->getComponent<cogs::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<cogs::Entity> rightBound = root->addChild("RightBoundary");
		rightBound.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(31, 12.5f, 0.0f));
		rightBound.lock()->addComponent<cogs::BoxCollider>(glm::vec3(1.0f, 16.0f, 1.0f));
		rightBound.lock()->addComponent<cogs::RigidBody>(physicsWorld, 0.0f);
		rightBound.lock()->getComponent<cogs::RigidBody>().lock()->setRestitution(1.0f);

		std::weak_ptr<cogs::Entity> ball = root->addChild("Ball");
		ball.lock()->setTag("ball");
		ball.lock()->addComponent<cogs::MeshRenderer>(cogs::ResourceManager::getMesh("Models/TestModels/sphere.obj"), renderer3D);
		ball.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(0.0f, 3.0f, 0.0f));
		ball.lock()->addComponent<cogs::SphereCollider>(1.0f);
		ball.lock()->addComponent<cogs::RigidBody>(physicsWorld, 1.0f);
		ball.lock()->getComponent<cogs::RigidBody>().lock()->setActivationState(5);
		ball.lock()->getComponent<cogs::RigidBody>().lock()->setLinearFactor(glm::vec3(1.0f, 1.0f, 0.0f));
		ball.lock()->getComponent<cogs::RigidBody>().lock()->setRestitution(1.0f);
		ball.lock()->addComponent<BallBehavior>();
		ball.lock()->addComponent<cogs::Light>();
		ball.lock()->getComponent<cogs::Light>().lock()->setLightType(cogs::LightType::POINT);
		ball.lock()->getComponent<cogs::Light>().lock()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		ball.lock()->getComponent<cogs::Light>().lock()->setAmbientIntensity(0.05f);
		ball.lock()->getComponent<cogs::Light>().lock()->setDiffuseIntensity(0.8f);
		ball.lock()->getComponent<cogs::Light>().lock()->setSpecularIntensity(1.0f);
		ball.lock()->getComponent<cogs::Light>().lock()->setAttenuation(cogs::Attenuation(1.0f, 0.09f, 0.032f));
		//ball.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, 100, 1.0f, 10.0f, 1.0f,
		//		gravity, cogs::Color::white, 1.0f,
		//		cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
		//		[](cogs::Particle& _particle, float _gravity, float _deltaTime)
		//{
		//		//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
		//		_particle.m_velocity.y += _gravity * _deltaTime;
		//		_particle.m_position += _particle.m_velocity * _deltaTime;
		//});

		for (int i = -30; i < 30; i += 4)
		{
				for (int j = -10; j < 0; j += 4)
				{
								std::weak_ptr<cogs::Entity> brick = root->addChild("Brick");
								brick.lock()->setTag("brick");
								brick.lock()->addComponent<cogs::MeshRenderer>(cogs::ResourceManager::getMesh("Models/TestModels/cube.obj"), renderer3D);
								brick.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(0.0f + i, 30.0f + j, 0.0f));
								brick.lock()->addComponent<cogs::BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f));
								brick.lock()->addComponent<cogs::RigidBody>(physicsWorld, 0.0f);
								brick.lock()->getComponent<cogs::RigidBody>().lock()->setActivationState(5);
								brick.lock()->getComponent<cogs::RigidBody>().lock()->setRestitution(1.0f);
				}
		}

		cogs::BulletDebugRenderer debugRenderer;

		debugRenderer.setDebugMode(debugRenderer.DBG_DrawWireframe);
		physicsWorld->setDebugDrawer(&debugRenderer);

		while (!quit)
		{
				fpsLimiter.beginFrame();

				cogs::Input::update();

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
								cogs::Input::setMouseCoords(static_cast<float>(evnt.motion.x), static_cast<float>(evnt.motion.y));
								cogs::Input::setMouseMotion(static_cast<float>(evnt.motion.xrel), static_cast<float>(evnt.motion.yrel));
								break;
						}
						case SDL_KEYDOWN:
						{
								cogs::Input::pressKey(evnt.key.keysym.sym);
								break;
						}
						case SDL_KEYUP:
						{
								cogs::Input::releaseKey(evnt.key.keysym.sym);
								break;
						}
						case SDL_MOUSEBUTTONDOWN:
						{
								cogs::Input::pressKey(evnt.button.button);
								break;
						}
						case SDL_MOUSEBUTTONUP:
						{
								cogs::Input::releaseKey(evnt.button.button);
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
								//		//camera.lock()->getComponent<cogs::Camera>()->resize(window.getWidth(), window.getHeight());
								//		window.resizeHandled();
								//}
						}
						}
				}

				if (cogs::Input::isKeyPressed(cogs::KeyCode::ESC))
				{
						quit = true;
				}
				if (cogs::Input::isKeyPressed(cogs::KeyCode::TAB))
				{
						debugMode = !debugMode;
				}
				if (cogs::Input::isKeyPressed(cogs::KeyCode::ALPHA9))
				{
						camera2.lock()->getComponent<cogs::Transform>().lock()->setWorldPosition(mainCamera.lock()->getComponent<cogs::Transform>().lock()->worldPosition());
						camera2.lock()->getComponent<cogs::Transform>().lock()->setWorldOrientation(mainCamera.lock()->getComponent<cogs::Transform>().lock()->worldOrientationRaw());
				}

				//Update
				root->refreshAll();
				root->updateAll(fpsLimiter.deltaTime());

				physicsWorld->stepSimulation(fpsLimiter.deltaTime());

				//Render

				//main camera
				/*std::weak_ptr<cogs::Camera> mainCam = cogs::Camera::getMain();

				assert(!mainCam.expired());

				cogs::Framebuffer::setActive(mainCam.lock()->getRenderTarget());
				window.setClearColor(mainCam.lock()->getBackgroundColor());
				window.clear(true, true);

				root->renderAll(mainCam);*/

				//other cameras
				for (std::weak_ptr<cogs::Camera> camera : cogs::Camera::getAllCameras())
				{
						if (!camera.lock()->getEntity().lock()->isActive() || camera.expired())
						{
								continue;
						}
						// set the current camera
						cogs::Camera::setCurrent(camera);

						// set the render target
						cogs::Framebuffer::setActive(camera.lock()->getRenderTarget());

						// clear the window with the camera's background color
						window.setClearColor(camera.lock()->getBackgroundColor());
						window.clear(true, true);

						renderer3D->begin();
						renderer2D->begin();
						particleRenderer->begin();

						// call the render function of all entities (submits all entities with sprite and mesh renderers)
						root->renderAll();

						renderer3D->end();
						renderer2D->end();
						particleRenderer->end();

						renderer3D->flush();

						if (debugMode)
						{
								//use the debug renderer to draw the debug physics world
								physicsWorld->debugDrawWorld();
								camera2.lock()->getComponent<cogs::Camera>().lock()->renderFrustum(&debugRenderer);
								//debugRenderer.drawMeshSphereBounds(nanosuit);
								debugRenderer.end();
								debugRenderer.render(camera.lock()->getViewMatrix(), camera.lock()->getProjectionMatrix(), 5.0f);
						}
						
						//render the camera's skybox if it has one
						camera.lock()->renderSkybox();

						particleRenderer->flush();
						renderer2D->flush();
						//set the render target to the default window
						cogs::Framebuffer::setActive(std::weak_ptr<cogs::Framebuffer>());
				}

				root->postProcessAll();

				window.swapBuffer();

				fpsLimiter.endFrame();

				window.setWindowTitle("FPS: " + std::to_string(fpsLimiter.fps()) + " DT: " + std::to_string(fpsLimiter.deltaTime()));
		}
		cogs::ResourceManager::clear();
		window.close();
		return 0;
}