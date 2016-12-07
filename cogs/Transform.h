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
								const glm::vec3& _eulerAngles = glm::vec3(0.0f, 0.0f, 0.0f),
								const glm::vec3& _scale = glm::vec3(1.0f),
								Transform* _parent = nullptr);
						~Transform();

						void init() override;
						void update(float _deltaTime) override;
						void render() override;

						/**
						 	*	\brief rotates the transform of the entity by euler angles in degrees
						 	* \param _eulerAngles - vec3 euler angles in degrees to be added
						 	*/
						void rotate(const glm::vec3& _eulerAngles);

						/**
						  *	\brief sets the orientation of the transform to be looking at the set target
						  * \param _target - the vec3 target the orientation should be looking at
						  */
						void lookAt(const glm::vec3& _target);

						/**
						  *	\brief translates the position of the transform by _offset
						  * \param _offset - the vec3 offset to add
						  */
						void translate(const glm::vec3& _offset);

						/**
						  *	\brief scales the local scale of the transform by _offset
						  * \param _offset - the vec3 offset to add
						  */
						void scale(const glm::vec3& _offset);

						/**
								*	\brief local space getters
								*/
						inline const glm::vec3& localPosition()			 const noexcept { return m_localPosition; }
						inline const glm::vec3& localOrientation() const noexcept { return m_localOrientation; }
						inline const glm::vec3& localScale()						 const noexcept { return m_localScale; }

						glm::mat4 localTransform()		 const;

						/**
								*	\brief world space getters
								*/
						inline const glm::vec3& worldPosition()			 const noexcept { return m_worldPosition; }
						inline const glm::vec3& worldOrientation() const noexcept { return m_worldOrientation; }
						inline const glm::vec3& worldScale()							const noexcept { return m_worldScale; }
						inline						 glm::vec3 origin()											 const noexcept { return m_worldPosition + (m_worldScale * 0.5f); }

						glm::mat4 worldTransform()		 const;

						/**
								*	\brief Getters for the axis of the transform
								* \return the direction vec3
								*/
						glm::vec3 localRightAxis()		 const noexcept { return m_localOrientationRaw * glm::vec3(1.0f, 0.0f, 0.0f); }
						glm::vec3 localUpAxis()					 const noexcept { return m_localOrientationRaw * glm::vec3(0.0f, 1.0f, 0.0f); }
						glm::vec3 localForwardAxis() const noexcept { return m_localOrientationRaw * glm::vec3(0.0f, 0.0f, 1.0f); }

						glm::vec3 worldRightAxis()		 const noexcept { return m_worldOrientationRaw * glm::vec3(1.0f, 0.0f, 0.0f); }
						glm::vec3 worldUpAxis()					 const noexcept { return m_worldOrientationRaw * glm::vec3(0.0f, 1.0f, 0.0f); }
						glm::vec3 worldForwardAxis() const noexcept { return m_worldOrientationRaw * glm::vec3(0.0f, 0.0f, -1.0f); }

						//setters
						inline void setLocalOrientation(const glm::quat& _value) { internal_setLocalOrientation(_value); }
						inline void setLocalOrientation(const glm::vec3& _value) { internal_setLocalOrientation(_value); }
						inline void setLocalPosition		 (const glm::vec3& _value)	{ internal_setLocalPosition		 (_value); }
						inline void setLocalScale					 (const glm::vec3& _value)	{ internal_setLocalScale					 (_value); }

						inline void setWorldOrientation(const glm::quat& _value) { internal_setWorldOrientation(_value); }
						inline void setWorldOrientation(const glm::vec3& _value) { internal_setWorldOrientation(_value); }
						inline void setWorldPosition		 (const glm::vec3& _value)	{ internal_setWorldPosition		 (_value); }
						inline void setWorldScale					 (const glm::vec3& _value)	{ internal_setWorldScale					 (_value); }

						inline void setParent									 (Transform* _parent)						{ m_parent = _parent; }
						inline Transform* getParent()		const noexcept											 { if (m_parent) return m_parent; else return nullptr; }

						bool operator== (const Transform& _rhs) const;

				private:
						/**
								* Internal functions for setting the transform attributes
								*/
						void internal_setLocalOrientation(const glm::quat& _value);
						void internal_setLocalOrientation(const glm::vec3& _value);
						void internal_setLocalPosition		 (const glm::vec3& _value);
						void internal_setLocalScale					 (const glm::vec3& _value);
																																																																				
						void internal_setWorldOrientation(const glm::quat& _value);
						void internal_setWorldOrientation(const glm::vec3& _value);
						void internal_setWorldPosition		 (const glm::vec3& _value);
						void internal_setWorldScale					 (const glm::vec3& _value);

						void internal_updateTransform();

				private:
						Transform* m_parent;

						//The transform is the result of scaling->rotating->translating
						glm::vec3 m_worldPosition;
						glm::vec3 m_worldScale;
						glm::vec3 m_worldOrientation;
						glm::quat m_worldOrientationRaw;

						glm::vec3 m_localPosition;
						glm::vec3 m_localScale;
						glm::vec3 m_localOrientation;
						glm::quat m_localOrientationRaw;
				};
		}
}


#endif // !TRANSFORM_H