#include "SpatialHash.h"

#include "ParticleSystem.h"
#include "BulletDebugRenderer.h"

#include <algorithm>

namespace cogs
{
		SpatialHash::SpatialHash()
		{
		}
		SpatialHash::SpatialHash(const glm::vec3 & _cellSize) :
				m_cellSize(_cellSize)
		{
		}
		SpatialHash::~SpatialHash()
		{
		}
		void SpatialHash::setCellSize(const glm::vec3 & _cellSize)
		{
				m_cellSize = _cellSize;
		}
		const glm::vec3 & SpatialHash::getCellSize()
		{
				return m_cellSize;
		}
		void SpatialHash::clearBuckets()
		{
				int saveSize = m_buckets.size();
				m_buckets.clear();
				m_buckets.reserve(saveSize);
		}
		unsigned int SpatialHash::getNumBuckets()
		{
				return m_buckets.size();
		}
		void SpatialHash::addParticle(Particle * _particle)
		{
				std::vector<glm::ivec3> cellIDs = getResidingCellsID(_particle);
				for (auto& id : cellIDs)
				{
						//printf("cell ID %d, %d, %d \n\n", id.x, id.y, id.z);
						m_buckets[id].push_back(_particle);
				}
				//_particle->m_cellIndex = cellIDs;
		}

		//void SpatialHash::updateParticle(Particle * _particle)
		//{
		//		std::vector<glm::ivec3> cellIDs = getResidingCellsID(_particle);

		//		for (auto& oldID : _particle->m_cellIndex)
		//		{
		//				auto it = std::find(cellIDs.begin(), cellIDs.end(), oldID);

		//				if (it == cellIDs.end())
		//				{
		//						//not found
		//						m_buckets[oldID].remove(_particle);
		//				}
		//		}

		//		_particle->m_cellIndex = cellIDs;

		//		for (auto& newID : cellIDs)
		//		{
		//				auto it = std::find(m_buckets[newID].begin(), m_buckets[newID].end(), _particle);

		//				if (it == m_buckets[newID].end())
		//				{
		//						//not found
		//						m_buckets[newID].push_back(_particle);
		//				}
		//		}
		//}

		std::vector<Particle*> SpatialHash::getNeighbors(Particle * _particle)
		{
				std::vector<Particle*> otherParticles;
				std::vector<glm::ivec3> bucketsParticleIsIn = getResidingCellsID(_particle);

				for (auto& id : bucketsParticleIsIn)
				{
						otherParticles.insert(otherParticles.end(), m_buckets[id].begin(), m_buckets[id].end());
				}

				return otherParticles;
		}
		void SpatialHash::render(BulletDebugRenderer * _renderer)
		{
				for (auto& bucket : m_buckets)
				{
						glm::ivec3 index = bucket.first;

						glm::vec3 min(index.x * m_cellSize.x, index.y * m_cellSize.y, index.z * m_cellSize.z);
						glm::vec3 max(index.x * m_cellSize.x + m_cellSize.x, index.y * m_cellSize.y + m_cellSize.y, index.z * m_cellSize.z + m_cellSize.z);

						_renderer->drawBox(btVector3(min.x, min.y, min.z), btVector3(max.x, max.y, max.z), btVector3(1.0f, 1.0f, 1.0f));

						//std::vector<Particle*> particlesInBucket = bucket.second;

						/*for (Particle* particle : particlesInBucket)
						{
						_renderer->drawSphere(btVector3(particle->m_position.x, particle->m_position.y, particle->m_position.z),
						particle->m_width * 0.5f, btVector3(1.0f, 1.0f, 1.0f));
						}*/
				}
		}
		std::vector<glm::ivec3> SpatialHash::getResidingCellsID(Particle * _particle)
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
		void SpatialHash::addBucket(const glm::vec3 & _point, std::vector<glm::ivec3>& _bucketToAdd)
		{
				glm::ivec3 cellID = hash(_point);

				if (std::find(_bucketToAdd.begin(), _bucketToAdd.end(), cellID) == _bucketToAdd.end())
				{
						_bucketToAdd.push_back(cellID);
				}
		}
		glm::ivec3 SpatialHash::hash(const glm::vec3 & _point)
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
}