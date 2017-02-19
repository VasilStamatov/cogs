#ifndef BALL_BEHAVIOR_H
#define BALL_BEHAVIOR_H

#include <cogs\RigidBody.h>
#include <cogs\Transform.h>

namespace ce = cogs::ecs;

/**
* Component for the ball to handle its behavior
*/
class BallBehavior : public ce::Component
{
public:
		BallBehavior();
		~BallBehavior();

		/**
		* \brief The initialize component function
		*/
		void init() override;

		/**
		* \brief The update component function
		*/
		void update(float _deltaTime) override;

		/**
		* \brief handle collisions of the ball
		*/
		void onCollision(const glm::vec3& _pointA,
				const glm::vec3& _pointB,
				const glm::vec3& _normalOnB,
				ce::Entity* _other) override;

private:
		std::weak_ptr<ce::RigidBody> m_rb; ///< the rigidbody of the ball
		float m_desiredVelocity{ 0.0f }; ///< the desired velocity of the ball
};


#endif // !BALL_BEHAVIOR_H
