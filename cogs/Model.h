#ifndef MODEL_H
#define MODEL_H

#include "ModelData.h"

namespace cogs
{
		namespace graphics
		{
				class Model
				{
				public:
						Model(const std::string& _name, const std::string& _filePath);
						Model(const std::string& _name, const std::vector<graphics::Mesh>& _meshes);
						Model(const Model& _other);
						~Model();

						void render();

						void reupload() { m_modelData.lock()->reupload(); }
						bool isValid() { return m_modelData.lock()->isValid(); }
						void addMesh(const Mesh& _mesh) { m_modelData.lock()->addMesh(_mesh); }

						inline const std::string& getName()			const { return m_name; }
						inline const std::string& getPath()			const { return m_modelData.lock()->getPath(); }
						inline std::vector<Mesh>& getMeshes() const { return m_modelData.lock()->getMeshes(); }

				private:
						static std::map<std::string, std::shared_ptr<ModelData>> s_resourceMap;

						std::weak_ptr<ModelData> m_modelData;
						std::string m_name{ "" };
				};
		}
}
#endif // !MODEL_H