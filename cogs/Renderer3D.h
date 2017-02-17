#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "Renderer.h"

namespace cogs
{
		namespace graphics
		{
				class Renderer3D : public Renderer
				{
				public:
						Renderer3D(std::weak_ptr<GLSLProgram> _shader);
						Renderer3D();
						virtual ~Renderer3D();

						void init() override;
						void begin() override;
						void submit(std::weak_ptr<ecs::Entity> _entity) override;
						void end() override;
						void flush() override;
				};
		}
}

#endif