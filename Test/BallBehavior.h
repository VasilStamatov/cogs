#ifndef BALL_BEHAVIOR_H
#define BALL_BEHAVIOR_H

#include <cogs\RigidBody.h>
#include <cogs\Transform.h>

/**
* Component for the ball to handle its behavior
*/
class BallBehavior : public cogs::Component
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
				cogs::Entity* _other) override;

private:
		std::weak_ptr<cogs::RigidBody> m_rb; ///< the rigidbody of the ball
		float m_desiredVelocity{ 0.0f }; ///< the desired velocity of the ball
};


#endif // !BALL_BEHAVIOR_H
