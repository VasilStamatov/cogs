#include <cogs\Window.h>
#include <cogs\Timing.h>
#include <cogs\Physics.h>
#include <cogs\Entity.h>
#include <cogs\Framebuffer.h>
#include <cogs\Skybox.h>
#include <cogs\ResourceManager.h>
#include <cogs\Camera.h>
#include <cogs\FPSCameraControl.h>
#include <cogs\ParticleRenderer.h>
#include <cogs\ParticleSystem.h>
#include <cogs\GLTexture2D.h>
#include <cogs\SpatialHash.h>
#include <cogs\Input.h>
#include <cogs\Renderer3D.h>
#include <cogs\MeshRenderer.h>
#include <cogs\IOManager.h>

int main(int argc, char** argv)
{
		cogs::Window window;
		window.create("Test", 1024, 576, cogs::WindowCreationFlags::NONE);
		window.setRelativeMouseMode(true);
		bool quit{ false };
		bool debugMode{ false };
		cogs::FpsLimiter fpsLimiter(6000.0f);

		const glm::vec3 gravity(0.0f, -9.81f, 0.0f);

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
		mainCamera.lock()->getComponent<cogs::Camera>().lock()->setSkybox(testSkybox);
		mainCamera.lock()->addComponent<cogs::FPSCameraControl>(50.0f);
		mainCamera.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(0.0f, 0.0f, 55.0f));

		std::shared_ptr<cogs::ParticleRenderer> particleRenderer = std::make_shared<cogs::ParticleRenderer>(
				cogs::ResourceManager::getGLSLProgram("ParticleShader", "Shaders/ParticleShader.vert", "Shaders/ParticleShader.frag"));

		std::shared_ptr<cogs::SpatialHash> spatialhash = std::make_shared<cogs::SpatialHash>(glm::vec3(5.0f));

		std::weak_ptr<cogs::Entity> particleSystem1 = root->addChild("ParticleSystem1");
		particleSystem1.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(-25.0f, 5.0f, 5.0f));
		particleSystem1.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::purple,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		std::weak_ptr<cogs::Entity> particleSystem2 = root->addChild("ParticleSystem2");
		particleSystem2.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(-20.0f, 5.0f, 5.0f));
		particleSystem2.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::green,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		std::weak_ptr<cogs::Entity> particleSystem3 = root->addChild("ParticleSystem3");
		particleSystem3.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(-15.0f, 5.0f, 5.0f));
		particleSystem3.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::teal,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		std::weak_ptr<cogs::Entity> particleSystem4 = root->addChild("ParticleSystem4");
		particleSystem4.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(-10.0f, 5.0f, 5.0f));
		particleSystem4.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::yellow,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		std::weak_ptr<cogs::Entity> particleSystem5 = root->addChild("ParticleSystem5");
		particleSystem5.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(-5.0f, 5.0f, 5.0f));
		particleSystem5.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::lime,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		std::weak_ptr<cogs::Entity> particleSystem6 = root->addChild("ParticleSystem6");
		particleSystem6.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(0.0f, 5.0f, 5.0f));
		particleSystem6.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::red,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		std::weak_ptr<cogs::Entity> particleSystem7 = root->addChild("ParticleSystem7");
		particleSystem7.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(5.0f, 5.0f, 5.0f));
		particleSystem7.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::fuchsia,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		std::weak_ptr<cogs::Entity> particleSystem8 = root->addChild("ParticleSystem8");
		particleSystem8.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(10.0f, 5.0f, 5.0f));
		particleSystem8.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::aqua,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		std::weak_ptr<cogs::Entity> particleSystem9 = root->addChild("ParticleSystem9");
		particleSystem9.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(15, 5.0f, 5.0f));
		particleSystem9.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::white,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		std::weak_ptr<cogs::Entity> particleSystem10 = root->addChild("ParticleSystem10");
		particleSystem10.lock()->getComponent<cogs::Transform>().lock()->translate(glm::vec3(20.0f, 5.0f, 5.0f));
		particleSystem10.lock()->addComponent<cogs::ParticleSystem>(particleRenderer, spatialhash, 1000, 10.0f, 1.0f, 0.0f, true, true,
				gravity, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), cogs::Color::silver,
				cogs::ResourceManager::getGLTexture2D("Textures/particleStar.png", "texture_diffuse"),
				[](cogs::Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				//_particle.m_color.a = (unsigned char)((_particle.m_life / 1.0f) * 255);
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		});

		cogs::BulletDebugRenderer debugRenderer;

		debugRenderer.setDebugMode(debugRenderer.DBG_DrawWireframe);

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

				//Update
				spatialhash->clearBuckets();
				root->refreshAll();
				root->updateAll(fpsLimiter.deltaTime());

				//Render

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

						particleRenderer->begin();

						// call the render function of all entities (submits all entities with sprite and mesh renderers)
						root->renderAll();

						particleRenderer->end();

						//being flushing objects to the screen

						//first opaque objects

						if (debugMode)
						{
								//use the debug renderer to draw the debug physics world
								spatialhash->render(&debugRenderer);
								particleSystem1.lock()->getComponent<cogs::ParticleSystem>().lock()->RenderBounds(&debugRenderer);
								//debugRenderer.drawMeshSphereBounds(nanosuit);
								debugRenderer.end();
								debugRenderer.render(camera.lock()->getViewMatrix(), camera.lock()->getProjectionMatrix(), 1.0f);
						}

						//render the camera's skybox if it has one
						camera.lock()->renderSkybox();

						//next transparent objects
						particleRenderer->flush();

						//set the render target to the default window
						cogs::Framebuffer::setActive(std::weak_ptr<cogs::Framebuffer>());
				}

				root->postProcessAll();

				window.swapBuffer();

				fpsLimiter.endFrame();

				static int counter = 0;

				static float fps = 0.0f;
				static float dt = 0.0f;

				fps += fpsLimiter.fps();
				dt += fpsLimiter.deltaTime();

				if (counter == 100)
				{
						fps /= 100.0f;
						dt /= 100.0f;
						window.setWindowTitle("FPS: " + std::to_string(fps) + " DT: " + std::to_string(dt));
						counter = 0;
				}
				else
				{
						counter++;
				}

		}
		cogs::ResourceManager::clear();
		window.close();
		return 0;
}