#ifndef MODEL_H
#define MODEL_H

#include "Utils.h"

namespace cogs
{
		namespace graphics
		{
				class Model
				{
				public:
						Model(const std::string& _name, const std::string& _filePath)
								: m_name(_name), m_filePath(_filePath)
						{
								m_meshes = utils::loadModel(m_filePath);
						}

						Model(const std::string& _name, const std::vector<graphics::Mesh>& _meshes)
								: m_name(_name), m_meshes(_meshes), m_filePath(_name)
						{}

						~Model() { for (auto& mesh : m_meshes) mesh.dispose(); }

						void render() { for (auto& mesh : m_meshes) mesh.render(); }
						void reupload() { for (auto& mesh : m_meshes) mesh.reupload(); }
						bool isValid() 
						{
								for (auto& mesh : m_meshes)
								{
										if (!mesh.isValid()) return false;
								}
								return true;
						}
						void addMesh(const Mesh& _mesh) { m_meshes.push_back(_mesh); }

						inline const std::string& getName()									const { return m_name; }
						inline const std::string& getPath()									const { return m_filePath; }
						inline const std::vector<Mesh>& getMeshes() const { return m_meshes; }

				private:
						std::string m_name{ "" };
						std::string m_filePath{ "" };
						std::vector<Mesh> m_meshes;
				};
		}
}
#endif // !MODEL_H