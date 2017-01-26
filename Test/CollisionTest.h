#ifndef COLLISION_TEST_H
#define COLLISION_TEST_H

#include <cogs\Entity.h>
#include <cogs\Input.h>
#include <cogs\Random.h>
#include <iostream>

namespace ce = cogs::ecs;
namespace cu = cogs::utils;

class CollisionTest : public ce::Component
{
public:
		CollisionTest() {} 
		~CollisionTest() {}

		/**
		* The initialize component function
		*/
		void init() override {}

		/**
		* The update component function
		*/
		void update(float _deltaTime) override {}

		/**
		* The render component function
		*/
		void render() override {}

		void onCollision(const glm::vec3& _pointA,
				const glm::vec3& _pointB,
				const glm::vec3& _normalOnB,
				ce::Entity* _other) override 
		{
				//if (cu::Input::isKeyPressed(cu::KeyCode::SPACE))
				{
					/*	std::cout << "\nPoint A: x:" << _pointA.x << " y:" << _pointA.y << " z:" << _pointA.z <<
								"\nPoint B: x:" << _pointB.x << " y:" << _pointB.y << " z:" << _pointB.z <<
								"\nNormal on B: x:" << _normalOnB.x << " y:" << _normalOnB.y << " z:" << _normalOnB.z <<
								"\nOther Entity name: " << _other->getName() << std::endl;*/
						std::cout << cu::Random::getRandInt(0, 1000) << std::endl;
						std::cout << m_entity->getName() << " is colliding with " << _other->getName() << std::endl;
				}
		}

private:

};

#endif // !COLLISION_TEST_H