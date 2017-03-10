#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include "Renderer2D.h"

namespace cogs
{
		namespace graphics
		{
				class ParticleRenderer : public Renderer2D
				{
				public:
						ParticleRenderer(std::weak_ptr<GLSLProgram> _shader);
						~ParticleRenderer();

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
						//disposes of buffer objects and data
						void dispose() override;

				protected:
						void createSpriteBatches() override; ///< fill the spritebatches vector with the batches
					 void sortSprites() override; ///< sorts the sprites

				private:
						/** Enum for the buffer objects */
						enum BufferObjects : unsigned int
						{
								POSITION,
								COLOR,

								NUM_BUFFERS
						};

						VBO m_VBOs[BufferObjects::NUM_BUFFERS] = { 0 }; ///< the vbos
				};
		}
}
#endif //!PARTICLE_RENDERER_H