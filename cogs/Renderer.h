#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

namespace cogs
{
		using uint = unsigned int;
		using VBO = uint;
		using VAO = uint;

		class Entity;
		class GLSLProgram;

		/**
		* \brief Renderer base class
		*/
		class Renderer
		{
		public:
				Renderer() {}
				Renderer(std::weak_ptr<GLSLProgram> _shader) : m_shader(_shader) {}
				virtual ~Renderer() {}

				//The first function called by the renderer
				virtual void init() = 0;
				//Called at the beggining of every frame (to clear buffers of entities)
				virtual void begin() = 0;
				//Submit an entity to the entities buffer
				virtual void submit(std::weak_ptr<Entity> _entity) = 0;
				//Called after all entities have been submitted, used for sorting/batching/culling
				virtual void end() = 0;
				//Flushes the renderer, rendering everything in the buffer
				virtual void flush() = 0;
				//disposes of buffer objects and data
				virtual void dispose() = 0;
				//Set the shader to render with
				void setShader(std::weak_ptr<GLSLProgram> _shader) { m_shader = _shader; }

		protected:
				std::weak_ptr<GLSLProgram> m_shader; ///< shader to render with
		};
}

#endif // !RENDERER_H