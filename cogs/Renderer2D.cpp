#include "Renderer2D.h"

#include "Entity.h"
#include "Sprite.h"
#include "GLTexture2D.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "GLSLProgram.h"

#include <GL\glew.h>

namespace cogs
{
		Renderer2D::Renderer2D(std::weak_ptr<GLSLProgram> _shader) : Renderer(_shader)
		{
				init();
		}
		Renderer2D::Renderer2D()
		{
				init();
		}
		Renderer2D::~Renderer2D()
		{
				dispose();
		}
		void Renderer2D::init()
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
						0.5f, -0.5f, 0.0f, // bottom right corner
						0.5f,  0.5f, 0.0f };			// top right corner

				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				glEnableVertexAttribArray(BufferObjects::POSITION);
				glVertexAttribPointer(BufferObjects::POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

				// bind the color buffer
				glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::COLOR]);

				glEnableVertexAttribArray(BufferObjects::COLOR);
				glVertexAttribPointer(BufferObjects::COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Color), 0);
				glVertexAttribDivisor(BufferObjects::COLOR, 1);

				glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::SIZE]);

				glEnableVertexAttribArray(BufferObjects::SIZE);
				glVertexAttribPointer(BufferObjects::SIZE, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
				glVertexAttribDivisor(BufferObjects::SIZE, 1);

				unsigned int indices[] = { 0,1,2,			// first triangle (bottom left - top left - top right)
																															0,2,3 }; // second triangle (bottom left - top right - bottom right)

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[BufferObjects::INDEX]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

				// bind the buffer for world matrices
				glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::WORLDMAT]);
				// cannot upload mat4's all at once, so upload them as 4 vec4's
				for (size_t i = 0; i < 4; i++)
				{
						//enable the channel of the current matrix row (4,5,6,7)
						glEnableVertexAttribArray(BufferObjects::WORLDMAT + i);
						//tell opengl how to read it
						glVertexAttribPointer(BufferObjects::WORLDMAT + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
								(const void*)(sizeof(float) * i * 4));

						/** This function is what makes it per-instance data rather than per vertex
						* The first parameter is the attribute channel as above (4,5,6,7)
						* The second parameter tells the rate at which the attribute advanced during the rendering
						* 1 means that this data is updated after 1 instance has been rendered
						* by default it's 0 which makes it per-vertex and if it's over 1 than more than 1 instances will use this data
						*/
						glVertexAttribDivisor(BufferObjects::WORLDMAT + i, 1);
				}
				// unbind the vao after the setup is done
				glBindVertexArray(0);
		}

		void Renderer2D::submit(std::weak_ptr<Entity> _entity)
		{
				std::weak_ptr<Sprite> sprite = _entity.lock()->getComponent<SpriteRenderer>().lock()->getSprite();
				std::weak_ptr<GLTexture2D> texture = sprite.lock()->getTexture();

				//The transform values of the sprite
				std::weak_ptr<Transform> transform = _entity.lock()->getComponent<Transform>();

				auto iter = m_entitiesMap.find(texture.lock()->getTextureID());

				//check if it's not in the map
				if (iter == m_entitiesMap.end())
				{
						InstanceData instance;
						m_entitiesMap.insert(std::make_pair(texture.lock()->getTextureID(), instance));
				}

				m_entitiesMap[texture.lock()->getTextureID()].worldmats.push_back(transform.lock()->worldTransform());
				m_entitiesMap[texture.lock()->getTextureID()].colors.push_back(sprite.lock()->getColor());
				m_entitiesMap[texture.lock()->getTextureID()].sizes.push_back(sprite.lock()->getSize());
		}

		void Renderer2D::flush()
		{
				std::weak_ptr<Camera> currentCam = Camera::getCurrent();

				m_shader.lock()->use();
				m_shader.lock()->uploadValue("projection", currentCam.lock()->getProjectionMatrix());
				m_shader.lock()->uploadValue("view", currentCam.lock()->getViewMatrix());
				/* Bind the VAO. This sets up the opengl state we need, including the
				vertex attribute pointers and it binds the VBO */
				glBindVertexArray(m_VAO);

				//glDepthMask(GL_FALSE);

				for (auto& it : m_entitiesMap)
				{
						InstanceData instances = it.second;
						GLuint texID = it.first;
						//bind the per-instance buffers
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::WORLDMAT]);
						//upload the data
						glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances.worldmats.size(), instances.worldmats.data(), GL_DYNAMIC_DRAW);

						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::COLOR]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * instances.colors.size(), instances.colors.data(), GL_DYNAMIC_DRAW);

						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::SIZE]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instances.sizes.size(), instances.sizes.data(), GL_DYNAMIC_DRAW);

						glBindTexture(GL_TEXTURE_2D, texID);

						glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instances.worldmats.size());
				}

				//glDepthMask(GL_TRUE);

				//unbind the vao
				glBindVertexArray(0);

				m_shader.lock()->unUse();
		}

		void Renderer2D::begin()
		{
				m_entitiesMap.clear();
		}

		void Renderer2D::end()
		{
				sortSprites();
		}

		void Renderer2D::dispose()
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

		void Renderer2D::sortSprites()
		{

		}
}
