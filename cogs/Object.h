#ifndef OBJECT_H
#define OBJECT_H

#include <string>

namespace cogs
{
		/**
		* \brief base Object class
		*/
		class Object
		{
		public:
				Object() {}
				Object(const std::string& _name) : m_name(_name), m_destroyed(false) {}
				virtual ~Object() {}

				//name setter and getter
				void setName(const std::string& _name) { m_name = _name; }
				const std::string& getName() const noexcept { return m_name; }

				//destroy setter and getter
				void destroy() { m_destroyed = true; }
				bool isDestroyed() const noexcept { return m_destroyed; }

		protected:
				std::string m_name{ "default" }; ///< the name of the object
				bool m_destroyed{ false }; ///< the destroyed flag of the object
		};
}
#endif // !OBJECT_H