#ifndef COMPONENT_H
#define COMPONENT_H

#include <glm\vec3.hpp>

namespace cogs
{
		namespace ecs
		{
				/* define a typedef for the component ID type */
				using ComponentID = unsigned int;

				/* Hide implementation details */
				namespace Internal
				{
						/* Generate a unique ID for components */
						inline ComponentID getUniqueComponentID() noexcept
						{
								/* by using a static variable every time this function is called it will refer to the same "lastID" instance
								 * This way it will always return a unique ID */
								static ComponentID lastID{ 0u };
								return lastID++;
						}
				}
				
				/* forward declare the entity class */
				class Entity;

				/** The component class game components will inherit from*/
				class Component
				{
				public:
						Component() {}
						//since this is going to be used polymorphically the destructor needs to have the virtual keyword
						virtual ~Component() {}

						/* Let the component have an init function so it can be used as a constructor
						 * init is called after the parent entity has been assigned, while the constructor doesn't have it */
						virtual void init() = 0;
						/* Usually game components will have an Update and Draw function */
						virtual void update(float _deltaTime) = 0;
						virtual void render() = 0;

						virtual void onCollision(const glm::vec3& _pointA,
								const glm::vec3& _pointB,
								const glm::vec3& _normalOnB,
								Entity* _other) {}

						virtual void setEntity(Entity* _entity) { m_entity = _entity; }
				protected:
						/* Store the parent entity containing this component in a pointer due to forward declaration */
						Entity* m_entity{ nullptr };
				};

				/* Get the unique ID of every component type (same component types have same IDs) */
				template<typename T>
				inline ComponentID getComponentTypeID() noexcept
				{
						/* every time this function is called with specific type "T"
						 * it is instantiating this template with its own unique "typeID" variable */
						static_assert(std::is_base_of<Component, T>::value, "Must inherit from Component");

						static ComponentID typeID{ Internal::getUniqueComponentID() };
						return typeID;
				}
		}
}


#endif // !COMPONENT_H