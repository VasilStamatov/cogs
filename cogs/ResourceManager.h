#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "GLSLProgram.h"
#include "GLTexture2D.h"
#include "Material.h"
#include "Model.h"
#include "Sprite.h"

#include <map>

namespace cogs
{
		namespace utils
		{
				class ResourceManager
				{
				public:
						/* GLSLProgram getters */
						static std::weak_ptr<graphics::GLSLProgram> getGLSLProgram(const std::string& _name);
						static std::weak_ptr<graphics::GLSLProgram> getGLSLProgram(const std::string& _name,
								const std::string& _vsFilePath,
								const std::string& _fsFilePath,
								const std::string& _gsFilePath = "");

						/* GLTexture2D getters */
						static std::weak_ptr<graphics::GLTexture2D> getGLTexture2D(const std::string& _filePath);
						static std::weak_ptr<graphics::GLTexture2D> getGLTexture2D(const std::string& _filePath, const std::string& _name, bool _alpha = true);

						/* Material getters */
						static std::weak_ptr<graphics::Material> getMaterial(const std::string& _name);

						/* Model getters */
						static std::weak_ptr<graphics::Model> getModel(const std::string& _filePath);
						static std::weak_ptr<graphics::Model> getModel(const std::string& _name, const std::string& _filePath);
						static std::weak_ptr<graphics::Model> getModel(const std::string& _name, const std::vector<graphics::Mesh>& _meshes);

						/* Sprite getters */
						static std::weak_ptr<graphics::Sprite> getSprite(const std::string& _name);
						static std::weak_ptr<graphics::Sprite> getSprite(const std::string& _name,
								std::weak_ptr<graphics::GLTexture2D> _texture,
								const glm::vec2& _size, const graphics::Color& _color);

						/* clear all resource maps */
						static void clear();

						/* clear the shader map */
						static void clearGLSLPrograms();

						/* clear the GLTexture2D map */
						static void clearGLTexture2Ds();

						/* clear the material map */
						static void clearMaterials();

						/* clear the model map */
						static void clearModels();

						/* clear the sprite map */
						static void clearSprites();

				private:
						ResourceManager() {}
						~ResourceManager() {}

				private:
						static std::map<std::string, std::shared_ptr<graphics::GLSLProgram>> s_shaderMap;
						static std::map<std::string, std::shared_ptr<graphics::GLTexture2D>> s_glTex2DMap;
						static std::map<std::string, std::shared_ptr<graphics::Material>> s_materialMap;
						static std::map<std::string, std::shared_ptr<graphics::Model>> s_modelMap;
						static std::map<std::string, std::shared_ptr<graphics::Sprite>> s_spriteMap;
				};
		}
}

#endif // !RESOURCE_MANAGER_H