#ifndef COMPONENT_H
#define COMPONENT_H

#include <glm\vec3.hpp>
#include <memory>

namespace cogs
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
				virtual ~Component() {  }

				/**
				* \brief init is called after the parent entity has been assigned, while the constructor doesn't have it
				* it's the first function called after the constructor
				*/
				virtual void init() = 0;

				/**
				* \brief the update function of each component, called once per frame
				*/
				virtual void update(float _deltaTime) = 0;

				/**
				* \brief the render function of a component, should be implemented if a component is expected to render stuff
				*/
				virtual void render() {};

				/**
				* \brief the collision handling function, should be implemented on components which handle collisions of entities
				*/
				virtual void onCollision(const glm::vec3& _pointA,
						const glm::vec3& _pointB,
						const glm::vec3& _normalOnB,
						Entity* _other) {}

				/**
				* \brief the postProcess function of components, called after the engine has finished rendering
				* should be used to handle postprocessing
				*/
				virtual void postProcess() {}

				/**
				* \brief sets the reference to the entity which hold this component
				*/
				void setEntity(std::weak_ptr<Entity> _entity) { m_entity = _entity; }

				/**
				* \brief gets the reference to the entity which hold this component
				*/
				std::weak_ptr<Entity> getEntity() { return m_entity; }

		protected:
				std::weak_ptr<Entity> m_entity; ///< reference to the entity which holds this component
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


#endif // !COMPONENT_H