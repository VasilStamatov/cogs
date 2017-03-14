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
				glVertexAttribPointer(SPRITE_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

				unsigned int indices[] = { 0,1,2,			// first triangle (bottom left - top left - top right)
																															0,2,3 }; // second triangle (bottom left - top right - bottom right)

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[BufferObjects::INDEX]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

				// bind the color buffer
				glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::INSTANCED_ATTRIBS]);

				glEnableVertexAttribArray(SPRITE_COLOR_ATTRIBUTE);
				glVertexAttribPointer(SPRITE_COLOR_ATTRIBUTE, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstancedAttributes),
						(const GLvoid*)offsetof(InstancedAttributes, InstancedAttributes::color));
				glVertexAttribDivisor(SPRITE_COLOR_ATTRIBUTE, 1);

				glEnableVertexAttribArray(SPRITE_SIZE_ATTRIBUTE);
				glVertexAttribPointer(SPRITE_SIZE_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, sizeof(InstancedAttributes),
						(const GLvoid*)offsetof(InstancedAttributes, InstancedAttributes::size));
				glVertexAttribDivisor(SPRITE_SIZE_ATTRIBUTE, 1);

				// cannot upload mat4's all at once, so upload them as 4 vec4's
				for (size_t i = 0; i < 4; i++)
				{
						//enable the channel of the current matrix row (4,5,6,7)
						glEnableVertexAttribArray(SPRITE_WORLDMAT_ATTRIBUTE + i);
						//tell opengl how to read it
						glVertexAttribPointer(SPRITE_WORLDMAT_ATTRIBUTE + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstancedAttributes),
								(const GLvoid*)offsetof(InstancedAttributes, InstancedAttributes::worldMat[i]));
								//(const void*)(sizeof(float) * i * 4));

						/** This function is what makes it per-instance data rather than per vertex
						* The first parameter is the attribute channel as above (4,5,6,7)
						* The second parameter tells the rate at which the attribute advanced during the rendering
						* 1 means that this data is updated after 1 instance has been rendered
						* by default it's 0 which makes it per-vertex and if it's over 1 than more than 1 instances will use this data
						*/
						glVertexAttribDivisor(SPRITE_WORLDMAT_ATTRIBUTE + i, 1);
				}
				glBufferData(GL_ARRAY_BUFFER, sizeof(InstancedAttributes) * SPRITE_MAX_INSTANCES, nullptr, GL_STREAM_DRAW);

				// unbind the vao after the setup is done
				glBindVertexArray(0);
		}

		void Renderer2D::submit(std::weak_ptr<Entity> _entity)
		{
				std::weak_ptr<Sprite> sprite = _entity.lock()->getComponent<SpriteRenderer>().lock()->getSprite();
				std::weak_ptr<GLTexture2D> texture = sprite.lock()->getTexture();

				//The transform values of the sprite
				std::weak_ptr<Transform> transform = _entity.lock()->getComponent<Transform>();

				auto iter = m_spritesMap.find(texture.lock()->getTextureID());

				//check if it's not in the map
				if (iter == m_spritesMap.end())
				{
						std::vector<InstancedAttributes> newInstances;
						m_spritesMap.insert(std::make_pair(texture.lock()->getTextureID(), newInstances));
				}
				InstancedAttributes newInstance;
				newInstance.worldMat = transform.lock()->worldTransform();
				newInstance.color = sprite.lock()->getColor();
				newInstance.size = sprite.lock()->getSize();

				m_spritesMap[texture.lock()->getTextureID()].push_back(newInstance);
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

				for (auto& it : m_spritesMap)
				{
						std::vector<InstancedAttributes> instances = it.second;
						GLuint texID = it.first;
						//bind the per-instance buffers
						glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[BufferObjects::INSTANCED_ATTRIBS]);
						//upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstancedAttributes) * instances.size(), instances.data());

						glBindTexture(GL_TEXTURE_2D, texID);

						glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instances.size());
				}

				//glDepthMask(GL_TRUE);

				//unbind the vao
				glBindVertexArray(0);

				m_shader.lock()->unUse();
		}

		void Renderer2D::begin()
		{
				m_spritesMap.clear();
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
