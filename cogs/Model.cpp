#include "Model.h"

namespace cogs
{
		namespace graphics
		{
				std::map<std::string, std::shared_ptr<ModelData>> Model::s_resourceMap;

				Model::Model(const std::string& _name, const std::string& _filePath) : m_name(_name)
				{
						auto iter = s_resourceMap.find(_filePath);

						if (iter == s_resourceMap.end())
						{
								std::shared_ptr<ModelData> newModelData = std::make_shared<ModelData>(_filePath);
								s_resourceMap.insert(std::make_pair(_filePath, std::move(newModelData)));
								m_modelData = s_resourceMap.at(_filePath);
						}
						else
						{
								m_modelData = iter->second;
						}
				}

				Model::Model(const std::string& _name, const std::vector<graphics::Mesh>& _meshes) : m_name(_name)
				{
						auto iter = s_resourceMap.find(m_name);

						//check if it's not in the map
						if (iter == s_resourceMap.end())
						{
								std::shared_ptr<ModelData> newModelData = std::make_shared<ModelData>(_meshes, m_name);
								s_resourceMap.insert(std::make_pair(m_name, std::move(newModelData)));
								m_modelData = s_resourceMap.at(m_name);
						}
						else
						{
								m_modelData = iter->second;
						}
				}

				Model::Model(const Model & _other) : 
						m_modelData(_other.m_modelData),
						m_name(_other.m_name) {}

				void Model::render()
				{
						m_modelData.lock()->render();
				}

				Model::~Model()
				{
						m_modelData.reset();
				}
		}
}