#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include "Utils.h"

namespace cogs
{
		namespace graphics
		{
				class ModelData
				{
				public:
						ModelData()	{}

						ModelData(const std::vector<Mesh>& _meshes,
								const std::string& _filePath = "") : m_filePath(_filePath)
						{
								m_meshes = _meshes;
						}
						ModelData(const std::string& _filePath) : m_filePath(_filePath)
						{
								m_meshes = utils::loadModel(m_filePath);
						}
						~ModelData()
						{
								dispose(); 
						}

						void addMesh(const Mesh& _mesh) { m_meshes.push_back(_mesh); }

						void render()			{ for (auto& mesh : m_meshes) mesh.render(); }
						void dispose()		{ for (auto& mesh : m_meshes) mesh.dispose(); }
						void reupload() { for (auto& mesh : m_meshes) mesh.reupload(); }

						bool isValid()
						{
								for (auto& mesh : m_meshes)
								{
										if (!mesh.isValid()) return false;
								}
								return true;
						}

						inline std::vector<Mesh>& getMeshes() { return m_meshes; }
						inline const std::string& getPath()			{ return m_filePath; }

				private:
						std::vector<Mesh> m_meshes;
						std::string m_filePath{ "" };
				};
		}
}
#endif // !MODEL_DATA_H