#ifndef RENDERER_H
#define RENDERER_H

#include "Entity.h"

namespace cogs
{
		namespace graphics
		{
				using uint = unsigned int;
				using VBO = uint;
				using VAO = uint;

				class Renderer
				{
				public:
						Renderer() {}
						virtual ~Renderer() {}

						virtual void init() = 0;
						virtual void submit(std::weak_ptr<ecs::Entity> _entity) = 0;
						virtual void flush() = 0;
				};
		}
}

#endif // !RENDERER_H