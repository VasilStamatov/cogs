#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <unordered_map>
#include <vector>
#include <glm\gtx\hash.hpp>

namespace cogs
{
		class BulletDebugRenderer;
		struct Particle;

		class SpatialHash
		{
		public:
				SpatialHash();
				SpatialHash(const glm::vec3& _cellSize);
				~SpatialHash();

				void setCellSize(const glm::vec3& _cellSize);
				const glm::vec3& getCellSize();

				void clearBuckets();

				unsigned int getNumBuckets();

				void addParticle(Particle* _particle);
				//void updateParticle(Particle* _particle);

				std::vector<Particle*> getNeighbors(Particle* _particle);

				void render(BulletDebugRenderer* _renderer);

		private:
				std::vector<glm::ivec3> getResidingCellsID(Particle* _particle);

				void addBucket(const glm::vec3& _point, std::vector<glm::ivec3>& _bucketToAdd);

				glm::ivec3 hash(const glm::vec3& _point);

		private:
				glm::vec3 m_cellSize{ 0 };

				std::unordered_map<glm::ivec3, std::vector<Particle*>> m_buckets;
		};
}
#endif // !SPATIAL_HASH_H