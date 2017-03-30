#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <unordered_map>
#include <vector>
#include <glm\gtx\hash.hpp>

namespace cogs
{
		//forward declarations
		class BulletDebugRenderer;

		template<typename T>
		class SpatialHash
		{
				//modern typedef of the typename
				using Item = T;

		public:
				SpatialHash() { }
				/**
				* \brief Constructor that sets the cell size
				& @param _cellSize - vec3 for the dimensions on each axis
				*/
				SpatialHash(const glm::vec3& _cellSize) :
						m_cellSize(_cellSize)
				{
				}
				~SpatialHash() {}

				/**
				* \brief Set the cell size
				& @param _cellSize - vec3 for the dimensions on each axis
				*/
				void setCellSize(const glm::vec3& _cellSize)
				{
						m_cellSize = _cellSize;
				}

				/**
				* \brief get the cell size
				*/
				const glm::vec3& getCellSize() const noexcept
				{
						return m_cellSize;
				}

				/**
				* \brief clear the buckets
				*/
				void clearBuckets()
				{
						//store the current buckets size
						int saveSize = m_buckets.size();
						//clear the buckets maps
						m_buckets.clear();
						//reserve the amount of positions that the map had before clearing, for optimization
						m_buckets.reserve(saveSize);
				}

				/**
				* \brief get the number of buckets in the map
				*/
				unsigned int getNumBuckets()
				{
						return m_buckets.size();
				}

				/**
				* \brief add an item to the hash table
				* @param _item - pointer to the item to be added
				* @param _centerPos - center position of the item in world space
				* @param _radius - radius of the item
				*/
				void addItem(Item* _item, const glm::vec3& _centerPos, float _radius)
				{
						std::vector<glm::ivec3> cellIDs = getResidingCellsID(_centerPos, _radius);
						for (auto& id : cellIDs)
						{
								//printf("cell ID %d, %d, %d \n\n", id.x, id.y, id.z);
								m_buckets[id].push_back(_item);
						}
				}

				//void updateParticle(Particle* _particle);

				/**
				* \brief get a vector of neighboring items to a specific item
				* @param _centerPos - center position of the item in world space
				* @param _radius - radius of the item
				*/
				std::vector<Item*> getNeighbors(const glm::vec3& _centerPos, float _radius)
				{
						std::vector<Item*> otherItems;
						std::vector<glm::ivec3> bucketsParticleIsIn = getResidingCellsID(_centerPos, _radius);

						for (auto& id : bucketsParticleIsIn)
						{
								otherItems.insert(otherItems.end(), m_buckets[id].begin(), m_buckets[id].end());
						}

						return otherItems;
				}

				/**
				* \brief Render all the buckets boxes for debugging
				* @param _renderer - reference to the debug renderer that will render the boxes
				*/
				void render(BulletDebugRenderer* _renderer)
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

		private:
				/**
				* \brief get the hashed ids of the buckets that this position resides in
				* @param _centerPos - center position of the item in world space
				* @param _radius - radius of the item
				*/
				std::vector<glm::ivec3> getResidingCellsID(const glm::vec3& _centerPos, float _radius)
				{
						//create the vector of bucket IDs that this item resides in
						std::vector<glm::ivec3> bucketsParticleIsIn;

						// get the min coordinate of the box
						glm::vec3 min(_centerPos.x - _radius,
								_centerPos.y - _radius,
								_centerPos.z);

						//get the max coordinate of the box
						glm::vec3 max(_centerPos.x + _radius,
								_centerPos.y + _radius,
								_centerPos.z);

						//add all vertices to their respective bucket
						addBucket(min, bucketsParticleIsIn);
						addBucket(glm::vec3(max.x, min.y, min.z), bucketsParticleIsIn);
						addBucket(max, bucketsParticleIsIn);
						addBucket(glm::vec3(min.x, max.y, max.z), bucketsParticleIsIn);

						return bucketsParticleIsIn;
				}

				/**
				* \brief Adds a bucket ID to the vector of bucket IDs based on the world point
				* @param _point - world space point
				* @param _bucketToAdd - vector of bucket IDs the new bucket will be added
				*/
				void addBucket(const glm::vec3& _point, std::vector<glm::ivec3>& _bucketToAdd)
				{
						//hash the world coordinate point
						glm::ivec3 cellID = hash(_point);

						//check if the bucket with this ID already exists in the vector of bucket IDs
						if (std::find(_bucketToAdd.begin(), _bucketToAdd.end(), cellID) == _bucketToAdd.end())
						{
								//Add it if it doesn't exist already
								_bucketToAdd.push_back(cellID);
						}
				}

				/**
				* \brief Hashes a world point to the ID of the bucket that the point resides in
				* @param _point - world space point
				*/
				glm::ivec3 hash(const glm::vec3& _point)
				{
						//the bucked ID
						glm::ivec3 cellID;

						//calculate the cell ID by division with cell size
						cellID.x = (int)(_point.x / m_cellSize.x);
						cellID.y = (int)(_point.y / m_cellSize.y);
						cellID.z = (int)(_point.z / m_cellSize.z);

						if (_point.x < 0.0f) { cellID.x--; }
						if (_point.y < 0.0f) { cellID.y--; }
						if (_point.z < 0.0f) { cellID.z--; }

						//return the bucket ID
						return cellID;
				}

		private:
				glm::vec3 m_cellSize{ 0 }; ///< size of the cells in every axis

				/** the actual hashtable that uses ivec3 as hash key and vector of Item pointers as value */
				std::unordered_map<glm::ivec3, std::vector<Item*>> m_buckets;
		};
}
#endif // !SPATIAL_HASH_H