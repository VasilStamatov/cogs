#include "ParticleSystem.h"

#include "Entity.h"
#include "Camera.h"
#include "Random.h"
#include "GLTexture2D.h"
#include "ParticleRenderer.h"

#include <algorithm>
#include <glm\gtx\norm.hpp>

namespace cogs
{
		ParticleSystem::ParticleSystem(std::weak_ptr<ParticleRenderer> _renderer,
				int _maxParticles,
				float _initialSpeed,
				float _width,
				bool _additive,
				const glm::vec3& _worldGravity,
				const Color& _color,
				float _decayRate,
				std::weak_ptr<GLTexture2D> _texture,
				std::function<void(Particle&, float, float)> _updateFunc) :
				m_maxParticles(_maxParticles),
				m_initialSpeed(_initialSpeed),
				m_particlesWidth(_width),
				m_additive(_additive),
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
				//generateParticles(_deltaTime);
				spawnParticle();
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

				/*if (m_collisions)
				{
						collideParticles();
				}*/
		}

		void ParticleSystem::render()
		{
				if (!m_additive)
				{
						sortParticles();
				}
				m_renderer.lock()->submit(m_entity);
		}

		void ParticleSystem::setTexture(std::weak_ptr<GLTexture2D> _texture)
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

		void ParticleSystem::setRenderer(std::weak_ptr<ParticleRenderer> _renderer)
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

		void ParticleSystem::generateParticles(float _deltaTime)
		{
				/*float particlesToCreate = m_particlesPerSec * _deltaTime;

				int count = (int)ceilf(particlesToCreate);

				float partialParticle = glm::clamp(particlesToCreate, 0.0f, 1.0f);

				for (int i = 0; i < count; i++)
				{
						spawnParticle();
				}

				if (Random::getRandFloat(0.0f, 1.0f) < partialParticle)
				{
						spawnParticle();
				}*/
		}

		void ParticleSystem::spawnParticle()
		{
				int particleIndex = findFreeParticle();

				/*if (particleIndex > m_maxParticles)
				{
						return;
				}*/

				float dirX = Random::getRandFloat(0.0f, 1.0f) * 2.0f - 1.0f;
				float dirY = Random::getRandFloat(0.0f, 1.0f) * 2.0f - 1.0f;
				float dirZ = Random::getRandFloat(0.0f, 1.0f) * 2.0f - 1.0f;
				glm::vec3 vel = glm::normalize(glm::vec3(dirX, dirY, dirZ));
				vel *= m_initialSpeed;
				//find a free particle index, or overwrite the first one
				//get an alias to the particle
				auto& particle = m_particles[particleIndex];

				//give it a full life as it's being spawned
				particle.m_life = 1.0f;

				//set its local position to 0 as it's being spawned from the particle system
				particle.m_position = m_entity.lock()->getComponent<Transform>().lock()->worldPosition();

				//set its velocity to the emitting velocity
				particle.m_velocity = vel;

				//set its color to the set color for all particles
				particle.m_color = m_particlesColor;

				particle.m_width = m_particlesWidth;
		}

		void ParticleSystem::sortParticles()
		{
				std::weak_ptr<Camera> currentCam = Camera::getCurrent();
				const glm::vec3& cameraPos = currentCam.lock()->getEntity().lock()->getComponent<Transform>().lock()->worldPosition();

				std::sort(&m_particles[0], &m_particles[m_maxParticles],
						[&cameraPos](const Particle& _p1, const Particle& _p2)
				{
						float distanceFromCamera1 = glm::length2(_p1.m_position - cameraPos);
						float distanceFromCamera2 = glm::length2(_p2.m_position - cameraPos);

						return (distanceFromCamera1 > distanceFromCamera2);
				});
		}

		//void ParticleSystem::collideParticles()
		//{
		//		for (size_t i = 0; i < m_maxParticles - 1; i++)
		//		{
		//				if (m_particles[i].m_life <= 0.0f)
		//				{
		//						continue;
		//				}
		//				for (size_t j = i + 1; j < m_maxParticles; j++)
		//				{
		//						if (m_particles[j].m_life <= 0.0f)
		//						{
		//								continue;
		//						}
		//						float p1Radius = m_particles[i].m_width * 0.5f;
		//						float p2Radius = m_particles[j].m_width * 0.5f;

		//						glm::vec3 p1min(m_particles[i].m_position.x - p1Radius,
		//								m_particles[i].m_position.y - p1Radius,
		//								m_particles[i].m_position.z);
		//						glm::vec3 p1max(m_particles[i].m_position.x + p1Radius,
		//								m_particles[i].m_position.y + p1Radius,
		//								m_particles[i].m_position.z);

		//						glm::vec3 p2min(m_particles[j].m_position.x - p2Radius,
		//								m_particles[j].m_position.y - p2Radius,
		//								m_particles[j].m_position.z);
		//						glm::vec3 p2max(m_particles[j].m_position.x + p2Radius,
		//								m_particles[j].m_position.y + p2Radius,
		//								m_particles[j].m_position.z);

		//						if (p1max.x > p2min.x &&
		//								p1min.x < p2max.x &&
		//								p1max.y > p2min.y &&
		//								p1min.y < p2max.y &&
		//								p1max.z == p2min.z &&
		//								p1min.z == p2max.z)
		//						{
		//								//AABBs are overlapping

		//								float distance = sqrtf(
		//										((m_particles[i].m_position.x - m_particles[j].m_position.x) *
		//										(m_particles[i].m_position.x - m_particles[j].m_position.x)) +
		//												((m_particles[i].m_position.y - m_particles[j].m_position.y) *
		//										(m_particles[i].m_position.y - m_particles[j].m_position.y))
		//								);

		//								if (distance < p1Radius + p2Radius)
		//								{
		//										//circle collision detected !
		//										/*
		//										newVelX1 = (firstBall.speed.x * (firstBall.mass – secondBall.mass) + (2 * secondBall.mass * secondBall.speed.x)) / (firstBall.mass + secondBall.mass);
		//										newVelY1 = (firstBall.speed.y * (firstBall.mass – secondBall.mass) + (2 * secondBall.mass * secondBall.speed.y)) / (firstBall.mass + secondBall.mass);
		//										newVelX2 = (secondBall.speed.x * (secondBall.mass – firstBall.mass) + (2 * firstBall.mass * firstBall.speed.x)) / (firstBall.mass + secondBall.mass);
		//										newVelY2 = (secondBall.speed.y * (secondBall.mass – firstBall.mass) + (2 * firstBall.mass * firstBall.speed.y)) / (firstBall.mass + secondBall.mass);
		//										*/

		//										float newVelX1 = (m_particles[i].m_velocity.x * (p1Radius - p2Radius) +
		//												(2 * p2Radius * m_particles[j].m_velocity.x)) / (p1Radius + p2Radius);

		//										float newVelY1 = (m_particles[i].m_velocity.y * (p1Radius - p2Radius) +
		//												(2 * p2Radius * m_particles[j].m_velocity.y)) / (p1Radius + p2Radius);

		//										float newVelZ1 = (m_particles[i].m_velocity.z * (p1Radius - p2Radius) +
		//												(2 * p2Radius * m_particles[j].m_velocity.z)) / (p1Radius + p2Radius);

		//										float newVelX2 = (m_particles[j].m_velocity.x * (p2Radius - p1Radius) +
		//												(2 * p1Radius * m_particles[i].m_velocity.x)) / (p1Radius + p2Radius);

		//										float newVelY2 = (m_particles[j].m_velocity.y * (p2Radius - p1Radius) +
		//												(2 * p1Radius * m_particles[i].m_velocity.y)) / (p1Radius + p2Radius);

		//										float newVelZ2 = (m_particles[j].m_velocity.z * (p2Radius - p1Radius) +
		//												(2 * p1Radius * m_particles[i].m_velocity.z)) / (p1Radius + p2Radius);

		//										m_particles[i].m_velocity = glm::vec3(newVelX1, newVelY1, newVelZ1);
		//										m_particles[j].m_velocity = glm::vec3(newVelX2, newVelY2, newVelZ2);

		//										m_particles[i].m_position += m_particles[i].m_velocity;
		//										m_particles[j].m_position += m_particles[j].m_velocity;
		//								}
		//						}
		//				}
		//		}
		//}
}