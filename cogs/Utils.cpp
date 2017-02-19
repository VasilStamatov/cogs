#include "Utils.h"

#include "ResourceManager.h"
#include "MeshRenderer.h"

#include <SDL\SDL_timer.h>
#include <SOIL2\SOIL2.h>

#include <GL\glew.h>

namespace cogs
{
		namespace utils
		{
				float getTime()
				{
						return static_cast<float>(SDL_GetTicks());
				}

				void sleep(const float _millis)
				{
						SDL_Delay(static_cast<Uint32>(_millis));
				}

				bool loadTexture(const char * _filePath, bool _alpha, int * _width, int * _height, unsigned int * _id)
				{
						unsigned char* image = SOIL_load_image(_filePath, _width, _height, 0, _alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

						if (image == nullptr)
						{
								return false;
						}

						//Generate the openGL texture object
						glGenTextures(1, _id);

						//Bind the texture object
						glBindTexture(GL_TEXTURE_2D, *_id);

						//Upload the pixels to the texture
						glTexImage2D(GL_TEXTURE_2D, 0, _alpha ? GL_RGBA : GL_RGB, *_width, *_height, 0, _alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
						//Generate the mipmaps
						glGenerateMipmap(GL_TEXTURE_2D);

						//Set some texture parameters
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

						//Unbind the texture
						glBindTexture(GL_TEXTURE_2D, 0);

						SOIL_free_image_data(image);

						return true;
				}

				bool loadCubemap(const std::vector<std::string>& _fileNames, int * _width, int * _height, unsigned int * _id)
				{
						//Generate the openGL handle for the cubemap
						glGenTextures(1, _id);

						//bind the cubemap
						glBindTexture(GL_TEXTURE_CUBE_MAP, *_id);

						for (GLuint i = 0; i < 6; i++)
						{
								unsigned char* image = SOIL_load_image(_fileNames.at(i).c_str(), _width, _height, 0, SOIL_LOAD_RGB);
								if (image == nullptr)
								{
										return false;
								}
								glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, *_width, *_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
								SOIL_free_image_data(image);
						}

						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
						glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

						return true;
				}

				graphics::Mesh loadPrimitive(const std::string & _filePath)
				{
						Assimp::Importer importer;

						const aiScene* scene = importer.ReadFile(_filePath.c_str(),
								aiProcess_Triangulate |
								aiProcess_GenSmoothNormals |
								//aiProcess_FlipUVs |
								aiProcess_CalcTangentSpace);

						if (!scene)
						{
								printf("Mesh failed to load");
								assert(false);
						}

						//this is supposed to be a primitive with only 1 mesh so get the first in the scene
						const aiMesh* mesh = scene->mMeshes[0];
						if (mesh == nullptr)
						{
								assert(false);
						}

						std::vector<glm::vec3> positions(mesh->mNumVertices);
						std::vector<glm::vec2> uvs(mesh->mNumVertices);
						std::vector<glm::vec3> normals(mesh->mNumVertices);
						std::vector<glm::vec3> tangents(mesh->mNumVertices);
						std::vector<unsigned int> indices;
						indices.reserve(mesh->mNumFaces * 3);

						const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);

						//load all the per-vertex data
						for (unsigned int currVert = 0; currVert < mesh->mNumVertices; ++currVert)
						{
								const aiVector3D pos = mesh->mVertices[currVert];
								const aiVector3D normal = mesh->mNormals[currVert];
								const aiVector3D uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][currVert] : aiZeroVector;
								const aiVector3D tangent = mesh->mTangents[currVert];

								positions.at(currVert) = (glm::vec3(pos.x, pos.y, pos.z));
								uvs.at(currVert) = (glm::vec2(uv.x, uv.y));
								normals.at(currVert) = (glm::vec3(normal.x, normal.y, normal.z));
								tangents.at(currVert) = (glm::vec3(tangent.x, tangent.y, tangent.z));
						}

						//load all the indices for indexed rendering
						for (unsigned int currFace = 0; currFace < mesh->mNumFaces; ++currFace)
						{
								const aiFace& face = mesh->mFaces[currFace];
								assert(face.mNumIndices == 3);
								indices.push_back(face.mIndices[0]);
								indices.push_back(face.mIndices[1]);
								indices.push_back(face.mIndices[2]);
						}

						return graphics::Mesh(mesh->mName.C_Str(), indices, positions, uvs, normals, tangents);
				}
				std::shared_ptr<ecs::Entity> loadEntityWithMeshes(const std::string & _filePath, std::weak_ptr<graphics::Renderer3D> _renderer)
				{
						Assimp::Importer importer;

						const aiScene* scene = importer.ReadFile(_filePath.c_str(),
								aiProcess_Triangulate |
								aiProcess_GenSmoothNormals |
								//aiProcess_FlipUVs |
								aiProcess_CalcTangentSpace);

						//error check
						if (!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
						{
								printf("ERROR::ASSIMP:: %s \n", importer.GetErrorString());
								assert(false);
						}
						std::shared_ptr<ecs::Entity> mainHandle = ecs::Entity::create(scene->mRootNode->mName.C_Str());

						const std::string directory = _filePath.substr(0, _filePath.find_last_of('/'));

						internal::processNode(scene->mRootNode, scene, directory, mainHandle, _renderer);

						return std::move(mainHandle);
				}

				void loadMeshesToEntity(std::weak_ptr<ecs::Entity> _mainHolder,
						const std::string & _filePath, std::weak_ptr<graphics::Renderer3D> _renderer)
				{
						Assimp::Importer importer;

						const aiScene* scene = importer.ReadFile(_filePath.c_str(),
								aiProcess_Triangulate |
								aiProcess_GenSmoothNormals |
								//aiProcess_FlipUVs |
								aiProcess_CalcTangentSpace);

						//error check
						if (!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
						{
								printf("ERROR::ASSIMP:: %s \n", importer.GetErrorString());
								assert(false);
						}

						const std::string directory = _filePath.substr(0, _filePath.find_last_of('/'));

						internal::processNode(scene->mRootNode, scene, directory, _mainHolder, _renderer);
				}

				void internal::processNode(aiNode* _node, const aiScene* _scene, const std::string& _directory,
						std::weak_ptr<ecs::Entity> _parent,
						std::weak_ptr<graphics::Renderer3D> _renderer)
				{
						for (GLuint i = 0; i < _node->mNumMeshes; i++)
						{
								aiMesh* assimpMesh = _scene->mMeshes[_node->mMeshes[i]];
								std::weak_ptr<graphics::Mesh> cogsMesh = utils::ResourceManager::getMesh(assimpMesh->mName.C_Str());
								std::weak_ptr<graphics::Material> cogsMaterial = utils::ResourceManager::getMaterial(assimpMesh->mName.C_Str());

								//check if it has no positions (not loaded)
								if (cogsMesh.lock()->getPositions().empty())
								{
										processMesh(assimpMesh, cogsMesh);
								}

								//check if it has no diffuse map (not loaded)
								if (cogsMaterial.lock()->getDiffuseMap().expired())
								{
										//load all the textures for this mesh
										if (assimpMesh->mMaterialIndex >= 0)
										{
												aiMaterial* material = _scene->mMaterials[assimpMesh->mMaterialIndex];
												processMaterial(material, cogsMaterial, _directory);
										}
								}

								std::weak_ptr<ecs::Entity> child = _parent.lock()->addChild(_node->mName.C_Str());
								child.lock()->addComponent<ecs::MeshRenderer>(cogsMesh, cogsMaterial, _renderer);
						}

						for (GLuint i = 0; i < _node->mNumChildren; i++)
						{
								aiNode* aiChild = _node->mChildren[i];

								std::weak_ptr<ecs::Entity> child = _parent.lock()->getChild(aiChild->mName.C_Str());

								if (child.expired())
								{
										processNode(aiChild, _scene, _directory, _parent, _renderer);
								}
								else
								{
										processNode(aiChild, _scene, _directory, child, _renderer);
								}
						}
				}

				void internal::processMesh(aiMesh* _aiMesh, std::weak_ptr<graphics::Mesh> _cogsMesh)
				{
						std::vector<glm::vec3> positions(_aiMesh->mNumVertices);
						std::vector<glm::vec2> uvs(_aiMesh->mNumVertices);
						std::vector<glm::vec3> normals(_aiMesh->mNumVertices);
						std::vector<glm::vec3> tangents(_aiMesh->mNumVertices);
						std::vector<unsigned int> indices;
						indices.reserve(_aiMesh->mNumFaces * 3);

						const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);

						//load all the per-vertex data
						for (unsigned int currVert = 0; currVert < _aiMesh->mNumVertices; ++currVert)
						{
								const aiVector3D pos = _aiMesh->mVertices[currVert];
								const aiVector3D normal = _aiMesh->mNormals[currVert];
								const aiVector3D uv = _aiMesh->HasTextureCoords(0) ? _aiMesh->mTextureCoords[0][currVert] : aiZeroVector;
								const aiVector3D tangent = _aiMesh->mTangents[currVert];

								positions.at(currVert) = (glm::vec3(pos.x, pos.y, pos.z));
								uvs.at(currVert) = (glm::vec2(uv.x, uv.y));
								normals.at(currVert) = (glm::vec3(normal.x, normal.y, normal.z));
								tangents.at(currVert) = (glm::vec3(tangent.x, tangent.y, tangent.z));
						}

						//load all the indices for indexed rendering
						for (unsigned int currFace = 0; currFace < _aiMesh->mNumFaces; ++currFace)
						{
								const aiFace& face = _aiMesh->mFaces[currFace];
								assert(face.mNumIndices == 3);
								indices.push_back(face.mIndices[0]);
								indices.push_back(face.mIndices[1]);
								indices.push_back(face.mIndices[2]);
						}

						_cogsMesh.lock()->setPositions(positions);
						_cogsMesh.lock()->setTexCoords(uvs);
						_cogsMesh.lock()->setNormals(normals);
						_cogsMesh.lock()->setTangents(tangents);
						_cogsMesh.lock()->setIndices(indices);
						_cogsMesh.lock()->reupload();
				}

				void internal::processMaterial(aiMaterial* _aiMaterial,
						std::weak_ptr<graphics::Material> _cogsMaterial, const std::string& _directory)
				{
						if (_aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
						{
								aiString str;
								_aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str);
								_cogsMaterial.lock()->setDiffuseMap(
										ResourceManager::getGLTexture2D(_directory + "/" + str.C_Str(), "texture_diffuse"));
						}
						if (_aiMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
						{
								aiString str;
								_aiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &str);
								_cogsMaterial.lock()->setSpecularMap(
										ResourceManager::getGLTexture2D(_directory + "/" + str.C_Str(), "texture_specular"));
						}
						if (_aiMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0)
						{
								aiString str;
								_aiMaterial->GetTexture(aiTextureType_AMBIENT, 0, &str);
								_cogsMaterial.lock()->setReflectionMap(
										ResourceManager::getGLTexture2D(_directory + "/" + str.C_Str(), "texture_reflection"));
						}
						if (_aiMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0)
						{
								aiString str;
								_aiMaterial->GetTexture(aiTextureType_HEIGHT, 0, &str);
								_cogsMaterial.lock()->setNormalMap(
										ResourceManager::getGLTexture2D(_directory + "/" + str.C_Str(), "texture_normal"));
						}
				}
		}
}