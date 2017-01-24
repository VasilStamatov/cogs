#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"
#include "CMotionState.h"
#include <memory>

namespace cogs
{
		namespace ecs
		{
				/**
				* This component, given to an entity will give it a rigidbody,
				constructed from information that should already exist in the entity such as collider shape,
				and will add it to the physics world
				*/
				class RigidBody : public Component
				{
				public:
						/**
						* Pass the mass that this entity should be (keep it between 0.0f and 1.0f)
						*/
						RigidBody(float _mass = 1.0f);
						~RigidBody();

						/**
						* The initialize component function
						*/
						void init() override;

						/**
						* The update component function
						*/
						void update(float _deltaTime) override;

						/**
						* The render component function
						*/
						void render() override;

						/**
						* Translates the body by a certain amount
						*/
						void translate(const glm::vec3& _offset);

						/**
						* applies a force to the origin of the body
						*/
						void applyCentralForce(const glm::vec3& _force);

						/**
						* applies a force to the body from a specific relative position
						*/
						void applyForce(const glm::vec3& _force, const glm::vec3 & _relativePos);

						/**
						* Makes the rigid body move
						*/
						void setLinearVelocity(const glm::vec3& _offset);

						/**
						* Makes the rigid body spin
						*/
						void setAngularVelocity(const glm::vec3& _offset);
						
						/**
						* The spring or bounciness of a rigid body
						*/
						void setRestitution(float _restitution);

						/**
						* Locks linear movement to specific axis
						*/
						void setLinearFactor(const glm::vec3& _offset);

						/**
						* Locks angular movement to specific axis
						*/
						void setAngularFactor(const glm::vec3& _offset);

						/**
						* unknown
						*/
						void setDamping(float _x, float _y);

						/**
						* Sets the rigidbody transform to equal the transform component's world transform
						*/
						void setWorldTransform();

						/**
						* Getters
						*/
						std::weak_ptr<btRigidBody> getRigidBody()											  const { return m_rigidBody; }
						std::weak_ptr<physics::CMotionState> getMotionState() const { return m_motionState; }


				private:
						std::shared_ptr<btRigidBody> m_rigidBody{ nullptr }; ///< the rigid body that bullet uses
						std::shared_ptr<physics::CMotionState> m_motionState{ nullptr }; ///< motion state implementation for this engine
						float m_mass{ 0.0f }; ///< the mass of this rigid body
				};
		}
}
#endif // !RIGIDBODY_H