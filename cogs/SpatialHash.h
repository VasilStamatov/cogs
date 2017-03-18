#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include "ParticleSystem.h"
#include "BulletDebugRenderer.h"

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <glm\gtx\hash.hpp>

namespace cogs
{
		class SpatialHash
		{
		public:
				SpatialHash() {}
				SpatialHash(const glm::vec3& _cellSize) :
						m_cellSize(_cellSize)
				{
				}
				~SpatialHash()
				{

				}

				void setCellSize(const glm::vec3& _cellSize)
				{
						m_cellSize = _cellSize;
				}

				const glm::vec3& getCellSize()
				{
						return m_cellSize;
				}

				void clearBuckets()
				{
						int saveSize = m_buckets.size();
						m_buckets.clear();
						m_buckets.reserve(saveSize);
				}

				unsigned int getNumBuckets()
				{
						return m_buckets.size();
				}

				void addParticle(Particle* _particle)
				{
						std::vector<glm::ivec3> cellIDs = getResidingCellsID(_particle);
						for (auto& id : cellIDs)
						{
								//printf("cell ID %d, %d, %d \n\n", id.x, id.y, id.z);
								m_buckets[id].push_back(_particle);
						}
				}

				std::vector<Particle*> getNeighbors(Particle* _particle)
				{
						std::vector<Particle*> otherParticles;
						std::vector<glm::ivec3> bucketsParticleIsIn = getResidingCellsID(_particle);

						for (auto& id : bucketsParticleIsIn)
						{
								otherParticles.insert(otherParticles.end(), m_buckets[id].begin(), m_buckets[id].end());
						}

						return otherParticles;
				}

				void render(BulletDebugRenderer* _renderer)
				{
						for (auto& bucket : m_buckets)
						{
								glm::ivec3 index = bucket.first;

								glm::vec3 min(index.x * m_cellSize.x, index.y * m_cellSize.y, index.z * m_cellSize.z);
								glm::vec3 max(index.x * m_cellSize.x + m_cellSize.x, index.y * m_cellSize.y + m_cellSize.y, index.z * m_cellSize.z + m_cellSize.z);

								_renderer->drawBox(btVector3(min.x, min.y, min.z), btVector3(max.x, max.y, max.z), btVector3(1.0f, 1.0f, 1.0f));

								std::vector<Particle*> particlesInBucket = bucket.second;

								/*for (Particle* particle : particlesInBucket)
								{
										_renderer->drawSphere(btVector3(particle->m_position.x, particle->m_position.y, particle->m_position.z),
												particle->m_width * 0.5f, btVector3(1.0f, 1.0f, 1.0f));
								}*/
						}
				}

		private:
				std::vector<glm::ivec3> getResidingCellsID(Particle* _particle)
				{
						std::vector<glm::ivec3> bucketsParticleIsIn;

						glm::vec3 min(_particle->m_position.x - _particle->m_width * 0.5f,
								_particle->m_position.y - _particle->m_width * 0.5f,
								_particle->m_position.z);

						glm::vec3 max(_particle->m_position.x + _particle->m_width * 0.5f,
								_particle->m_position.y + _particle->m_width * 0.5f,
								_particle->m_position.z);

						addBucket(min, bucketsParticleIsIn);
						addBucket(glm::vec3(max.x, min.y, min.z), bucketsParticleIsIn);
						addBucket(max, bucketsParticleIsIn);
						addBucket(glm::vec3(min.x, max.y, max.z), bucketsParticleIsIn);

						return bucketsParticleIsIn;
				}

				void addBucket(const glm::vec3& _point, std::vector<glm::ivec3>& _bucketToAdd)
				{
						glm::ivec3 cellID = hash(_point);

						if (std::find(_bucketToAdd.begin(), _bucketToAdd.end(), cellID) == _bucketToAdd.end())
						{
								_bucketToAdd.push_back(cellID);
						}
				}

				glm::ivec3 hash(const glm::vec3& _point)
				{
						glm::ivec3 cellID;

						//i = x + width*y + width*height*z;
						cellID.x = (int)(_point.x / m_cellSize.x);
						cellID.y = (int)(_point.y / m_cellSize.y);
						cellID.z = (int)(_point.z / m_cellSize.z);

						if (_point.x < 0.0f) { cellID.x--; }
						if (_point.y < 0.0f) { cellID.y--; }
						if (_point.z < 0.0f) { cellID.z--; }

						return cellID;
				}

		private:
				glm::vec3 m_cellSize{ 0 };

				std::unordered_map<glm::ivec3, std::vector<Particle*>> m_buckets;
		};
}
#endif // !SPATIAL_HASH_H