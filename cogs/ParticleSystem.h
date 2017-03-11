#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Component.h"

#include "Color.h"
#include "GLTexture2D.h"
#include "ParticleRenderer.h"

#include <functional>

namespace cogs
{
		namespace ecs
		{
				struct Particle
				{
						/** a particle's position in local space,
						in relation to the entity who has this particle system component*/
						glm::vec3 m_position{ 0.0f };
						glm::vec3 m_velocity{ 0.0f }; ///< a particle's velocity
						graphics::Color m_color{ 255, 255 }; ///< a particle's tint
						float m_width{ 0.0f }; ///< the width of the particle
						float m_life{ 0.0f }; ///< the life of the particle,
				};
				// Default function pointer
				inline void defaultParticleUpdate(Particle& _particle, float _gravity, float _deltaTime)
				{
						_particle.m_velocity.y += _gravity * _deltaTime;
						_particle.m_position += _particle.m_velocity * _deltaTime;
				}

				class ParticleSystem : public Component
				{
				public:
						ParticleSystem(std::weak_ptr<graphics::ParticleRenderer> _renderer,
								int _maxParticles,
								float _initialSpeed,
								float _pps,
								float _width,
								const glm::vec3& _worldGravity,
								const graphics::Color& _color,
								float _decayRate,
								std::weak_ptr<graphics::GLTexture2D> _texture,
								std::function<void(Particle&, float, float)> _updateFunc = defaultParticleUpdate);
						~ParticleSystem();

						void init() override;

						void update(float _deltaTime) override;

						void render() override;

						void setTexture(std::weak_ptr<graphics::GLTexture2D> _texture);
						void setMaxParticles(int _maxParticles);
						void setParticleLifetime(float _particleLifetime);
						void setParticlesWidth(float _width);
						void setUpdateFunc(std::function<void(Particle&, float, float)> _updateFunc);
						void setRenderer(std::weak_ptr<graphics::ParticleRenderer> _renderer);

						std::weak_ptr<graphics::GLTexture2D> getTexture() { return m_texture; }
						int getMaxParticles() { return m_maxParticles; }
					 Particle* getParticles() { return m_particles; }
				private:
						//finds a free particle position from all possible particles (maxParticles) or overwrites the first one
						int findFreeParticle();
						void generateParticles(float _deltaTime);
						void spawnParticle();

				private:
						/** custom update function for each particle system */
						std::function<void(Particle&, float, float)> m_updateFunc;

						/** Array of all particles */
						Particle* m_particles{ nullptr };

						glm::vec3 m_worldGravity{ 0.0f, 0.0f, 0.0f };

						/** The color set to the newly spawned particles */
						graphics::Color m_particlesColor{ graphics::Color::white };

						/** The speed at which the particles decay */
						float m_decayRate{ 0.1f };
						
						/** The width/size of the particles */
						float m_particlesWidth{ 1.0f };

						/** index of the last available particle in the particle array */
						int m_lastFreeParticle{ 0 };
						int m_maxParticles{ 0 };
						float m_initialSpeed{ 1.0f };

						float m_particlesPerSec{ 0.0f };
						/** 2D texture for the particle */
						std::weak_ptr<graphics::GLTexture2D> m_texture;

						std::weak_ptr<graphics::ParticleRenderer> m_renderer;
				};
		}
}

#endif // !PARTICLE_SYSTEM_H