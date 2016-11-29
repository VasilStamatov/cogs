#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>

namespace cogs
{
		namespace ecs
		{
				class Transform : public Component
				{
				public:
						Transform(const glm::vec3& _pos = glm::vec3(0.0f),
								const glm::quat& _rot = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
								const glm::vec3 _scale = glm::vec3(1.0f),
								Transform* _parent = nullptr);
						~Transform();

						//Let the component have an init function so it can be used as a constructor
						virtual void init() override;
						//Usually game components will have an Update and Draw function
						virtual void update(float _deltaTime) override;
						virtual void render() override;

						void rotate(const glm::vec3& _eulerAngles);
						void lookAt(const glm::vec3& _target, const glm::vec3& _up);
						void translate(const glm::vec3& _offset);

						//getters
						inline glm::vec3* getPosition() { return &m_position; }
						inline glm::quat* getRotation() { return &m_rotation; }
						inline glm::vec3* getScale()			 { return &m_scale; }

						inline const glm::vec3& getPosition() const noexcept { return m_position; }
						inline const glm::quat& getRotation() const noexcept { return m_rotation; }
						inline const glm::vec3& getScale()				const noexcept { return m_scale; }

					 glm::vec3 getTransformedPos() const;
						glm::quat getTransformedRot() const;
						glm::mat4 getTransformation() const;

						//setters
						inline void setPosition(const glm::vec3& _position) { m_position = _position; }
						inline void setRotation(const glm::quat& _rotation) { m_rotation = _rotation; }
						inline void setScale			(const glm::vec3& _scale)			 { m_scale = _scale; }
						inline void setParent		(Transform* _parent)								 { m_parent = _parent; }

				private:
						Transform* m_parent;

						//The transform is the result of scaling->rotating->translating
						glm::vec3 m_position;
						glm::quat m_rotation;
						glm::vec3 m_scale;
				};
		}
}


#endif // !TRANSFORM_H