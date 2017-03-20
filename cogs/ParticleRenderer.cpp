#include "ParticleRenderer.h"

#include "Entity.h"
#include "GLTexture2D.h"
#include "Camera.h"
#include "GLSLProgram.h"
#include "ParticleSystem.h"

#include <GL\glew.h>
#include <glm\gtx\norm.hpp>

namespace cogs
{
		ParticleRenderer::ParticleRenderer(std::weak_ptr<GLSLProgram> _shader) : Renderer(_shader)
		{
				init();
		}
		ParticleRenderer::~ParticleRenderer()
		{
				dispose();
		}
		void ParticleRenderer::init()
		{
				//generate the vertex array buffer
				glGenVertexArrays(1, &m_VAO);

				//generate the vertex buffer objects
				glGenBuffers(BufferObjects::NUM_BUFFERS, m_VBOs);

				//bind the vao and continue working on the vbos under it
				glBindVertexArray(m_VAO);

				//bind the position buffer
				glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::POSITION]);

				float vertices[] =
				{ -0.5f,  0.5f, 0.0f,	 // top left corner
						-0.5f, -0.5f, 0.0f,		// bottom left corner
						0.5f, -0.5f, 0.0f,	 	// bottom right corner
						0.5f,  0.5f, 0.0f }; // top right corner

				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(PARTICLE_POSITION_ATTRIBUTE);
				glVertexAttribPointer(PARTICLE_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

				unsigned int indices[] = { 0,1,2,			// first triangle (bottom left - top left - top right)
																															0,2,3 }; // second triangle (bottom left - top right - bottom right)

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[BufferObjects::INDEX]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::INSTANCED_ATTRIBS]);

				glEnableVertexAttribArray(PARTICLE_WORLDNSIZE_ATTRIBUTE);
				glVertexAttribPointer(PARTICLE_WORLDNSIZE_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttributes),
						(const GLvoid*)offsetof(InstanceAttributes, InstanceAttributes::worldPosAndSize));
				glVertexAttribDivisor(PARTICLE_WORLDNSIZE_ATTRIBUTE, 1);

				glEnableVertexAttribArray(PARTICLE_COLOR_ATTRIBUTE);
				glVertexAttribPointer(PARTICLE_COLOR_ATTRIBUTE, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceAttributes),
						(const GLvoid*)offsetof(InstanceAttributes, InstanceAttributes::color));
				glVertexAttribDivisor(PARTICLE_COLOR_ATTRIBUTE, 1);

				glEnableVertexAttribArray(PARTICLE_TEXOFFSETS_ATTRIBUTE);
				glVertexAttribPointer(PARTICLE_TEXOFFSETS_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttributes),
						(const GLvoid*)offsetof(InstanceAttributes, InstanceAttributes::texOffsets));
				glVertexAttribDivisor(PARTICLE_TEXOFFSETS_ATTRIBUTE, 1);

				glEnableVertexAttribArray(PARTICLE_BLEND_ATTRIBUTE);
				glVertexAttribPointer(PARTICLE_BLEND_ATTRIBUTE, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceAttributes),
						(const GLvoid*)offsetof(InstanceAttributes, InstanceAttributes::blendFactor));
				glVertexAttribDivisor(PARTICLE_BLEND_ATTRIBUTE, 1);

				glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceAttributes) * PARTICLE_MAX_INSTANCES, nullptr, GL_STREAM_DRAW);

				// unbind the vao after the setup is done
				glBindVertexArray(0);
		}
		void ParticleRenderer::begin()
		{
				m_particlesMap.clear();
		}
		void ParticleRenderer::submit(std::weak_ptr<Entity> _entity)
		{
				std::weak_ptr<ParticleSystem> particleSystem = _entity.lock()->getComponent<ParticleSystem>();
				std::weak_ptr<GLTexture2D> texture = particleSystem.lock()->getTexture();
				std::weak_ptr<Camera> currentCam = Camera::getCurrent();

				Particle* particles = particleSystem.lock()->getParticles();

				auto iter = m_particlesMap.find(texture.lock()->getTextureID());

				//check if it's not in the map
				if (iter == m_particlesMap.end())
				{
						InstanceData instance;
						if (texture.lock()->getDims().x == 0)
						{
								instance.texNumOfRows = 1;
						}
						else
						{
								instance.texNumOfRows = (float)texture.lock()->getDims().x;
						}
						instance.isTexAdditive = particleSystem.lock()->getAdditive();

						m_particlesMap.insert(std::make_pair(texture.lock()->getTextureID(), instance));
				}

				for (int i = 0; i < particleSystem.lock()->getMaxParticles(); i++)
				{
						if (particles[i].m_life > 0.0f)
						{
								//submit the mesh if it's in the view frustum
								if (currentCam.lock()->sphereInFrustum(particles[i].m_position, particles[i].m_radius))
								{
										float lifeFactor = abs(particles[i].m_life - 1.0f);
										int stageCount = texture.lock()->getDims().x * texture.lock()->getDims().y;
										float atlasProgression = lifeFactor * stageCount;
										float index1{ 0.0f }, index2{ 0.0f }, blend{ 0.0f };
										blend = modff(atlasProgression, &index1);
										index2 = index1 < stageCount - 1 ? index1 + 1 : index1;

										glm::vec2 texOffset1 = texture.lock()->getTexOffsets((int)(index1));
										glm::vec2 texOffset2 = texture.lock()->getTexOffsets((int)(index1));

										InstanceAttributes newInstance;
										newInstance.worldPosAndSize = glm::vec4(particles[i].m_position, particles[i].m_radius * 2.0f);
										newInstance.color = particles[i].m_color;
										newInstance.texOffsets = glm::vec4(texOffset1, texOffset2);
										newInstance.blendFactor = blend;

										m_particlesMap[texture.lock()->getTextureID()].instanceAttribs.push_back(newInstance);
								}
						}
				}
		}
		void ParticleRenderer::end()
		{
				sortParticles();
		}

		void ParticleRenderer::flush()
		{
				std::weak_ptr<Camera> currentCam = Camera::getCurrent();

				m_shader.lock()->use();
				m_shader.lock()->uploadValue("projection", currentCam.lock()->getProjectionMatrix());
				m_shader.lock()->uploadValue("view", currentCam.lock()->getViewMatrix());
				m_shader.lock()->uploadValue("cameraRight_worldSpace",
						currentCam.lock()->getEntity().lock()->getComponent<Transform>().lock()->worldRightAxis());
				m_shader.lock()->uploadValue("cameraUp_worldSpace",
						currentCam.lock()->getEntity().lock()->getComponent<Transform>().lock()->worldUpAxis());
				/* Bind the VAO. This sets up the opengl state we need, including the
				vertex attribute pointers and it binds the VBO */
				glBindVertexArray(m_VAO);

				glDepthMask(GL_FALSE);

				for (auto& it : m_particlesMap)
				{
						InstanceData instances = it.second;
						GLuint texID = it.first;

						if (instances.isTexAdditive)
						{
								glBlendFunc(GL_SRC_ALPHA, GL_ONE);
						}

						m_shader.lock()->uploadValue("texNumOfRows", instances.texNumOfRows);

						//bind the per-instance buffers
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::INSTANCED_ATTRIBS]);
						//upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstanceAttributes) * instances.instanceAttribs.size(), instances.instanceAttribs.data());

						glBindTexture(GL_TEXTURE_2D, texID);

						glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instances.instanceAttribs.size());

						if (instances.isTexAdditive)
						{
								glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						}
				}

				glDepthMask(GL_TRUE);

				//unbind the vao
				glBindVertexArray(0);

				m_shader.lock()->unUse();
		}
		void ParticleRenderer::dispose()
		{
				//Dispose of all the buffest if they have't been disposed already

				if (m_VAO != 0)
				{
						glDeleteVertexArrays(1, &m_VAO);
						m_VAO = 0;
				}

				if (m_VBOs[0] != 0)
				{
						glDeleteBuffers(BufferObjects::NUM_BUFFERS, m_VBOs);

						for (size_t i = 0; i < BufferObjects::NUM_BUFFERS; i++)
						{
								m_VBOs[i] = 0;
						}
				}
		}
		void ParticleRenderer::sortParticles()
		{
				for (auto& it : m_particlesMap)
				{
						InstanceData instances = it.second;

						if (instances.isTexAdditive)
						{
								continue;
						}

						std::weak_ptr<Camera> currentCam = Camera::getCurrent();
						const glm::vec3& cameraPos = currentCam.lock()->getEntity().lock()->getComponent<Transform>().lock()->worldPosition();

						std::sort(instances.instanceAttribs.begin(), instances.instanceAttribs.end(),
								[&cameraPos](const InstanceAttributes& _p1, const InstanceAttributes& _p2)
						{
								glm::vec3 p1WorldPos(_p1.worldPosAndSize);
								glm::vec3 p2WorldPos(_p2.worldPosAndSize);

								float distanceFromCamera1 = glm::length2(p1WorldPos - cameraPos);
								float distanceFromCamera2 = glm::length2(p2WorldPos - cameraPos);

								return (distanceFromCamera1 > distanceFromCamera2);
						});
				}
		}
}