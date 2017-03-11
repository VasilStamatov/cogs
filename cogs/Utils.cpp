#include "Utils.h"

#include "ResourceManager.h"
//#include "MeshRenderer.h"

#include <SDL\SDL_timer.h>
#include <SOIL2\SOIL2.h>

#include <GL\glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

				void loadMesh(const std::string& _filePath,
						std::vector<graphics::SubMesh>& _subMeshes,
						std::vector<glm::vec3>& _positions,
						std::vector<glm::vec2>& _uvs,
						std::vector<glm::vec3>& _normals,
						std::vector<glm::vec3>& _tangents,
						std::vector<unsigned int>& _indices,
						std::vector<std::weak_ptr<graphics::Material>>& _materials)
				{
						Assimp::Importer importer;

						const aiScene* scene = importer.ReadFile(_filePath,
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

						_subMeshes.resize(scene->mNumMeshes);
						_materials.resize(scene->mNumMaterials);

						unsigned int numVertices{ 0 };
						unsigned int numIndices{ 0 };

						for (size_t i = 0; i < _subMeshes.size(); i++)
						{
								_subMeshes.at(i).m_materialIndex = scene->mMeshes[i]->mMaterialIndex;
								_subMeshes.at(i).m_numIndices = scene->mMeshes[i]->mNumFaces * 3;
								_subMeshes.at(i).m_baseVertex = numVertices;
								_subMeshes.at(i).m_baseIndex = numIndices;

								numVertices += scene->mMeshes[i]->mNumVertices;
								numIndices += _subMeshes.at(i).m_numIndices;
						}

						_positions.reserve(numVertices);
						_uvs.reserve(numVertices);
						_normals.reserve(numVertices);
						_tangents.reserve(numVertices);
						_indices.reserve(numIndices);

						const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);

						// Initialize the meshes in the scene one by one
						for (unsigned int i = 0; i < _subMeshes.size(); i++) 
						{
								const aiMesh* paiMesh = scene->mMeshes[i];

								//load all the per-vertex data
								for (unsigned int currVert = 0; currVert < paiMesh->mNumVertices; ++currVert)
								{
										const aiVector3D pos				 = paiMesh->mVertices[currVert];
										const aiVector3D normal  = paiMesh->mNormals[currVert];
										const aiVector3D uv					 = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][currVert] : aiZeroVector;
										const aiVector3D tangent = paiMesh->mTangents[currVert];

										_positions.push_back(glm::vec3(pos.x, pos.y, pos.z));
										_uvs.push_back(glm::vec2(uv.x, uv.y));
										_normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
										_tangents.push_back(glm::vec3(tangent.x, tangent.y, tangent.z));
								}

								//load all the indices for indexed rendering
								for (unsigned int currFace = 0; currFace < paiMesh->mNumFaces; ++currFace)
								{
										const aiFace& face = paiMesh->mFaces[currFace];
										assert(face.mNumIndices == 3);
										_indices.push_back(face.mIndices[0]);
										_indices.push_back(face.mIndices[1]);
										_indices.push_back(face.mIndices[2]);
								}
						}

						for (size_t i = 0; i < scene->mNumMaterials; i++)
						{
								aiMaterial* paiMaterial = scene->mMaterials[i];

								std::string materialName = directory + "/" + scene->mRootNode->mName.C_Str() + std::to_string(i);

								std::weak_ptr<graphics::Material> material = utils::ResourceManager::getMaterial(materialName);

								if (material.lock()->getDiffuseMap().expired())
								{
										//load materials data
										if (paiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
										{
												aiString str;
												paiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str);
												material.lock()->setDiffuseMap(
														ResourceManager::getGLTexture2D(directory + "/" + str.C_Str(), "texture_diffuse"));
										}
										if (paiMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
										{
												aiString str;
												paiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &str);
												material.lock()->setSpecularMap(
														ResourceManager::getGLTexture2D(directory + "/" + str.C_Str(), "texture_specular"));
										}
										if (paiMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0)
										{
												aiString str;
												paiMaterial->GetTexture(aiTextureType_AMBIENT, 0, &str);
												material.lock()->setReflectionMap(
														ResourceManager::getGLTexture2D(directory + "/" + str.C_Str(), "texture_reflection"));
										}
										if (paiMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0)
										{
												aiString str;
												paiMaterial->GetTexture(aiTextureType_HEIGHT, 0, &str);
												material.lock()->setNormalMap(
														ResourceManager::getGLTexture2D(directory + "/" + str.C_Str(), "texture_normal"));
										}
								}
								_materials.at(i) = material;
						}
				}
		}
}