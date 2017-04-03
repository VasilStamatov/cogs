#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <glm\gtx\hash.hpp>

#include "BulletDebugRenderer.h"

namespace cogs
{
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
				SpatialHash(float _cellSize) :
						m_cellSize(_cellSize)
				{
				}
				~SpatialHash() {}

				/**
				* \brief Set the cell size
				& @param _cellSize - vec3 for the dimensions on each axis
				*/
				void setCellSize(float _cellSize)
				{
						m_cellSize = _cellSize;
				}

				/**
				* \brief get the cell size
				*/
				float getCellSize() const noexcept
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
						std::unordered_set<glm::ivec3> cellIDs = getResidingCellsID(_centerPos, _radius);
						for (auto& id : cellIDs)
						{
								//printf("cell ID %d, %d, %d \n\n", id.x, id.y, id.z);
								m_buckets[id].push_back(_item);
						}
				}

				/**
				* \brief get a vector of neighboring items to a specific item
				* @param _centerPos - center position of the item in world space
				* @param _radius - radius of the item
				*/
				std::vector<Item*> getNeighbors(const glm::vec3& _centerPos, float _radius)
				{
						std::vector<Item*> otherItems;
						std::unordered_set<glm::ivec3> bucketsItemIsIn = getResidingCellsID(_centerPos, _radius);

						for (auto& id : bucketsItemIsIn)
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

								glm::vec3 min(index.x * m_cellSize, index.y * m_cellSize, index.z * m_cellSize);
								glm::vec3 max(index.x * m_cellSize + m_cellSize, index.y * m_cellSize + m_cellSize, index.z * m_cellSize + m_cellSize);

								_renderer->drawBox(btVector3(min.x, min.y, min.z), btVector3(max.x, max.y, max.z), btVector3(1.0f, 1.0f, 1.0f));
						}
				}

		private:
				/**
				* \brief get the hashed ids of the buckets that this position resides in
				* @param _centerPos - center position of the item in world space
				* @param _radius - radius of the item
				*/
				std::unordered_set<glm::ivec3> getResidingCellsID(const glm::vec3& _centerPos, float _radius)
				{
						//create the set of bucket IDs that this item resides in
						//sets have a faster find function than vectors, so it's appropriate here
						std::unordered_set<glm::ivec3> bucketsItemIsIn;

						// get the min coordinate of the box
						glm::vec3 min(_centerPos.x - _radius,
								_centerPos.y - _radius,
								_centerPos.z - _radius);

						//get the max coordinate of the box
						glm::vec3 max(_centerPos.x + _radius,
								_centerPos.y + _radius,
								_centerPos.z + _radius);

						//add all vertices to their respective bucket

						//bottom left back
						addBucket(min, bucketsItemIsIn);
						//bottom right back
						addBucket(glm::vec3(max.x, min.y, min.z), bucketsItemIsIn);
						//top right back
						addBucket(glm::vec3(max.x, max.y, min.z), bucketsItemIsIn);
						//top left back
						addBucket(glm::vec3(min.x, max.y, min.z), bucketsItemIsIn);
						//top right front
						addBucket(max, bucketsItemIsIn);
						//top left front
						addBucket(glm::vec3(min.x, max.y, max.z), bucketsItemIsIn);
						//bottom left front
						addBucket(glm::vec3(min.x, min.y, max.z), bucketsItemIsIn);
						//bottom right front
						addBucket(glm::vec3(max.x, min.y, max.z), bucketsItemIsIn);

						return bucketsItemIsIn;
				}

				/**
				* \brief Adds a bucket ID to the vector of bucket IDs based on the world point
				* @param _point - world space point
				* @param _bucketToAdd - vector of bucket IDs the new bucket will be added
				*/
				void addBucket(const glm::vec3& _point, std::unordered_set<glm::ivec3>& _bucketToAdd)
				{
						//hash the world coordinate point
						glm::ivec3 cellID = hash(_point);

						//check if the bucket with this ID already exists in the set of bucket IDs
						if (_bucketToAdd.find(cellID) == _bucketToAdd.end())
						{
								//Add it if it doesn't exist already
								_bucketToAdd.insert(cellID);
						}
				}

				/**
				* \brief Hashes a world point to the ID of the bucket that the point resides in
				* @param _point - world space point
				*/
				glm::ivec3 hash(const glm::vec3& _point)
				{
						//the bucked ID
						glm::ivec3 cellID(0);

						//calculate the cell ID by division with cell size
						cellID.x = (int)(_point.x / m_cellSize);
						cellID.y = (int)(_point.y / m_cellSize);
						cellID.z = (int)(_point.z / m_cellSize);

						if (_point.x < 0.0f) { cellID.x--; }
						if (_point.y < 0.0f) { cellID.y--; }
						if (_point.z < 0.0f) { cellID.z--; }

						//return the bucket ID
						return cellID;
				}

		private:
				float m_cellSize{ 0.0f }; ///< size of the cells in every axis

				/** the actual hashtable that uses ivec3 as hash key and vector of Item pointers as value */
				std::unordered_map<glm::ivec3, std::vector<Item*>> m_buckets;
		};
}
#endif // !SPATIAL_HASH_H