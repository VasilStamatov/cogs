#include "Material.h"

namespace cogs
{
		namespace graphics
		{
				std::map<std::string, std::shared_ptr<MaterialData>> Material::s_resourceMap;

				Material::Material(const std::string & _name) : m_name(_name)
				{
						auto iter = s_resourceMap.find(m_name);

						if (iter == s_resourceMap.end())
						{
								std::shared_ptr<MaterialData> newMaterialData = std::make_shared<MaterialData>();
								s_resourceMap.insert(std::make_pair(m_name, std::move(newMaterialData)));
								m_materialData = s_resourceMap.at(m_name);
						}
						else
						{
								m_materialData = iter->second;
						}
				}

				Material::Material(const std::string & _name, const GLSLProgram & _shader) : m_name(_name)
				{
						auto iter = s_resourceMap.find(m_name);

						if (iter == s_resourceMap.end())
						{
								std::shared_ptr<MaterialData> newMaterialData = std::make_shared<MaterialData>(_shader);
								s_resourceMap.insert(std::make_pair(m_name, std::move(newMaterialData)));
								m_materialData = s_resourceMap.at(m_name);
						}
						else
						{
								m_materialData = iter->second;
						}
				}

				Material::Material(const Material & _other) :
						m_name(_other.m_name),
						m_materialData(_other.m_materialData)
				{
				}
				Material::~Material()
				{
						m_materialData.reset();
				}
		}
}