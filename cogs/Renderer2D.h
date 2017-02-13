#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Renderer.h"

namespace cogs
{
		namespace graphics
		{
				class GLSLProgram;

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
						SpriteBatch(uint _offset, uint _numIndices, uint _texture) :
								m_firstIndex(_offset), m_numIndices(_numIndices), m_texture(_texture) {
						}
						uint m_firstIndex{ 0 };
						uint m_numIndices{ 0 };
						uint m_texture{ 0 };
				};

				class Renderer2D : public Renderer
				{
				public:
						Renderer2D(std::weak_ptr<GLSLProgram> _shader);
						Renderer2D();
						virtual ~Renderer2D();

						void init() override;
						void submit(std::weak_ptr<ecs::Entity> _entity) override;
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

						/**
						* sets the shader this renderer to use
						*/
						void setShader(std::weak_ptr<GLSLProgram> _shader);

				private:
						void sortSprites();
						void createSpriteBatches();

						enum BufferObjects : unsigned int
						{
								POSITION = 0,
								TEXCOORD = 1,
								COLOR    = 2,

								INDEX    = 3,

								NUM_BUFFERS = 4
						};

				private:
						std::weak_ptr<GLSLProgram> m_shader; // shader to render with
						std::vector<std::weak_ptr<ecs::Entity>> m_entities; // set ot submitted entities to render
						std::vector<SpriteBatch> m_spriteBatches; //set of batched sprites

						SpriteSortType m_sortType;

						VAO m_VAO{ 0 };
						VBO m_VBOs[BufferObjects::NUM_BUFFERS] = { 0 };
				};
		}
}

#endif // !RENDERER_H