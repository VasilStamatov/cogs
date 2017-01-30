#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"

#include <memory>
#include "CMotionState.h"
#include "Physics.h"

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
						RigidBody(std::weak_ptr<physics::Physics> _physicsWorld ,float _mass = 1.0f, bool _isKinematic = false);
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
						* 0 = doesn't bounce at all
						* between 0 and 1 = the object bounces, but with each bounce loses part of its energy
						* over 1 = the object gains energy with each bounce
						*/
						void setRestitution(float _restitution);

						/**
						* The friction of a rigid body
						*/
						void setFriction(float _friction);

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
						* Activates the rigidbody
						*/
						void activate();

						/**
						* Sets the activation state of the rigidbody
						* \param _state - the state the rigid body can be set to:
						* http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Activation_States
						* ACTIVE_TAG										 = 1 -> better use activate();
						* ISLAND_SLEEPING					 = 2 -> you probably don't want or need to set this one manually. 
						* WANTS_DEACTIVATION		 = 3 -> you probably don't want or need to set this one manually. 
						* DISABLE_DEACTIVATION = 4 -> makes a body active forever, used for something like a player-controlled object
						* DISABLE_SIMULATION		 = 5 -> Does the opposite, making a body deactivated forever
						*/
						void setActivationState(int _state);

						/**
						* Getters
						*/
						int getActivationState()																														const { return m_rigidBody->getActivationState(); }
						std::weak_ptr<btRigidBody> getRigidBody()											  const { return m_rigidBody; }
						std::weak_ptr<physics::CMotionState> getMotionState()							  const { return m_motionState; }


				private:
						std::shared_ptr<btRigidBody> m_rigidBody{ nullptr }; ///< the rigid body that bullet uses
						std::shared_ptr<physics::CMotionState> m_motionState{ nullptr }; ///< motion state implementation for this engine
						std::weak_ptr<physics::Physics> m_physicsWorld; ///< the world this rigidbody belongs to
						float m_mass{ 0.0f }; ///< the mass of this rigid body
						bool m_isKinematic{ false }; ///< flag wether this body is kinematic or not
				};
		}
}
#endif // !RIGIDBODY_H