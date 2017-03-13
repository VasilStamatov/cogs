#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include <glm\gtc\quaternion.hpp>

namespace cogs
{
		/**
		* \brief The transform component
		*/
		class Transform : public Component
		{
		public:
				Transform() {}
				/**
				*	\brief Constructs the transform using positon, orientation and scale
				* \param _pos - the world vec3 coordinates (set to origin (0,0,0) by default
				* \param _eulerAngles - the orientation on each axis in radians (set to 0 on each axis by default)
				* \param _scale - the scale on each axis (set to 1 by default)
				*/
				Transform(const glm::vec3& _pos,
						const glm::vec3& _eulerAngles,
						const glm::vec3& _scale);
				~Transform();

				void init() override;
				void update(float _deltaTime) override;

				/**
				*	\brief rotates the transform of the entity by euler angles in radians
				* \param _eulerAngles - vec3 euler angles in radians to be added
				*/
				void rotate(const glm::vec3& _eulerAngles);
				void rotate(float _x, float _y, float _z);

				/**
				*	\brief rotates the transform of the entity around an axis by angle in radians
				* \param _angle - vec3 euler angles in radians to be added
				*/
				void rotate(const glm::vec3& _axis, float _angle);
				void rotate(float _x, float _y, float _z, float _angle);

				/**
				*	\brief rotates the transform of the entity by quat concatenation
				* \param _rotationQuat - the rotation quat to be applied
				*/
				void rotate(const glm::quat& _rotationQuat);

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
				void translate(float _x, float _y, float _z);

				/**
				*	\brief scales the local scale of the transform by _offset
				* \param _offset - the vec3 offset to add
				*/
				void offsetScale(const glm::vec3& _offset);
				void offsetScale(float _x, float _y, float _z);

				/**
				*	\brief returns the local space position
				*/
				inline const glm::vec3& localPosition()	const noexcept { return m_localPosition; }

				/**
				*	\brief returns the local space orientation in euler angles (radians)
				*/
				inline const glm::vec3& localOrientation() const noexcept { return m_localOrientation; }

				/**
				*	\brief returns the local space scale
				*/
				inline const glm::vec3& localScale() const noexcept { return m_localScale; }

				/**
				*	\brief returns the local space model matrix
				*/
				glm::mat4 localTransform()	const;

				/**
				*	\brief returns the world space position
				*/
				glm::vec3 worldPosition() const noexcept;

				/**
				*	\brief returns the world space orientation in euler angles (radians)
				*/
				glm::vec3 worldOrientation() const noexcept;

				/**
				*	\brief returns the world space orientation as a quat
				*/
				glm::quat worldOrientationRaw() const noexcept;

				/**
				*	\brief returns the world space scale
				*/
				glm::vec3 worldScale()	const noexcept;

				/**
				*	\brief returns the world space model matrix
				*/
				glm::mat4 worldTransform() const;

				/**
				*	\brief Returns the right axis using the local space coordinates (direction to the right (positive x axis))
				*/
				glm::vec3 localRightAxis() const noexcept { return m_localOrientationRaw * glm::vec3(1.0f, 0.0f, 0.0f); }

				/**
				*	\brief Returns the up axis using the local space coordinates (direction pointing up (positive y axis))
				*/
				glm::vec3 localUpAxis()	const noexcept { return m_localOrientationRaw * glm::vec3(0.0f, 1.0f, 0.0f); }

				/**
				*	\brief Returns the forward axis using the local space coordinates
				* (direction pointing forwards (to the negative z axis as opengl is using right handed coordinate system))
				*/
				glm::vec3 localForwardAxis() const noexcept { return m_localOrientationRaw * glm::vec3(0.0f, 0.0f, -1.0f); }

				/**
				*	\brief Returns the right axis using the world space coordinates (direction to the right (positive x axis))
				*/
				glm::vec3 worldRightAxis() const noexcept { return worldOrientationRaw() * glm::vec3(1.0f, 0.0f, 0.0f); }

				/**
				*	\brief Returns the up axis using the world space coordinates (direction pointing up (positive y axis))
				*/
				glm::vec3 worldUpAxis()	const noexcept { return worldOrientationRaw() * glm::vec3(0.0f, 1.0f, 0.0f); }

				/**
				*	\brief Returns the forward axis using the world space coordinates
				* (direction pointing forwards (to the negative z axis as opengl is using right handed coordinate system))
				*/
				glm::vec3 worldForwardAxis() const noexcept { return worldOrientationRaw() * glm::vec3(0.0f, 0.0f, -1.0f); }

				/**
				*	\brief setters for local transform
				*/
				void setLocalOrientation(const glm::quat& _value);
				void setLocalOrientation(const glm::vec3& _value);
				void setLocalPosition(const glm::vec3& _value);
				void setLocalScale(const glm::vec3& _value);

				/**
				*	\brief setters for world transform
				*/
				void setWorldOrientation(const glm::quat& _value);
				void setWorldOrientation(const glm::vec3& _value);
				void setWorldPosition(const glm::vec3& _value);
				void setWorldScale(const glm::vec3& _value);

				/**
				*	\brief Getter and setter for the parent
				*/
				void setParent(std::weak_ptr<Transform> _parent);
				inline std::weak_ptr<Transform> getParent()		const noexcept { return m_parent; }

				//operator overload to check if 2 transforms are equal
				bool operator== (const Transform& _rhs) const;

		private:
				std::weak_ptr<Transform> m_parent; ///< the parent transform of this transform

				glm::vec3 m_localPosition{ 0.0f, 0.0f, 0.0f }; ///< local position
				glm::vec3 m_localScale{ 1.0f, 1.0f, 1.0f }; ///< local scale
				glm::vec3 m_localOrientation{ 0.0f, 0.0f, 0.0f }; ///< local orientation in euler angles in radians
				glm::quat m_localOrientationRaw{ glm::vec3(0.0f, 0.0f, 0.0f) }; ///< local orientation as a quaternion
		};
}
#endif // !TRANSFORM_H