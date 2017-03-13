#include "ResourceManager.h"
#include "Utils.h"
#include "GLSLProgram.h"
#include "GLTexture2D.h"
#include "GLCubemapTexture.h"
#include "Material.h"
#include "Mesh.h"
#include "Sprite.h"

namespace cogs
{
		std::map<std::string, std::shared_ptr<GLSLProgram>> ResourceManager::s_shaderMap = {};
		std::map<std::string, std::shared_ptr<GLTexture2D>> ResourceManager::s_glTex2DMap = {};
		std::map<std::string, std::shared_ptr<Material>> ResourceManager::s_materialMap = {};
		std::map<std::string, std::shared_ptr<Mesh>> ResourceManager::s_meshMap = {};
		std::map<std::string, std::shared_ptr<Sprite>> ResourceManager::s_spriteMap = {};
		std::map<std::string, std::shared_ptr<GLCubemapTexture>> ResourceManager::s_glTex3DMap = {};

		std::weak_ptr<GLSLProgram> ResourceManager::getGLSLProgram(const std::string & _name)
		{
				auto iter = s_shaderMap.find(_name);

				//check if it's not in the map
				if (iter == s_shaderMap.end())
				{
						//return an empty/expired pointer
						return std::weak_ptr<GLSLProgram>();
				}
				else
				{
						//return the found resource
						return iter->second;
				}
		}

		std::weak_ptr<GLSLProgram> ResourceManager::getGLSLProgram(const std::string & _name,
				const std::string & _vsFilePath, const std::string & _fsFilePath, const std::string & _gsFilePath)
		{
				auto iter = s_shaderMap.find(_name);

				//check if it's not in the map
				if (iter == s_shaderMap.end())
				{
						//if the resource does not exist, create it
						std::shared_ptr<GLSLProgram> newShader =
								std::make_shared<GLSLProgram>(_name, _vsFilePath, _fsFilePath, _gsFilePath);

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

		std::weak_ptr<GLTexture2D> ResourceManager::getGLTexture2D(const std::string & _filePath)
		{
				auto iter = s_glTex2DMap.find(_filePath);

				//check if it's not in the map
				if (iter == s_glTex2DMap.end())
				{
						// if the resource does not exist, create it
						std::shared_ptr<GLTexture2D> newTexture =
								std::make_shared<GLTexture2D>(_filePath, _filePath);

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

		std::weak_ptr<GLTexture2D> ResourceManager::getGLTexture2D(const std::string & _filePath,
				const std::string & _name)
		{
				auto iter = s_glTex2DMap.find(_filePath);

				//check if it's not in the map
				if (iter == s_glTex2DMap.end())
				{
						// if the resource does not exist, create it
						std::shared_ptr<GLTexture2D> newTexture =
								std::make_shared<GLTexture2D>(_name, _filePath);

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

		std::weak_ptr<GLCubemapTexture> ResourceManager::getGLCubemap(const std::string & _name)
		{
				auto iter = s_glTex3DMap.find(_name);

				//check if it's not in the map
				if (iter == s_glTex3DMap.end())
				{
						return std::weak_ptr<GLCubemapTexture>();
				}
				else
				{
						//return the found resource
						return iter->second;
				}
		}

		std::weak_ptr<GLCubemapTexture> ResourceManager::getGLCubemap(const std::string & _name, const std::vector<std::string>& _fileNames)
		{
				auto iter = s_glTex3DMap.find(_name);

				//check if it's not in the map
				if (iter == s_glTex3DMap.end())
				{
						// if the resource does not exist, create it
						std::shared_ptr<GLCubemapTexture> newTexture =
								std::make_shared<GLCubemapTexture>(_name, _fileNames);

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

		std::weak_ptr<Material> ResourceManager::getMaterial(const std::string & _name)
		{
				auto iter = s_materialMap.find(_name);

				//check if it's not in the map
				if (iter == s_materialMap.end())
				{
						// if the resource does not exist, create it
						std::shared_ptr<Material> newMaterial = std::make_shared<Material>(_name);

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

		std::weak_ptr<Mesh> ResourceManager::getMesh(const std::string & _filePath)
		{
				auto iter = s_meshMap.find(_filePath);

				//check if it's not in the map
				if (iter == s_meshMap.end())
				{
						// if the resource does not exist, create it
						std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>(_filePath);

						//insert it into the resource map
						s_meshMap.insert(std::make_pair(_filePath, std::move(newMesh)));

						//return it as it 100% exists
						return s_meshMap.at(_filePath);
				}
				else
				{
						//return the found resource
						return iter->second;
				}
		}

		//std::weak_ptr<Mesh> ResourceManager::getPrimitive(const std::string& _filePath)
		//{
		//		auto iter = s_meshMap.find(_filePath);

		//		//check if it's not in the map
		//		if (iter == s_meshMap.end())
		//		{
		//				// if the resource does not exist, create it
		//				std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>(loadPrimitive(_filePath));

		//				//insert it into the resource map
		//				s_meshMap.insert(std::make_pair(_filePath, std::move(newMesh)));

		//				//return it as it 100% exists
		//				return s_meshMap.at(_filePath);
		//		}
		//		else
		//		{
		//				//return the found resource
		//				return iter->second;
		//		}
		//}

		std::weak_ptr<Sprite> ResourceManager::getSprite(const std::string & _name)
		{
				auto iter = s_spriteMap.find(_name);

				//check if it's not in the map
				if (iter == s_spriteMap.end())
				{
						//return an empty/expired pointer
						return std::weak_ptr<Sprite>();
				}
				else
				{
						//return the found resource
						return iter->second;
				}
		}

		std::weak_ptr<Sprite> ResourceManager::getSprite(const std::string& _name,
				std::weak_ptr<GLTexture2D> _texture,
				const glm::vec2& _size, const Color& _color)
		{
				auto iter = s_spriteMap.find(_name);

				//check if it's not in the map
				if (iter == s_spriteMap.end())
				{
						// if the resource does not exist, create it
						std::shared_ptr<Sprite> newMaterial =
								std::make_shared<Sprite>(_name, _texture, _size, _color);

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
				s_meshMap.clear();
		}

		void ResourceManager::clearSprites()
		{
				s_spriteMap.clear();
		}
}