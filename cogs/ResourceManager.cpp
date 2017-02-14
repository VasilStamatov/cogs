#include "ResourceManager.h"

namespace cogs
{
		namespace utils
		{
				std::map<std::string, std::shared_ptr<graphics::GLSLProgram>> ResourceManager::s_shaderMap = {};
				std::map<std::string, std::shared_ptr<graphics::GLTexture2D>> ResourceManager::s_glTex2DMap = {};
				std::map<std::string, std::shared_ptr<graphics::Material>> ResourceManager::s_materialMap = {};
				std::map<std::string, std::shared_ptr<graphics::Model>> ResourceManager::s_modelMap = {};
				std::map<std::string, std::shared_ptr<graphics::Sprite>> ResourceManager::s_spriteMap = {};
				std::map<std::string, std::shared_ptr<graphics::GLCubemapTexture>> ResourceManager::s_glTex3DMap = {};

				std::weak_ptr<graphics::GLSLProgram> ResourceManager::getGLSLProgram(const std::string & _name)
				{
						auto iter = s_shaderMap.find(_name);

						//check if it's not in the map
						if (iter == s_shaderMap.end())
						{
								//return an empty/expired pointer
								return std::weak_ptr<graphics::GLSLProgram>();
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::GLSLProgram> ResourceManager::getGLSLProgram(const std::string & _name,
						const std::string & _vsFilePath, const std::string & _fsFilePath, const std::string & _gsFilePath)
				{
						auto iter = s_shaderMap.find(_name);

						//check if it's not in the map
						if (iter == s_shaderMap.end())
						{
								//if the resource does not exist, create it
								std::shared_ptr<graphics::GLSLProgram> newShader =
										std::make_shared<graphics::GLSLProgram>(_name, _vsFilePath, _fsFilePath, _gsFilePath);

								//insert it into the resource map
								s_shaderMap.insert(std::make_pair(_name, std::move(newShader)));

								//return it as it 100% exists
								return s_shaderMap.at(_name);
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::GLTexture2D> ResourceManager::getGLTexture2D(const std::string & _filePath)
				{
						auto iter = s_glTex2DMap.find(_filePath);

						//check if it's not in the map
						if (iter == s_glTex2DMap.end())
						{
								//return an empty/expired pointer
								return std::weak_ptr<graphics::GLTexture2D>();
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::GLTexture2D> ResourceManager::getGLTexture2D(const std::string & _filePath,
						const std::string & _name, bool _alpha)
				{
						auto iter = s_glTex2DMap.find(_filePath);

						//check if it's not in the map
						if (iter == s_glTex2DMap.end())
						{
								// if the resource does not exist, create it
								std::shared_ptr<graphics::GLTexture2D> newTexture = 
										std::make_shared<graphics::GLTexture2D>(_name, _filePath, _alpha);

								//insert it into the resource map
								s_glTex2DMap.insert(std::make_pair(_filePath, std::move(newTexture)));

								//return it as it 100% exists
								return s_glTex2DMap.at(_filePath);
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::GLCubemapTexture> ResourceManager::getGLCubemap(const std::string & _name)
				{
						auto iter = s_glTex3DMap.find(_name);

						//check if it's not in the map
						if (iter == s_glTex3DMap.end())
						{
								return std::weak_ptr<graphics::GLCubemapTexture>();
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::GLCubemapTexture> ResourceManager::getGLCubemap(const std::string & _name, const std::vector<std::string>& _fileNames)
				{
						auto iter = s_glTex3DMap.find(_name);

						//check if it's not in the map
						if (iter == s_glTex3DMap.end())
						{
								// if the resource does not exist, create it
								std::shared_ptr<graphics::GLCubemapTexture> newTexture =
										std::make_shared<graphics::GLCubemapTexture>(_name, _fileNames);

								//insert it into the resource map
								s_glTex3DMap.insert(std::make_pair(_name, std::move(newTexture)));

								//return it as it 100% exists
								return s_glTex3DMap.at(_name);
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::Material> ResourceManager::getMaterial(const std::string & _name)
				{
						auto iter = s_materialMap.find(_name);

						//check if it's not in the map
						if (iter == s_materialMap.end())
						{
								// if the resource does not exist, create it
								std::shared_ptr<graphics::Material> newMaterial = std::make_shared<graphics::Material>(_name);

								//insert it into the resource map
								s_materialMap.insert(std::make_pair(_name, std::move(newMaterial)));

								//return it as it 100% exists
								return s_materialMap.at(_name);
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::Model> ResourceManager::getModel(const std::string & _filePath)
				{
						auto iter = s_modelMap.find(_filePath);

						//check if it's not in the map
						if (iter == s_modelMap.end())
						{
								//return an empty/expired pointer
								return std::weak_ptr<graphics::Model>();
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::Model> ResourceManager::getModel(const std::string & _name, const std::string & _filePath)
				{
						auto iter = s_modelMap.find(_filePath);

						//check if it's not in the map
						if (iter == s_modelMap.end())
						{
								// if the resource does not exist, create it
								std::shared_ptr<graphics::Model> newMaterial = std::make_shared<graphics::Model>(_name, _filePath);

								//insert it into the resource map
								s_modelMap.insert(std::make_pair(_filePath, std::move(newMaterial)));

								//return it as it 100% exists
								return s_modelMap.at(_filePath);
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::Model> ResourceManager::getModel(const std::string & _name,
						const std::vector<graphics::Mesh>& _meshes)
				{
						auto iter = s_modelMap.find(_name);

						//check if it's not in the map
						if (iter == s_modelMap.end())
						{
								// if the resource does not exist, create it
								std::shared_ptr<graphics::Model> newMaterial = std::make_shared<graphics::Model>(_name, _meshes);

								//insert it into the resource map
								s_modelMap.insert(std::make_pair(_name, std::move(newMaterial)));

								//return it as it 100% exists
								return s_modelMap.at(_name);
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::Sprite> ResourceManager::getSprite(const std::string & _name)
				{
						auto iter = s_spriteMap.find(_name);

						//check if it's not in the map
						if (iter == s_spriteMap.end())
						{
								//return an empty/expired pointer
								return std::weak_ptr<graphics::Sprite>();
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				std::weak_ptr<graphics::Sprite> ResourceManager::getSprite(const std::string& _name,
						std::weak_ptr<graphics::GLTexture2D> _texture,
						const glm::vec2& _size, const graphics::Color& _color)
				{
						auto iter = s_spriteMap.find(_name);

						//check if it's not in the map
						if (iter == s_spriteMap.end())
						{
								// if the resource does not exist, create it
								std::shared_ptr<graphics::Sprite> newMaterial =
										std::make_shared<graphics::Sprite>(_name, _texture, _size, _color);

								//insert it into the resource map
								s_spriteMap.insert(std::make_pair(_name, std::move(newMaterial)));

								//return it as it 100% exists
								return s_spriteMap.at(_name);
						}
						else
						{
								//return the found resource
								return iter->second;
						}
				}

				void ResourceManager::clear()
				{
						clearGLSLPrograms();
						clearGLTexture2Ds();
						clearGLCubemaps();
						clearMaterials();
						clearModels();
						clearSprites();
				}

				void ResourceManager::clearGLSLPrograms()
				{
						s_shaderMap.clear();
				}

				void ResourceManager::clearGLTexture2Ds()
				{
						s_glTex2DMap.clear();
				}

				void ResourceManager::clearGLCubemaps()
				{
						s_glTex3DMap.clear();
				}

				void ResourceManager::clearMaterials()
				{
						s_materialMap.clear();
				}

				void ResourceManager::clearModels()
				{
						s_modelMap.clear();
				}

				void ResourceManager::clearSprites()
				{
						s_spriteMap.clear();
				}
		}
}