#ifndef BALL_BEHAVIOR_H
#define BALL_BEHAVIOR_H

#include <cogs\RigidBody.h>
#include <cogs\Transform.h>

namespace ce = cogs::ecs;

class BallBehavior : public ce::Component
{
public:
		BallBehavior();
		~BallBehavior();

		/**
		* The initialize component function
		*/
		void init() override;

		/**
		* The update component function
		*/
		void update(float _deltaTime) override;

		void onCollision(const glm::vec3& _pointA,
				const glm::vec3& _pointB,
				const glm::vec3& _normalOnB,
				ce::Entity* _other) override;

private:
		ce::RigidBody* m_rb							{ nullptr };
		ce::Transform* m_transform{ nullptr };
		float m_desiredVelocity{ 0.0f };
};


#endif // !BALL_BEHAVIOR_H
