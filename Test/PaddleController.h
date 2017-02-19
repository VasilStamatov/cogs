#ifndef PADDLE_CONTROLLER_H
#define PADDLE_CONTROLLER_H

#include <cogs\Rigidbody.h>

namespace ce = cogs::ecs;

/**
* \brief Component for the paddle to handle its controls
*/
class PaddleController : public ce::Component
{
public:
		PaddleController(float _moveSpeed);
		~PaddleController();

		/**
		* \brief The initialize component function
		*/
		void init() override;

		/**
		* \brief The update component function
		*/
		void update(float _deltaTime) override;

		/**
		* \brief handle the collisions of the paddle
		*/
		void onCollision(const glm::vec3& _pointA,
				const glm::vec3& _pointB,
				const glm::vec3& _normalOnB,
				ce::Entity* _other) override;

private:
		std::weak_ptr<ce::RigidBody> m_rb; ///< the paddle's rigidbody
		float m_moveSpeed{ 0.0f }; ///< the movespeed of the paddle
};

#endif // !PADDLE_CONTROLLER_H