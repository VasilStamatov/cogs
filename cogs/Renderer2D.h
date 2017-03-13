#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Renderer.h"

#include "Color.h"
#include <glm\vec2.hpp>
#include <glm\mat4x4.hpp>
#include <unordered_map>
#include <vector>

namespace cogs
{
		/**
		* \brief derived class from Base Renderer to handle rendering sprites
		*/
		class Renderer2D : public Renderer
		{
		public:
				/**
				* \brief Construct the renderer and set the shader to render with
				*/
				Renderer2D(std::weak_ptr<GLSLProgram> _shader);
				Renderer2D();
				~Renderer2D();

				/**
				* \brief init the renderer
				*/
				void init() override;

				/**
				* Begin submission
				*/
				void begin() override;

				/**
				* \brief submit an entity to the renderer
				*/
				void submit(std::weak_ptr<Entity> _entity) override;

				/**
				* End submission and sort the sprites and put them in batches
				*/
				void end() override;

				/**
				* \brief flush the renderer
				*/
				void flush() override;

				/**
				* \brief Disposes of the buffer objects
				*/
				void dispose() override;

		private:
				void sortSprites(); ///< sorts the sprites

				/** Enum for the buffer objects */
				enum BufferObjects : unsigned int
				{
						POSITION,
						COLOR,
						SIZE,
						WORLDMAT,

						INDEX,

						NUM_BUFFERS
				};

				VAO m_VAO{ 0 }; ///< the vao to be used
				VBO m_VBOs[BufferObjects::NUM_BUFFERS] = { 0 }; ///< the vbos

				struct InstanceData
				{
						std::vector<Color> colors;
						std::vector<glm::vec2> sizes;
						std::vector<glm::mat4> worldmats;
				};
				//key = texture id (all sprites of the same texture to be instanced rendered)
				//value = instance data = per instance data
				std::unordered_map<unsigned int, InstanceData> m_entitiesMap;
		};
}

#endif // !RENDERER_H