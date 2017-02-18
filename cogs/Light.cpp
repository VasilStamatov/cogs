#include "Light.h"
#include "Entity.h"

namespace cogs
{
		namespace ecs
		{
				std::vector<std::weak_ptr<Light>> Light::s_allLights;

				Light::Light()
				{
				}
				Light::~Light()
				{
						//s_allLights.erase(s_allLights.begin() + m_lightIndex);
				}
				void Light::init()
				{
						m_transform = m_entity.lock()->getComponent<Transform>();

						//m_lightIndex = s_allLights.size();
						s_allLights.push_back(m_entity.lock()->getComponent<Light>());
				}
				void Light::update(float _deltaTime)
				{
				}
			 glm::vec3 Light::getPosition()
				{
						return m_transform.lock()->worldPosition();
				}
			 glm::vec3 Light::getDirection()
				{
						return m_transform.lock()->worldForwardAxis();
				}
		}
}