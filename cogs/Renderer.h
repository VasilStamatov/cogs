#ifndef RENDERER_H
#define RENDERER_H

#include "Entity.h"

namespace cogs
{
		namespace graphics
		{
				class Renderer
				{
				public:
						Renderer() {}
						virtual ~Renderer() {}

						virtual void init() = 0;
						virtual void submit(ecs::Entity* _entity) = 0;
						virtual void flush(const glm::mat4& _view, const glm::mat4& _projection) = 0;
				};
		}
}

#endif // !RENDERER_H