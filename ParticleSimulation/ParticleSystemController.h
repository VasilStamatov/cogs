#ifndef PARTICLE_SYSTEM_CONTROLLER_H
#define PARTICLE_SYSTEM_CONTROLLER_H

#include <cogs\Entity.h>
#include <cogs\Component.h>
#include <cogs\Input.h>
#include <cogs\ParticleSystem.h>

class ParticleSystemController : public cogs::Component
{
public:
		ParticleSystemController(const cogs::KeyCode& _playKey, const cogs::KeyCode& _stopKey) :
				m_playKey(_playKey),
				m_stopKey(_stopKey)
		{

		}
		~ParticleSystemController()
		{

		}

		void init() override
		{
				m_particleSystem = m_entity.lock()->getComponent<cogs::ParticleSystem>();
		}

		void update(float _deltaTime) override
		{
				if (cogs::Input::isKeyPressed(m_playKey))
				{
						if (m_particleSystem.lock()->isPlaying())
						{
								m_particleSystem.lock()->pause();
						}
						else
						{
								m_particleSystem.lock()->play();
						}
				}
				if (cogs::Input::isKeyPressed(m_stopKey))
				{
						if (m_particleSystem.lock()->isStopped())
						{
								m_particleSystem.lock()->continueEmitting();
						}
						else
						{
								m_particleSystem.lock()->stopEmitting();
						}
				}
		}

		void render() override
		{

		}

private:
		std::weak_ptr<cogs::ParticleSystem> m_particleSystem;
		cogs::KeyCode m_playKey;
		cogs::KeyCode m_stopKey;
};
#endif // !PARTICLE_SYSTEM_CONTROLLER_H