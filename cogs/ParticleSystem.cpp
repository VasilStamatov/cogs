#include "ParticleSystem.h"

#include "Entity.h"
#include "Camera.h"
#include "Random.h"
#include "GLTexture2D.h"
#include "ParticleRenderer.h"
#include "SpatialHash.h"
#include "BulletDebugRenderer.h"

#include <glm\gtx\norm.hpp>

namespace cogs
{
		ParticleSystem::ParticleSystem(std::weak_ptr<ParticleRenderer> _renderer,
				std::weak_ptr<SpatialHash> _hashTable,
				int _maxParticles,
				float _particlePerSec,
				float _initialSpeed,
				float _width,
				float _mass,
				float _decayRate,
				bool _additive,
				bool _collide,
				bool _playOnInit,
				const glm::vec3& _worldGravity,
				const glm::vec3& _maxBounds,
				const glm::vec3& _minBounds,
				const Color& _color,
				std::weak_ptr<GLTexture2D> _texture,
				std::function<void(Particle&, const glm::vec3&, float)> _updateFunc) :
				m_hashTable(_hashTable),
				m_maxParticles(_maxParticles),
				m_particlesPerFrame(1.0f / _particlePerSec),
				m_initialSpeed(_initialSpeed),
				m_particlesRadius(_width * 0.5f),
				m_particlesMass(_mass),
				m_decayRate(_decayRate),
				m_additive(_additive),
				m_collisions(_collide),
				m_playOnInit(_playOnInit),
				m_worldGravity(_worldGravity),
				m_maxBounds(_maxBounds),
				m_minBounds(_minBounds),
				m_particlesColor(_color),
				m_renderer(_renderer),
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
				if (m_playOnInit)
				{
						play();
				}
		}

		void ParticleSystem::update(float _deltaTime)
		{
				if (m_isPlaying)
				{
						if (!m_isStopped)
						{
								generateParticles(_deltaTime);
						}
						m_numActiveParticles = 0;
						for (int i = 0; i < m_maxParticles; ++i)
						{
								// check if it is active
								if (m_particles[i].m_life > 0.0f)
								{
										m_numActiveParticles++;
										// Update using function pointer
										m_updateFunc(m_particles[i], m_worldGravity, _deltaTime);

										//reduce its life by decay amount with frame independence
										m_particles[i].m_life -= (m_decayRate * _deltaTime);

										if (m_collisions)
										{
												collideWithBounds(&m_particles[i]);
												m_hashTable.lock()->addParticle(&m_particles[i]);
										}
								}
						}

						if (m_collisions)
						{
								collideParticles();
						}
				}
		}

		void ParticleSystem::render()
		{
				m_renderer.lock()->submit(m_entity);
		}

		void ParticleSystem::play()
		{
				m_isPlaying = true;
		}

		void ParticleSystem::pause()
		{
				m_isPlaying = false;
		}

		void ParticleSystem::continueEmitting()
		{
				m_isStopped = false;
		}

		void ParticleSystem::stopEmitting()
		{
				m_isStopped = true;
		}

		bool ParticleSystem::isPlaying()
		{
				return m_isPlaying;
		}

		bool ParticleSystem::isStopped()
		{
				return m_isStopped;
		}

		void ParticleSystem::setTexture(std::weak_ptr<GLTexture2D> _texture)
		{
				m_texture = _texture;
		}

		void ParticleSystem::setDecayRate(float _decayRate)
		{
				m_decayRate = _decayRate;
		}

		void ParticleSystem::setParticlesWidth(float _width)
		{
				m_particlesRadius = _width * 0.5f;
		}

		void ParticleSystem::setInitialSpeed(float _initialSpeed)
		{
				m_initialSpeed = _initialSpeed;
		}

		void ParticleSystem::setCollide(bool _flag)
		{
				m_collisions = _flag;
		}

		void ParticleSystem::setAdditive(bool _flag)
		{
				m_additive = m_additive;
		}

		void ParticleSystem::setWorldGravity(const glm::vec3 & _gravity)
		{
				m_worldGravity = _gravity;
		}

		void ParticleSystem::setMaxBounds(const glm::vec3 & _bounds)
		{
				m_maxBounds = _bounds;
		}

		void ParticleSystem::setMinBounds(const glm::vec3 & _bounds)
		{
				m_minBounds = _bounds;
		}

		void ParticleSystem::setUpdateFunc(std::function<void(Particle&, const glm::vec3&, float)> _updateFunc)
		{
				m_updateFunc = _updateFunc;
		}

		void ParticleSystem::setRenderer(std::weak_ptr<ParticleRenderer> _renderer)
		{
				m_renderer = _renderer;
		}

		void ParticleSystem::renderBounds(BulletDebugRenderer * _debugRenderer)
		{
				//floor
				_debugRenderer->drawLine(btVector3(m_minBounds.x, m_minBounds.y, m_minBounds.z),
						btVector3(m_maxBounds.x, m_minBounds.y, m_minBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_maxBounds.x, m_minBounds.y, m_minBounds.z),
						btVector3(m_maxBounds.x, m_minBounds.y, m_maxBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_maxBounds.x, m_minBounds.y, m_maxBounds.z),
						btVector3(m_minBounds.x, m_minBounds.y, m_maxBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_minBounds.x, m_minBounds.y, m_maxBounds.z),
						btVector3(m_minBounds.x, m_minBounds.y, m_minBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				//right wall
				_debugRenderer->drawLine(btVector3(m_maxBounds.x, m_minBounds.y, m_minBounds.z),
						btVector3(m_maxBounds.x, m_maxBounds.y, m_minBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_maxBounds.x, m_maxBounds.y, m_minBounds.z),
						btVector3(m_maxBounds.x, m_maxBounds.y, m_maxBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_maxBounds.x, m_maxBounds.y, m_maxBounds.z),
						btVector3(m_maxBounds.x, m_minBounds.y, m_maxBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				//left wall
				_debugRenderer->drawLine(btVector3(m_minBounds.x, m_minBounds.y, m_minBounds.z),
						btVector3(m_minBounds.x, m_maxBounds.y, m_minBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_minBounds.x, m_maxBounds.y, m_minBounds.z),
						btVector3(m_minBounds.x, m_maxBounds.y, m_maxBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_minBounds.x, m_maxBounds.y, m_maxBounds.z),
						btVector3(m_minBounds.x, m_minBounds.y, m_maxBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				//ceiling
				_debugRenderer->drawLine(btVector3(m_minBounds.x, m_maxBounds.y, m_minBounds.z),
						btVector3(m_maxBounds.x, m_maxBounds.y, m_minBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_maxBounds.x, m_maxBounds.y, m_minBounds.z),
						btVector3(m_maxBounds.x, m_maxBounds.y, m_maxBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_maxBounds.x, m_maxBounds.y, m_maxBounds.z),
						btVector3(m_minBounds.x, m_maxBounds.y, m_maxBounds.z), btVector3(1.0f, 0.0f, 0.0f));

				_debugRenderer->drawLine(btVector3(m_minBounds.x, m_maxBounds.y, m_maxBounds.z),
						btVector3(m_minBounds.x, m_maxBounds.y, m_minBounds.z), btVector3(1.0f, 0.0f, 0.0f));
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
				// no particles are free
				return m_maxParticles + 1;
		}

		void ParticleSystem::generateParticles(float _deltaTime)
		{
				// limit the particles per sec accumulation to 60 fps minimum
				// otherwise if it goes too low it will start spawning massive amounts at once
				// and make it even slower
				if (_deltaTime > 0.016f)
				{
						_deltaTime = 0.016f;
				}
				m_accumulator += _deltaTime;
				while (m_accumulator > m_particlesPerFrame)
				{
						spawnParticle();
						m_accumulator -= m_particlesPerFrame;
				}
		}

		void ParticleSystem::spawnParticle()
		{
				int particleIndex = findFreeParticle();

				if (particleIndex > m_maxParticles)
				{
						return;
				}

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
				particle.m_position = m_entity.lock()->getComponent<Transform>().lock()->worldPosition() + (Random::getRandFloat(-2.0f, 2.0f));

				//set its velocity to the emitting velocity
				//particle.m_velocity = glm::vec3(1.0f, 0.0f, 0.0f) * m_initialSpeed;
				particle.m_velocity = vel;

				//set its color to the set color for all particles
				particle.m_color = m_particlesColor;

				particle.m_radius = m_particlesRadius;

				particle.m_mass = m_particlesMass;
		}

		void ParticleSystem::collideParticles()
		{
				std::vector<Particle*> checkedParticles;

				for (int i = 0; i < m_maxParticles; i++)
				{
						if (m_particles[i].m_life > 0.0f)
						{
								checkedParticles.push_back(&m_particles[i]);

								std::vector<Particle*> neighbors = m_hashTable.lock()->getNeighbors(&m_particles[i]);

								for (Particle* neighbor : neighbors)
								{
										if (std::find(checkedParticles.begin(), checkedParticles.end(), neighbor) == checkedParticles.end())
										{
												checkCollision(&m_particles[i], neighbor);
										}
								}
						}
				}
		}

		void ParticleSystem::checkCollision(Particle * _p1, Particle * _p2)
		{
				glm::vec3 distVec = _p2->m_position - _p1->m_position;
				glm::vec3 distDir = glm::normalize(distVec);
				float dist = glm::length2(distVec);
				float totalRadius = _p1->m_radius + _p2->m_radius;

				float collisionDepth = totalRadius * totalRadius - dist;
				// Check for collision
				if (collisionDepth > 0.0f)
				{
						// Push away the balls based on ratio of masses
						_p1->m_position -= distDir * collisionDepth * (_p2->m_mass / _p1->m_mass) * 0.5f;
						_p2->m_position += distDir * collisionDepth * (_p1->m_mass / _p2->m_mass) * 0.5f;

						// Calculate deflection. http://stackoverflow.com/a/345863
						float aci = glm::dot(_p1->m_velocity, distDir);
						float bci = glm::dot(_p2->m_velocity, distDir);

						float acf = (aci * (_p1->m_mass - _p2->m_mass) + 2 * _p2->m_mass * bci) / (_p1->m_mass + _p2->m_mass);
						float bcf = (bci * (_p2->m_mass - _p1->m_mass) + 2 * _p1->m_mass * aci) / (_p1->m_mass + _p2->m_mass);

						_p1->m_velocity += (acf - aci) * distDir;
						_p2->m_velocity += (bcf - bci) * distDir;

						if (glm::length2(_p1->m_velocity + _p2->m_velocity) > 1.0f)
						{
								// Choose the faster ball
								bool choice = glm::length2(_p1->m_velocity) < glm::length2(_p2->m_velocity);

								// Faster ball transfers it's color to the slower ball
								choice ? _p2->m_color = _p1->m_color : _p1->m_color = _p2->m_color;
						}
				}
		}

		void ParticleSystem::collideWithBounds(Particle * _p)
		{
				if (_p->m_position.x < m_minBounds.x + _p->m_radius)
				{
						_p->m_position.x = m_minBounds.x + _p->m_radius;
						if (_p->m_velocity.x < 0.0f)
						{
								_p->m_velocity.x *= -1.0f;
						}
				}
				else if (_p->m_position.x > m_maxBounds.x - _p->m_radius)
				{
						_p->m_position.x = m_maxBounds.x - _p->m_radius;
						if (_p->m_velocity.x > 0.0f)
						{
								_p->m_velocity.x *= -1.0f;
						}
				}

				if (_p->m_position.y < m_minBounds.y + _p->m_radius)
				{
						_p->m_position.y = m_minBounds.y + _p->m_radius;
						if (_p->m_velocity.y < 0.0f)
						{
								_p->m_velocity.y *= -1.0f;
						}
				}
				else if (_p->m_position.y > m_maxBounds.y - _p->m_radius)
				{
						_p->m_position.y = m_maxBounds.y - _p->m_radius;
						if (_p->m_velocity.y > 0.0f)
						{
								_p->m_velocity.y *= -1.0f;
						}
				}

				if (_p->m_position.z < m_minBounds.z + _p->m_radius)
				{
						_p->m_position.z = m_minBounds.z + _p->m_radius;
						if (_p->m_velocity.z < 0.0f)
						{
								_p->m_velocity.z *= -1.0f;
						}
				}
				else if (_p->m_position.z > m_maxBounds.z - _p->m_radius)
				{
						_p->m_position.z = m_maxBounds.z - _p->m_radius;
						if (_p->m_velocity.z > 0.0f)
						{
								_p->m_velocity.z *= -1.0f;
						}
				}
		}
}