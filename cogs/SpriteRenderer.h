#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Renderer.h"

#include <GL\glew.h>

namespace cogs
{
		namespace graphics
		{
				/**
						* The per-vertex data for a sprite (4 total in a sprite)
						*/
				struct SpriteVertex
				{
						glm::vec3 position;
						glm::vec4 color;
				};

				constexpr GLuint MAX_SPRITES = 5000;
				constexpr GLuint VERTEX_SIZE = sizeof(SpriteVertex);
				constexpr GLuint SPRITE_SIZE = VERTEX_SIZE * 4;
				constexpr GLuint BUFFER_SIZE = SPRITE_SIZE * MAX_SPRITES;
				constexpr GLuint INDICES_SIZE = MAX_SPRITES * 6;

				constexpr GLuint POSITION_ATTRIBUTE_INDEX = 0;
				constexpr GLuint COLOR_ATTRIBUTE_INDEX			 = 1;

				using VBO = GLuint;
				using VAO = GLuint;
				using IBO = GLuint;

				class SpriteRenderer : public Renderer
				{
				public:
						SpriteRenderer();
					 ~SpriteRenderer();
						
						/**
								* Initializes the renderer
								*/
						void init() override;

						/**
								* Submit an entity to be rendered
								*/
						void submit(ecs::Entity* _entity) override;

						/**
								* Render all the submitted entities
								*/
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
								* Disposes of the buffer objects
								*/
						void dispose();

				private:
						VBO m_vbo{ 0 };
						IBO m_ibo{ 0 };
						VAO m_vao{ 0 };
						SpriteVertex* m_buffer{ nullptr };
						GLuint m_indicesCount{ 0 };
				};
		}
}
#endif // !SPRITE_RENDERER_H