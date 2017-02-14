#include "Utils.h"

#include "ResourceManager.h"

#include <SDL\SDL_timer.h>
#include <SOIL\SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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

				bool loadCubemap(const char ** _fileNames, int * _width, int * _height, unsigned int * _id)
				{
						//Generate the openGL handle for the cubemap
						glGenTextures(1, _id);

						//bind the cubemap
						glBindTexture(GL_TEXTURE_CUBE_MAP, *_id);

						for (GLuint i = 0; i < 6; i++)
						{
								unsigned char* image = SOIL_load_image(_fileNames[i], _width, _height, 0, SOIL_LOAD_RGB);
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

				std::vector<graphics::Mesh> loadModel(const std::string & _filePath)
				{
						Assimp::Importer importer;

						const aiScene* scene = importer.ReadFile(_filePath.c_str(),
								aiProcess_Triangulate |
								aiProcess_GenSmoothNormals |
								aiProcess_FlipUVs |
								aiProcess_CalcTangentSpace);

						if (!scene)
						{
								printf("Mesh failed to load");
								assert(false);
						}

						const std::string directory = _filePath.substr(0, _filePath.find_last_of('/'));

						std::vector<graphics::Mesh> meshes;
						meshes.reserve(scene->mNumMeshes);

						for (unsigned int currMesh = 0; currMesh < scene->mNumMeshes; ++currMesh)
						{
								const aiMesh* mesh = scene->mMeshes[currMesh];

								std::vector<glm::vec3> positions(mesh->mNumVertices);
								std::vector<glm::vec2> uvs(mesh->mNumVertices);
								std::vector<glm::vec3> normals(mesh->mNumVertices);
								std::vector<glm::vec3> tangents(mesh->mNumVertices);
								std::vector<unsigned int> indices;
								indices.reserve(mesh->mNumFaces * 3);

								std::weak_ptr<graphics::Material> meshMaterial = ResourceManager::getMaterial(mesh->mName.C_Str());

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

								//load all the textures for this mesh
								if (mesh->mMaterialIndex >= 0)
								{
										aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

										auto loadMaterialTexture = [&directory](aiMaterial * _mat, const aiTextureType & _type, const std::string & _name)
										{
												if (_mat->GetTextureCount(_type) > 0)
												{
														aiString str;
														_mat->GetTexture(_type, 0, &str);
														std::weak_ptr<graphics::GLTexture2D> texture = ResourceManager::getGLTexture2D(directory + "/" + str.C_Str(), _name);
														return texture;
												}
												return std::weak_ptr<graphics::GLTexture2D>();
										};

										// 1. Diffuse map
										meshMaterial.lock()->setDiffuseMap(loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse"));

										// 2. Specular maps
										meshMaterial.lock()->setSpecularMap(loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular"));

										// 3. reflection maps
										meshMaterial.lock()->setReflectionMap(loadMaterialTexture(material, aiTextureType_AMBIENT, "texture_reflection"));

										// 4. Normal maps
										meshMaterial.lock()->setNormalMap(loadMaterialTexture(material, aiTextureType_HEIGHT, "texture_normal"));
								}

								meshes.emplace_back(indices, positions, uvs, normals, tangents, meshMaterial);
						}

						return meshes;
				}
		}
}