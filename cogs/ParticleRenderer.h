#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include "Renderer.h"
#include "Color.h"

#include <unordered_map>

namespace cogs
{
		namespace graphics
		{
				class ParticleRenderer : public Renderer
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

				private:
						void sortParticles();

						/** Enum for the buffer objects */
						enum BufferObjects : unsigned int
						{
								POSITION,
								WORLDPOS_AND_SIZE,
								COLOR,
								TEXOFFSETS,
								BLENDFACTOR,

								INDEX,

								NUM_BUFFERS
						};

						VBO m_VBOs[BufferObjects::NUM_BUFFERS] = { 0 }; ///< the vbos
						VAO m_VAO{ 0 };

						struct InstanceData
						{
								bool isTexAdditive{ true };
								float texNumOfRows{ 0.0f };
								std::vector<Color> colors;
								std::vector<glm::vec4> worldPosAndSize;
								std::vector<glm::vec4> texOffsets;
								std::vector<float> blendFactors;
						};
						//key = texture id (all sprites of the same texture to be instanced rendered)
						//value = instance data = per instance data
						std::unordered_map<unsigned int, InstanceData> m_particlesMap;
				};
		}
}
#endif //!PARTICLE_RENDERER_H