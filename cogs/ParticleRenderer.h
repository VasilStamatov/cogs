#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include "Renderer.h"
#include "Color.h"

#include <unordered_map>
#include <vector>
#include <glm\vec4.hpp>

namespace cogs
{
		constexpr uint PARTICLE_POSITION_ATTRIBUTE	  = 0;
		constexpr uint PARTICLE_WORLDNSIZE_ATTRIBUTE	= 1;
		constexpr uint PARTICLE_COLOR_ATTRIBUTE					 = 2;
		constexpr uint PARTICLE_TEXOFFSETS_ATTRIBUTE = 3;
		constexpr uint PARTICLE_BLEND_ATTRIBUTE					 = 4;
		constexpr uint PARTICLE_MAX_INSTANCES					 		= 10000;

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
				void submit(std::weak_ptr<Entity> _entity) override;
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
						INSTANCED_ATTRIBS,

						INDEX,

						NUM_BUFFERS
				};

				VBO m_VBOs[BufferObjects::NUM_BUFFERS] = { 0 }; ///< the vbos
				VAO m_VAO{ 0 };

				struct InstanceAttributes
				{
						Color color;
						glm::vec4 worldPosAndSize;
						glm::vec4 texOffsets;
						float blendFactor;
				};

				struct InstanceData
				{
						bool isTexAdditive{ true };
						float texNumOfRows{ 0.0f };
						std::vector<InstanceAttributes> instanceAttribs;
				};
				//key = texture id (all sprites of the same texture to be instanced rendered)
				//value = instance data = per instance data
				std::unordered_map<unsigned int, InstanceData> m_particlesMap;
		};
}
#endif //!PARTICLE_RENDERER_H