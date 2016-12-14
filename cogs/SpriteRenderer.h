#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Renderer.h"

#include <GL\glew.h>

namespace cogs
{
		namespace graphics
		{
				/**
						* Determines how we should sort the sprites
						*/
				enum class SpriteSortType
				{
						NONE,
						FRONT_TO_BACK,
						BACK_TO_FRONT,
						TEXTURE
				};

				// Each sprite batch is used for a single draw call
				struct SpriteBatch
				{
						SpriteBatch(GLuint _offset, GLuint _numIndices, GLuint _texture) :
								m_firstIndex(_offset), m_numIndices(_numIndices), m_texture(_texture) {
						}
						GLuint m_firstIndex{ 0 };
						GLuint m_numIndices{ 0 };
						GLuint m_texture{ 0 };
				};

				/**
						* The per-vertex data for a sprite (4 total in a sprite)
						*/
				struct SpriteVertex
				{
						glm::vec3 position;
						glm::vec2 uv;
						glm::vec4 color;
				};

				constexpr GLuint POSITION_ATTRIBUTE_INDEX = 0;
				constexpr GLuint UV_ATTRIBUTE_INDEX						 = 1;
				constexpr GLuint COLOR_ATTRIBUTE_INDEX		  = 2;


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
						void begin(const SpriteSortType& _sortType = SpriteSortType::TEXTURE);

						/**
						  * End submission
						  */
						void end();

						/**
								* Disposes of the buffer objects
								*/
						void dispose();

				private:
						void SortSprites();
						void CreateSpriteBatches();

				private:
						VBO m_vbo{ 0 };
						IBO m_ibo{ 0 };
						VAO m_vao{ 0 };

						SpriteSortType m_sortType;

						std::vector<ecs::Entity*> m_entities;
						std::vector<SpriteBatch> m_spriteBatches;
				};
		}
}
#endif // !SPRITE_RENDERER_H