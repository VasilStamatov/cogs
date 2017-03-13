#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"

#include <vector>

namespace cogs
{
		class Transform;

		/**
		* \brief The types of light supported
		*/
		enum class LightType
		{
				DIRECTIONAL,
				POINT,
				SPOT
		};

		/**
		* \brief The attenuation of lights
		*/
		struct Attenuation
		{
				Attenuation() {}
				Attenuation(float _constant, float _linear, float _quadratic) :
						m_constant(_constant), m_linear(_linear), m_quadratic(_quadratic) {}
				float m_constant{ 0.0f };
				float m_linear{ 0.0f };
				float m_quadratic{ 0.0f };
		};

		/**
		* \brief Light component
		*/
		class Light : public Component
		{
		public:
				Light();
				~Light();

				/**
				* \brief initialize the light
				*/
				void init() override;

				/**
				* \brief per-frame update
				*/
				void update(float _deltaTime) override;

				//attribute setters
				void setLightType(const LightType& _lightType) { m_lightType = _lightType; }
				void setAttenuation(const Attenuation& _attenuation) { m_attenuation = _attenuation; }
				void setColor(const glm::vec3& _color) { m_lightColor = _color; }
				void setAmbientIntensity(float _intensity) { m_ambientIntensity = _intensity; }
				void setDiffuseIntensity(float _intensity) { m_diffuseIntensity = _intensity; }
				void setSpecularIntensity(float _intensity) { m_specularIntensity = _intensity; }
				void setCutOff(float _cutOff) { m_cutOff = _cutOff; }
				void setOuterCutOff(float _outerCutOff) { m_outerCutOff = _outerCutOff; }

				//attribute getters
				const LightType& getLightType()		   const { return m_lightType; }
				const Attenuation& getAttenuation()	const { return m_attenuation; }
				const glm::vec3& getColor()  const noexcept { return m_lightColor; }
				float getAmbientIntensity()  const noexcept { return m_ambientIntensity; }
				float getDiffuseIntensity()  const noexcept { return m_diffuseIntensity; }
				float getSpecularIntensity() const noexcept { return m_specularIntensity; }
				float getCutOff()											 const noexcept { return m_cutOff; }
				float getOuterCutOff()							const noexcept { return m_outerCutOff; }
				glm::vec3 getPosition();
				glm::vec3 getDirection();

				//getter of all the lights created
				static std::vector<std::weak_ptr<Light>> getAllLights() { return s_allLights; }

		private:
				static std::vector<std::weak_ptr<Light>> s_allLights; ///< static container of all the lights

				std::weak_ptr<Transform> m_transform; ///< transform reference of the entity
				LightType			m_lightType{ LightType::POINT }; ///< the type of light
				Attenuation m_attenuation; ///< attenuation of the light
				glm::vec3			m_lightColor{ 1.0f, 1.0f, 1.0f }; ///< color of the light
				float					  m_ambientIntensity{ 1.0f }; ///< ambient intensity of the light
				float					  m_diffuseIntensity{ 1.0f };  ///< diffuse intensity of the light
				float					  m_specularIntensity{ 1.0f }; ///< specular intensity of the light
				float					  m_cutOff{ 0.0f }; ///< inner cutoff of specular type lights
				float					  m_outerCutOff{ 0.0f }; ///< outer cutoff of specular type lights
				//int									m_lightIndex{ 0 };
		};
}

#endif // !LIGHT_H