#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Renderer.h"

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
						SpriteBatch(uint _offset, uint _numIndices, uint _texture) :
								m_firstIndex(_offset), m_numIndices(_numIndices), m_texture(_texture) {
						}
						uint m_firstIndex{ 0 };
						uint m_numIndices{ 0 };
						uint m_texture{ 0 };
				};

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
						virtual ~Renderer2D();

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
						void submit(std::weak_ptr<ecs::Entity> _entity) override;

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
						void dispose();

						/**
						* \brief sets the sorting type
						*/
						void setSortType(const SpriteSortType& _sortType = SpriteSortType::TEXTURE) { m_sortType = _sortType; }

				private:
						void sortSprites(); ///< sorts the sprites
						void createSpriteBatches(); ///< creates the sprite batches

						/** Enum for the buffer objects */
						enum BufferObjects : unsigned int
						{
								POSITION = 0,
								TEXCOORD = 1,
								COLOR    = 2,

								INDEX    = 3,

								NUM_BUFFERS = 4
						};

				private:
						std::vector<SpriteBatch> m_spriteBatches; ///< set of batched sprites

						SpriteSortType m_sortType{ SpriteSortType::TEXTURE }; ///< the current sort type

						VAO m_VAO{ 0 }; ///< the vao to be used
						VBO m_VBOs[BufferObjects::NUM_BUFFERS] = { 0 }; ///< the vbos
				};
		}
}

#endif // !RENDERER_H