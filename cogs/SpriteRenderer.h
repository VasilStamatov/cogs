#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Renderer.h"
#include "GLSLProgram.h"
#include "Color.h"

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
						Color color;
				};

				constexpr GLuint SPITE_POSITION_ATTRIBUTE_INDEX = 0;
				constexpr GLuint SPITE_UV_ATTRIBUTE_INDEX						 = 1;
				constexpr GLuint SPITE_COLOR_ATTRIBUTE_INDEX		  = 2;

				using VBO = GLuint;
				using VAO = GLuint;
				using IBO = GLuint;

				class SpriteRenderer : public Renderer
				{
				public:
						SpriteRenderer();
						SpriteRenderer(const std::string& _name, const std::string& _vsFilePath, const std::string& _fsFilePath, const std::string& _gsFilePath = "");
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
						void flush(const glm::mat4& _view, const glm::mat4& _projection) override;

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

						/**
								* sets the shader this renderer to use
								*/
						void setShader(const std::string& _name, const std::string& _vsFilePath, const std::string& _fsFilePath, const std::string& _gsFilePath = "");

				private:
						void SortSprites();
						void CreateSpriteBatches();

				private:
						VBO m_vbo{ 0 };
						IBO m_ibo{ 0 };
						VAO m_vao{ 0 };

						SpriteSortType m_sortType;

						GLSLProgram m_shader;

						std::vector<ecs::Entity*> m_entities;
						std::vector<SpriteBatch> m_spriteBatches;
				};
		}
}
#endif // !SPRITE_RENDERER_H