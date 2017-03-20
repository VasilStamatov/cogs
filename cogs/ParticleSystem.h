#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Component.h"

#include "Color.h"

#include <functional>

namespace cogs
{
		class ParticleRenderer;
		class GLTexture2D;
		class SpatialHash;
		class BulletDebugRenderer;

		struct Particle
		{
				/** a particle's position in local space,
				in relation to the entity who has this particle system component*/
				glm::vec3 m_position{ 0.0f };
				glm::vec3 m_velocity{ 0.0f }; ///< a particle's velocity
				Color m_color{ 255, 255 }; ///< a particle's tint
				float m_radius{ 0.0f }; ///< the width of the particle
				float m_life{ 0.0f }; ///< the life of the particle,
				float m_mass{ 1.0f }; ///< the mass of the particle
		};
		// Default function pointer
		inline void defaultParticleUpdate(Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		}

		class ParticleSystem : public Component
		{
		public:
				ParticleSystem(std::weak_ptr<ParticleRenderer> _renderer,
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
						std::function<void(Particle&, const glm::vec3&, float)> _updateFunc = defaultParticleUpdate);
				~ParticleSystem();

				void init() override;

				void update(float _deltaTime) override;

				void render() override;

				void play();
				void pause();
				void stopEmitting();
				void continueEmitting();

				bool isPlaying();
				bool isStopped();

				void setTexture(std::weak_ptr<GLTexture2D> _texture);
				void setDecayRate(float _decayRate);
				void setParticlesWidth(float _width);
				void setInitialSpeed(float _initialSpeed);
				void setCollide(bool _flag);
				void setAdditive(bool _flag);
				void setWorldGravity(const glm::vec3& _gravity);
				void setMaxBounds(const glm::vec3& _bounds);
				void setMinBounds(const glm::vec3& _bounds);
				void setUpdateFunc(std::function<void(Particle&, const glm::vec3&, float)> _updateFunc);
				void setRenderer(std::weak_ptr<ParticleRenderer> _renderer);
				void renderBounds(BulletDebugRenderer* _debugRenderer);

				std::weak_ptr<GLTexture2D> getTexture() { return m_texture; }
				int getMaxParticles() { return m_maxParticles; }
				int getNumActiveParticles() { return m_numActiveParticles; }
				bool getAdditive() { return m_additive; }
				Particle* getParticles() { return m_particles; }

		private:
				//finds a free particle position from all possible particles (maxParticles) or overwrites the first one
				int findFreeParticle();
				void generateParticles(float _deltaTime);
				void spawnParticle();
				void collideParticles();
				void checkCollision(Particle * _p1, Particle * _p2);
				void collideWithBounds(Particle * _p);

		private:
				/** custom update function for each particle system */
				std::function<void(Particle&, const glm::vec3&, float)> m_updateFunc;

				/** Array of all particles */
				Particle* m_particles{ nullptr };

				glm::vec3 m_worldGravity{ 0.0f, 0.0f, 0.0f };
				glm::vec3 m_maxBounds{ 0.0f, 0.0f, 0.0f };
				glm::vec3 m_minBounds{ 0.0f, 0.0f, 0.0f };

				/** The color set to the newly spawned particles */
				Color m_particlesColor{ Color::white };

				/** The speed at which the particles decay */
				float m_decayRate{ 0.1f };

				/** The width/size of the particles */
				float m_particlesRadius{ 0.5f };
				
				/** The mass of the particles*/
				float m_particlesMass{ 1.0f };

				/* accumulator for the particles per second */
				float m_accumulator{ 0.0f };

				/* number of particles generated per second */
				float m_particlesPerFrame{ 0.0f };

				/** index of the last available particle in the particle array */
				int m_lastFreeParticle{ 0 };
				int m_maxParticles{ 0 };
				int m_numActiveParticles{ 0 };
				float m_initialSpeed{ 1.0f };

				bool m_additive{ true };
				bool m_collisions{ false };
				bool m_isPlaying{ false };
				bool m_isStopped{ false };
				bool m_playOnInit{ true };

				/** 2D texture for the particle */
				std::weak_ptr<GLTexture2D> m_texture;

				std::weak_ptr<ParticleRenderer> m_renderer;

				std::weak_ptr<SpatialHash> m_hashTable;
		};
}

#endif // !PARTICLE_SYSTEM_H