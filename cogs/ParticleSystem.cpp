#include "ParticleSystem.h"

#include "Entity.h"
#include "Camera.h"
#include "Random.h"
#include "Input.h"

#include <algorithm>
#include <glm\gtx\norm.hpp>

namespace cogs
{
		namespace ecs
		{
				ParticleSystem::ParticleSystem(std::weak_ptr<graphics::ParticleRenderer> _renderer,
						int _maxParticles,
						float _initialSpeed,
						float _pps,
						float _width,
						const glm::vec3& _worldGravity,
						const graphics::Color& _color,
						float _decayRate,
						std::weak_ptr<graphics::GLTexture2D> _texture,
						std::function<void(Particle&, float, float)> _updateFunc) :
						m_maxParticles(_maxParticles),
						m_initialSpeed(_initialSpeed),
						m_particlesPerSec(_pps),
						m_particlesWidth(_width),
						m_worldGravity(_worldGravity),
						m_particlesColor(_color),
						m_renderer(_renderer),
						m_decayRate(_decayRate),
						m_texture(_texture),
						m_updateFunc(_updateFunc)
				{
						m_particles = new Particle[m_maxParticles];
				}
				ParticleSystem::~ParticleSystem()
				{
						delete[] m_particles;
				}

				void ParticleSystem::init()
				{
				}

				void ParticleSystem::update(float _deltaTime)
				{
						generateParticles(_deltaTime);
						for (int i = 0; i < m_maxParticles; ++i)
						{
								// check if it is active
								if (m_particles[i].m_life > 0.0f)
								{
										// Update using function pointer
										m_updateFunc(m_particles[i], m_worldGravity.y, _deltaTime);

										//reduce its life by decay amount with frame independence
										m_particles[i].m_life -= (m_decayRate * _deltaTime);
								}
						}
						/*if (utils::Input::isKeyDown(utils::KeyCode::G))
						{
								spawnParticle();
						}*/
				}

				void ParticleSystem::render()
				{
						sortParticles();
						m_renderer.lock()->submit(m_entity);
				}

				void ParticleSystem::setTexture(std::weak_ptr<graphics::GLTexture2D> _texture)
				{
						m_texture = _texture;
				}

				void ParticleSystem::setMaxParticles(int _maxParticles)
				{
						//m_particles.resize(_maxParticles);
				}

				void ParticleSystem::setParticleLifetime(float _particleLifetime)
				{
						m_decayRate = _particleLifetime;
				}

				void ParticleSystem::setParticlesWidth(float _width)
				{
						m_particlesWidth = _width;
				}

				void ParticleSystem::setUpdateFunc(std::function<void(Particle&, float, float)> _updateFunc)
				{
						m_updateFunc = _updateFunc;
				}

				void ParticleSystem::setRenderer(std::weak_ptr<graphics::ParticleRenderer> _renderer)
				{
						m_renderer = _renderer;
				}

				int ParticleSystem::findFreeParticle()
				{
						//check for a free particle from a range between the last free one, and the max possible particles
						for (int i = m_lastFreeParticle; i < m_maxParticles; ++i)
						{
								if (m_particles[i].m_life <= 0.0f)
								{
										m_lastFreeParticle = i;
										return i;
								}
						}

						//check for a free particle from a range between the first free, and the last free one
						for (int i = 0; i < m_lastFreeParticle; ++i)
						{
								if (m_particles[i].m_life <= 0.0f)
								{
										m_lastFreeParticle = i;
										return i;
								}
						}
						// no particles are free return the first index
						return 0;
				}

				void ParticleSystem::sortParticles()
				{
						std::weak_ptr<ecs::Camera> currentCam = ecs::Camera::getCurrent();
						const glm::vec3& cameraPos = currentCam.lock()->getEntity().lock()->getComponent<ecs::Transform>().lock()->worldPosition();
						const glm::vec3& psWorldPos = m_entity.lock()->getComponent<Transform>().lock()->worldPosition();

						std::sort(&m_particles[0], &m_particles[m_maxParticles],
								[&psWorldPos, &cameraPos](const Particle& _p1, const Particle& _p2)
						{
								glm::vec3 p1WorldPos = _p1.m_position + psWorldPos;
								glm::vec3 p2WorldPos = _p2.m_position + psWorldPos;

								float distanceFromCamera1 = glm::length2(p1WorldPos - cameraPos);
								float distanceFromCamera2 = glm::length2(p2WorldPos - cameraPos);

								return (distanceFromCamera1 > distanceFromCamera2);
						});
				}
				void ParticleSystem::generateParticles(float _deltaTime)
				{
						float particlesToCreate = m_particlesPerSec * _deltaTime;

						int count = (int)ceilf(particlesToCreate);

						float partialParticle = glm::clamp(particlesToCreate, 0.0f, 1.0f);

						for (int i = 0; i < count; i++)
						{
								spawnParticle();
						}

						if (utils::Random::getRandFloat(0.0f, 1.0f) < partialParticle)
						{
								spawnParticle();
						}
				}

				void ParticleSystem::spawnParticle()
				{
						int particleIndex = findFreeParticle();

						/*if (particleIndex > m_maxParticles)
						{
								return;
						}*/

						float dirX = utils::Random::getRandFloat(0.0f, 1.0f) * 2.0f - 1.0f;
						float dirZ = utils::Random::getRandFloat(0.0f, 1.0f) * 2.0f - 1.0f;
						glm::vec3 vel = glm::normalize(glm::vec3(dirX, 1.0f, dirZ));
						vel *= m_initialSpeed;
						//find a free particle index, or overwrite the first one
						//get an alias to the particle
						auto& particle = m_particles[particleIndex];

						//give it a full life as it's being spawned
						particle.m_life = 1.0f;

						//set its local position to 0 as it's being spawned from the particle system
						particle.m_position = glm::vec3(0.0f);

						//set its velocity to the emitting velocity
						particle.m_velocity = vel;

						//set its color to the set color for all particles
						particle.m_color = m_particlesColor;

						particle.m_width = m_particlesWidth;
				}

		}
}