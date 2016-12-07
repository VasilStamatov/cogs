#ifndef ENTITY_H
#define ENTITY_H

#include "Object.h"
#include "Transform.h"

#include <bitset>
#include <cassert>
#include <vector>
#include <memory>
#include <array>

namespace cogs
{
		namespace ecs
		{
				constexpr std::size_t MAX_COMPONENTS{ 32 };

				class Entity : public Object
				{
				public:
						/** The constructor adds a transform component, as all entities need at least a transform */
						Entity()
						{
								addComponent<Transform>();
						}

						/** The constructor adds a transform component, as all entities need at least a transform */
						Entity(const std::string& _name) : Object(_name)
						{
								addComponent<Transform>();
						}
						~Entity()
						{
						}

						/** Updates this entity and all its children */
						inline void updateAll(float _deltaTime)
						{
								update(_deltaTime);
								for (auto& child : m_children)
								{
										child->updateAll(_deltaTime);
								}
						}

						/** Renders this entity and all its children */
						inline void renderAll()
						{
								render();
								for (auto& child : m_children)
								{
										child->renderAll();
								}
						}

						/** Refreshes all the children (the root entity should always be alive and will delete all the dead children).
										Refreshing means to check if it's dead, and if it is, delete it. */
						inline void refreshAll()
						{
								m_children.erase
								(
										std::remove_if(std::begin(m_children), std::end(m_children),
												[](const std::shared_ptr<Entity>& _child)
								{
										return _child->isDestroyed();
								}),
										std::end(m_children)
										);
						}

						/** \brief Add components to this element of any type
								* \param[in] T is the component type
								* \param[in] TArgs is a parameter pack of types used to construct the component
								*/
						template<typename T, typename... TArgs>
						inline void addComponent(TArgs&&... _args)
						{
								/* check if this component is not already added */
								assert(!hasComponent<T>());

								/* begin by allocating component of type T on the heap by forwarding the passed arguments to its constructor */
								std::shared_ptr<Component> component = std::make_shared<T>(std::forward<TArgs>(_args)...);
								component->setEntity(this);
								/* Call the virtual function init of the component */
								component->init();

								/* Add the component to the vector (std::move is required as unique pointers cannot be copied) */
								m_components.emplace_back(std::move(component));

								/* when a component of type T is added, add it to the bitset and the array */
								m_componentArray[getComponentTypeID<T>()] = m_components.back();
								m_componentBitset[getComponentTypeID<T>()] = true;
						}

						/** \brief Adds a child to the vector of children
								* \param[in] _name The name of the child
								* \param[out] std::weak_ptr<Entity> return a weak pointer to the new entity
								*/
						std::weak_ptr<Entity> addChild(const std::string& _name)
						{
								/* create the new entity shared pointer */
								std::shared_ptr<Entity> newChild = std::make_shared<Entity>(_name);
								m_children.push_back(std::move(newChild));
								if (m_name != "Root")
								{
										m_children.back()->getComponent<Transform>()->setParent(getComponent<Transform>());
								}
								return m_children.back();
						}

						template<typename T>
						inline bool hasComponent() const
						{
								/* ask the bitset if the bit if the bit at the position with the type T is true or false */
								return m_componentBitset[getComponentTypeID<T>()];
						}

						/** \brief get a specific component from the m_componentArray
								* \param[out] T* return a pointer to the component requested
								*/
						template<typename T>
						inline T* getComponent() const
						{
								/* check if it has this component */
								assert(hasComponent<T>());
								//get the component pointer
								std::weak_ptr<Component> component = (m_componentArray[getComponentTypeID<T>()]);

								return reinterpret_cast<T*>(component.lock().get());
						}

						inline unsigned int numChildren() const noexcept { return m_children.size(); }

						inline std::weak_ptr<Entity> getChildAt(unsigned int _index) const { return m_children.at(_index); }

				private:
						/* Update this entity (all its components) */
						inline void update(float _deltaTime) { for (auto& component : m_components) { component->update(_deltaTime); } }
						/* Render this entity (all its components) */
						inline void render() { for (auto& component : m_components) { component->render(); } }

				private:
						/* An entity is also composed of numerous components
							* Therefore the components will be stored in an std::vector as unique pointers to allow polymorphism */
						std::vector<std::shared_ptr<Component>> m_components;

						/* The children of this entity */
						std::vector<std::shared_ptr<Entity>> m_children;

						/* A bitset to check the existance of a component with a specific ID */
						std::bitset<MAX_COMPONENTS> m_componentBitset;

						/* An array to get a component with specific ID */
						std::array<std::weak_ptr<Component>, MAX_COMPONENTS> m_componentArray;
				};
		}
}
#endif // !ENTITY_H