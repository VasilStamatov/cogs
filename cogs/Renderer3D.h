#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "Renderer.h"

namespace cogs
{
		namespace graphics
		{
				class GLSLProgram;

				class Renderer3D : public Renderer
				{
				public:
						Renderer3D(std::weak_ptr<GLSLProgram> _shader);
						Renderer3D();
						virtual ~Renderer3D();

						void init() override;
						void submit(std::weak_ptr<ecs::Entity> _entity) override;
						void flush() override;
						/**
						* Begin submission
						*/
						void begin();

						/**
						* End submission
						*/
						void end();

						/**
						* sets the shader this renderer to use
						*/
						void setShader(std::weak_ptr<GLSLProgram> _shader);

				private:

				private:
						std::weak_ptr<GLSLProgram> m_shader; // shader to render with
						std::vector<std::weak_ptr<ecs::Entity>> m_entities; // set ot submitted entities to render

				};
		}
}

#endif