#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Component.h"

#include "Color.h"

#include <functional>

namespace cogs
{
		/* Forward declarations */
		class ParticleRenderer;
		class GLTexture2D;
		class BulletDebugRenderer;
		template<typename T>
		class SpatialHash;

		struct Particle
		{
				glm::vec3 m_position{ 0.0f }; ///< a particle's position in world space
				glm::vec3 m_velocity{ 0.0f }; ///< a particle's velocity
				Color m_color{ 255, 255 }; ///< a particle's tint
				float m_radius{ 0.0f }; ///< the radius of the particle
				float m_life{ 0.0f }; ///< the life of the particle, (1.0 for 100% to 0.0 for 0%)
				float m_mass{ 1.0f }; ///< the mass of the particle, for collisions
		};

		// Default function pointer for updating a particle
		inline void defaultParticleUpdate(Particle& _particle, const glm::vec3& _gravity, float _deltaTime)
		{
				_particle.m_velocity += _gravity * _deltaTime;
				_particle.m_position += _particle.m_velocity * _deltaTime;
		}

		/** Particle System component class
		* Add this to any Entity and it can start emitting particles */
		class ParticleSystem : public Component
		{
		public:
				/**
				* \brief The component constructor 
				* @param _renderer - reference to the particle renderer which will handle rendering the particles of this emitter
				* @param _hashTable - reference to the hashtable that will store the particles emitted if collisions are enabled
				* @param _maxParticles - number of max particles being alive at one point
				* @param _particlePerSec - number of particles emitted per second
				* @param _initialSpeed - How fast are the particles when spawned
				* @param _width - how wide are the particles (radius is width / 2)
				* @param _mass - what's the mass of the particles emitted (used for collision handling)
				* @param _decayRate - How fast the particles' lifespan goes down
				* @param _additive - flag if the particles emitted are additive blended or not (will not sort if additive as it's order independent)
				* @param _collide - flag if the particles should collide with each other
				* @param _playOnInit - flag if the particle system should start emitting immediately
				* @param _worldGravity - The gravity that affects this particle system
				* @param _maxBounds - the max coorinate particles can go to 
				* @param _minBounds - the min coordinates particles can go to
				* @param _color - The tint of the particles
				* @param _texture - the texture of the particles
				* @param _updateFunc - the function called for each particle each frame to update them (if not paused)
				*/
				ParticleSystem(std::weak_ptr<ParticleRenderer> _renderer,
						std::weak_ptr<SpatialHash<Particle>> _hashTable,
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

				/**
				* \brief the first function called upon adding this component to an entity
				*/
				void init() override;

				/**
				* \brief The update function called every frame
				*/
				void update(float _deltaTime) override;

				/**
				* \brief The function that submits the particles to the particle renderer
				*/
				void render() override;

				/**
				* \brief start updating the particle system
				*/
				void play();
				/**
				* \brief stop updating the particle system
				*/
				void pause();
				/**
				* \brief continue updating, but stop emitting particles
				*/
				void stopEmitting();
				/**
				* \brief resume emitting particles
				*/
				void continueEmitting();
				/**
				* \brief inverts the playOnInit bool
				*/
				void togglePlayOnInit();

				/**
				* \brief Check if the particle system is currently playing (not paused)
				*/
				bool isPlaying();
				/**
				* \brief Check if the particle system is currently not emitting
				*/
				bool isStopped();

				/* Setters */
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

				/* Getters */
				std::weak_ptr<GLTexture2D> getTexture() const noexcept { return m_texture; }
				int getMaxParticles()																		 const noexcept { return m_maxParticles; }
				int getNumActiveParticles()											  const noexcept { return m_numActiveParticles; }
				bool getAdditive()																		    const noexcept { return m_additive; }
				Particle* getParticles()														  const noexcept { return m_particles; }

		private:
				/**
				* \brief generate the number of particles required for this frame (depends on particles per second)
				*/
				void generateParticles(float _deltaTime);
				/**
				* \brief spawns a single particle on the first free position found, unless there are no free positions
				*/
				void spawnParticle();
				/**
				* \brief spawns a single particle on the first free position found, unless there are no free positions
				*/
				void freeParticle(int _particleIndex);
				/**
				* \brief Check collisions with all neighboring particles from the hashtable
				*/
				void collideParticles();
				/**
				* \brief check if 2 particles are colliding, and resolve it
				*/
				void checkCollision(Particle * _p1, Particle * _p2);
				/**
				* \brief check collisions with the min and max bounds
				*/
				void collideWithBounds(Particle * _p);

		private:
				/** 
				* \brief custom update function for each particle system
				* @param Particle& - the particle to be updated
				* @param vec3 - the gravity applied to the particle
				* @param float - deltaTime
				*/
				std::function<void(Particle&, const glm::vec3&, float)> m_updateFunc;

				/** Dynamically allocated array of particles for cache friendliness */
				Particle* m_particles{ nullptr };

				glm::vec3 m_worldGravity{ 0.0f, 0.0f, 0.0f }; ///< gravity affecting the particles
				glm::vec3 m_maxBounds{ 0.0f, 0.0f, 0.0f }; ///< max bounds the particles can go to
				glm::vec3 m_minBounds{ 0.0f, 0.0f, 0.0f }; ///< min bounds the particles can go to

				/** The color set to the newly spawned particles */
				Color m_particlesColor{ Color::white };

				/** The speed at which the particles decay */
				float m_decayRate{ 0.1f };

				/** The radius of the particles */
				float m_particlesRadius{ 0.5f };
				
				/** The mass of the particles*/
				float m_particlesMass{ 1.0f };

				/* accumulator for the particles per second */
				float m_accumulator{ 0.0f };

				/* number of particles generated per frame */
				float m_particlesPerFrame{ 0.0f };

				/** index of the last available particle in the particle array */
				int m_lastFreeParticle{ 0 };
				/** number of max particles alive at one time */
				int m_maxParticles{ 0 };
				/** Current number of active particles in spawned */
				int m_numActiveParticles{ 0 };
				/** Initial speed of the particles when spawned */
				float m_initialSpeed{ 1.0f };

				bool m_additive{ true }; ///< flag if the particles are additively blended
				bool m_collisions{ false }; ///< flag if collisions should be checked
				bool m_isPlaying{ false }; ///< flag if the particle system is paused or playing
				bool m_isStopped{ false }; ///< flag if the particle emission is stopped or not
				bool m_playOnInit{ true }; ///< flag if the particles should start spawning on startup

				/** 2D texture for the particle */
				std::weak_ptr<GLTexture2D> m_texture;

				/** reference to the particle renderer where this particle system will submit to */
				std::weak_ptr<ParticleRenderer> m_renderer;

				/** reference to the hash table of particles this particle system will submit to (if collisions are on) */
				std::weak_ptr<SpatialHash<Particle>> m_hashTable;
		};
}

#endif // !PARTICLE_SYSTEM_H