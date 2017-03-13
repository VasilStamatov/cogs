#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Component.h"

#include "Color.h"

#include <functional>

namespace cogs
{
		class ParticleRenderer;
		class GLTexture2D;

		struct Particle
		{
				/** a particle's position in local space,
				in relation to the entity who has this particle system component*/
				glm::vec3 m_position{ 0.0f };
				glm::vec3 m_velocity{ 0.0f }; ///< a particle's velocity
				Color m_color{ 255, 255 }; ///< a particle's tint
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
				ParticleSystem(std::weak_ptr<ParticleRenderer> _renderer,
						int _maxParticles,
						float _initialSpeed,
						float _width,
						bool _additive,
						const glm::vec3& _worldGravity,
						const Color& _color,
						float _decayRate,
						std::weak_ptr<GLTexture2D> _texture,
						std::function<void(Particle&, float, float)> _updateFunc = defaultParticleUpdate);
				~ParticleSystem();

				void init() override;

				void update(float _deltaTime) override;

				void render() override;

				void setTexture(std::weak_ptr<GLTexture2D> _texture);
				void setMaxParticles(int _maxParticles);
				void setParticleLifetime(float _particleLifetime);
				void setParticlesWidth(float _width);
				void setUpdateFunc(std::function<void(Particle&, float, float)> _updateFunc);
				void setRenderer(std::weak_ptr<ParticleRenderer> _renderer);

				std::weak_ptr<GLTexture2D> getTexture() { return m_texture; }
				int getMaxParticles() { return m_maxParticles; }
				bool getAdditive() { return m_additive; }
				Particle* getParticles() { return m_particles; }

		private:
				//finds a free particle position from all possible particles (maxParticles) or overwrites the first one
				int findFreeParticle();
				void generateParticles(float _deltaTime);
				void spawnParticle();
				void sortParticles();
				//void collideParticles();

		private:
				/** custom update function for each particle system */
				std::function<void(Particle&, float, float)> m_updateFunc;

				/** Array of all particles */
				Particle* m_particles{ nullptr };

				glm::vec3 m_worldGravity{ 0.0f, 0.0f, 0.0f };

				/** The color set to the newly spawned particles */
				Color m_particlesColor{ Color::white };

				/** The speed at which the particles decay */
				float m_decayRate{ 0.1f };

				/** The width/size of the particles */
				float m_particlesWidth{ 1.0f };

				/** index of the last available particle in the particle array */
				int m_lastFreeParticle{ 0 };
				int m_maxParticles{ 0 };
				float m_initialSpeed{ 1.0f };

				bool m_additive{ true };
				//bool m_collisions{ true };

				/** 2D texture for the particle */
				std::weak_ptr<GLTexture2D> m_texture;

				std::weak_ptr<ParticleRenderer> m_renderer;
		};
}

#endif // !PARTICLE_SYSTEM_H