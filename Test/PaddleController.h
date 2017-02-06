#ifndef PADDLE_CONTROLLER_H
#define PADDLE_CONTROLLER_H

#include <cogs\Rigidbody.h>

namespace ce = cogs::ecs;

class PaddleController : public ce::Component
{
public:
		PaddleController(float _moveSpeed);
		~PaddleController();

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
		std::weak_ptr<ce::RigidBody> m_rb;
		float m_moveSpeed{ 0.0f };
};

#endif // !PADDLE_CONTROLLER_H