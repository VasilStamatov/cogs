#ifndef BILLBOARD_RENDERER_H
#define BILLBOARD_RENDERER_H

#include "Renderer2D.h"

namespace cogs
{
		namespace graphics
		{
				class BillboardRenderer : public Renderer
				{
				public:
						BillboardRenderer(std::weak_ptr<GLSLProgram> _shader);
						~BillboardRenderer();

						//The first function called by the renderer
						void init() override;
						//Called at the beggining of every frame (to clear buffers of entities)
						void begin() override;
						//Submit an entity to the entities buffer
						void submit(std::weak_ptr<ecs::Entity> _entity) override;
						//Called after all entities have been submitted, used for sorting/batching/culling
						void end() override;
						//Flushes the renderer, rendering everything in the buffer
						void flush() override;

						/**
						* Disposes of the buffer objects
						*/
						void dispose();

						void setSortType(const SpriteSortType& _sortType = SpriteSortType::TEXTURE) { m_sortType = _sortType; }

				private:
						void sortSprites();
						void createSpriteBatches();

				private:
						std::vector<SpriteBatch> m_spriteBatches; //set of batched sprites

						SpriteSortType m_sortType{ SpriteSortType::TEXTURE };

						VAO m_VAO{ 0 };

						//only 1 vbo for positions
						VBO m_VBO{ 0 };
				};
		}
}


#endif // !BILLBOARD_RENDERER_H