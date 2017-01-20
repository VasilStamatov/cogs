#include "BulletDebugRenderer.h"

namespace cogs
{
		namespace graphics
		{
				BulletDebugRenderer::BulletDebugRenderer()
				{
						m_shader.compileShaders("DebugShader", "Shaders/DebugShader.vert", "Shaders/DebugShader.frag");
						// Generate the buffers
						glGenVertexArrays(1, &m_vao);
						glGenBuffers(1, &m_vbo);
						glGenBuffers(1, &m_ibo);

						//bind the vertex array object
						glBindVertexArray(m_vao);

						//bind the vertex buffer object
						glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

						glEnableVertexAttribArray(DEBUG_POSITION_ATTRIBUTE_INDEX);
						glVertexAttribPointer(DEBUG_POSITION_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), 
								(const GLvoid*)offsetof(DebugVertex, DebugVertex::position)); 

						glEnableVertexAttribArray(DEBUG_COLOR_ATTRIBUTE_INDEX);
						glVertexAttribPointer(DEBUG_COLOR_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex),
								(const GLvoid*)offsetof(DebugVertex, DebugVertex::color));

						//bind the index buffer object
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

						//unbind the vao
						glBindVertexArray(0);
				}

				BulletDebugRenderer::~BulletDebugRenderer()
				{
						//Dispose of all the buffest if they have't been disposed already
						if (m_vao != 0)
						{
								glDeleteVertexArrays(1, &m_vao);
								m_vao = 0;
						}

						if (m_vbo != 0)
						{
								glDeleteBuffers(1, &m_vbo);
								m_vbo = 0;
						}

						if (m_ibo != 0)
						{
								glDeleteBuffers(1, &m_ibo);
								m_ibo = 0;
						}
				}

				void BulletDebugRenderer::drawLine(const btVector3 & _from, const btVector3 & _to, const btVector3 & _color)
				{
						int i = m_verts.size();
						//resize the verts to +2 in size
						m_verts.resize(m_verts.size() + 2);

						m_verts[i].position = _from;
						m_verts[i].color = _color;
						m_verts[i + 1].position = _to;
						m_verts[i + 1].color = _color;
						//push back the indices
						m_indices.push_back(i);
						m_indices.push_back(i + 1);
				}
				void BulletDebugRenderer::drawContactPoint(const btVector3 & _PointOnB, const btVector3 & _normalOnB, btScalar _distance, int _lifeTime, const btVector3 & _color)
				{
				}
				void BulletDebugRenderer::reportErrorWarning(const char * _warningString)
				{
						printf(_warningString);
				}
				void BulletDebugRenderer::draw3dText(const btVector3 & _location, const char * _textString)
				{
				}
				void BulletDebugRenderer::setDebugMode(int _debugMode)
				{
						m_debugDrawMode = DebugDrawModes(_debugMode);
				}
				void BulletDebugRenderer::end()
				{
						//bind the vbo
						glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
						// Orphan the buffer
						glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
						// Upload the data
						glBufferSubData(GL_ARRAY_BUFFER, 0, m_verts.size() * sizeof(DebugVertex), m_verts.data());
						//unbind the vbo
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						//bind the ibo
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
						// Orphan the buffer
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
						// Upload the data
						glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());
						//unbind the ibo
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						//set up the numelements to be the same as indices.size
						m_numElements = m_indices.size();
						//clear the inices and vertices vectors
						m_indices.clear();
						m_verts.clear();
				}
				void BulletDebugRenderer::render(const glm::mat4 & _view, const glm::mat4 & _projection, float _lineWidth)
				{
						//begin using the shader program
						m_shader.use();
						m_shader.uploadValue("projection", _projection);
						m_shader.uploadValue("view", _view);
						//set up the line width
						glLineWidth(_lineWidth);
						//bind the vertex array object
						glBindVertexArray(m_vao);
						//draw the elements
						glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);
						//unbind the vao
						glBindVertexArray(0);
						//stop using the shader program
						m_shader.unUse();
				}
		}
}