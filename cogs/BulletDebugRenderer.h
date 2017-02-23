#ifndef BULLET_DEBUG_RENDERER_H
#define BULLET_DEBUG_RENDERER_H

#include <Bullet\LinearMath\btIDebugDraw.h>
#include <vector>

#include "GLSLProgram.h"

namespace cogs
{
		namespace graphics
		{
				using VBO = uint;
				using VAO = uint;
				using IBO = uint;

				//the debug vertex struct (a vertex has a position and a color)
				struct DebugVertex
				{
						btVector3 position;
						btVector3 color;
				};

				constexpr uint DEBUG_POSITION_ATTRIBUTE_INDEX = 0;
				constexpr uint DEBUG_COLOR_ATTRIBUTE_INDEX				= 1;

				/**
				* \brief The renderer handling bullet physics debug rendering
				*/
				class BulletDebugRenderer : public btIDebugDraw
				{
				public:
						BulletDebugRenderer();
						virtual ~BulletDebugRenderer();

						void drawMeshDebug(std::weak_ptr<ecs::Entity> _entity);
						/**
						* \brief overriden function to submit line data
						*/
						virtual void	drawLine(const btVector3 & _from,
																												 const btVector3 & _to,
																												 const btVector3 & _color) override;

						/**
						* \brief overriden function to draw contact points
						*/
						virtual void drawContactPoint(const btVector3 & _PointOnB,
																																				const btVector3 & _normalOnB,
																																				btScalar _distance,
																																				int _lifeTime,
																																				const btVector3 & _color) override;
						/**
						* \brief overriden function to draw contact points
						*/
						virtual void reportErrorWarning(const char * _warningString) override;
						/**
						* \brief overriden function to draw 3D Text
						*/
						virtual void draw3dText(const btVector3 & _location, const char * _textString) override;
						/**
						* \brief overriden function to set the debug draw mode
						*/
						virtual void setDebugMode(int _debugMode) override;
						/**
						* \brief overriden function to get the debug draw mode
						*/
						virtual int  getDebugMode() const override { return m_debugDrawMode; }

						//ends the rendering
						void end();

						//flushes the vertices
						void render(const glm::mat4& _view, const glm::mat4& _projection, float _lineWidth);

				private:
						DebugDrawModes m_debugDrawMode{ DebugDrawModes::DBG_NoDebug }; ///< the current debug draw mode

						VBO m_vbo{ 0 }; ///< vertex buffer object of the DebugVertex
						IBO m_ibo{ 0 }; ///< index buffer object for indexed rendering
						VAO m_vao{ 0 }; ///< the vao to contain the buffer

						std::vector<DebugVertex> m_verts; ///< set of submitted vertices 
						std::vector<uint> m_indices; ///< set of the indices for indexed rendering
						uint m_numElements{ 0 }; ///< number of indices

						std::weak_ptr<GLSLProgram> m_shader; ///< reference to the shader used to render
				};
		}
}

#endif // !BULLET_DEBUG_RENDERER_H
