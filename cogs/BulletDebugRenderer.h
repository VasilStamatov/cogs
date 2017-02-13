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

				//the debug vertex struct ( a vertex has a position and a color)
				struct DebugVertex
				{
						btVector3 position;
						btVector3 color;
				};

				constexpr uint DEBUG_POSITION_ATTRIBUTE_INDEX = 0;
				constexpr uint DEBUG_COLOR_ATTRIBUTE_INDEX				= 1;

				class BulletDebugRenderer : public btIDebugDraw
				{
				public:
						BulletDebugRenderer();
						virtual ~BulletDebugRenderer();

						virtual void	drawLine(const btVector3 & _from,
																												 const btVector3 & _to,
																												 const btVector3 & _color) override;

						virtual void drawContactPoint(const btVector3 & _PointOnB,
																																				const btVector3 & _normalOnB,
																																				btScalar _distance,
																																				int _lifeTime,
																																				const btVector3 & _color) override;

						virtual void reportErrorWarning(const char * _warningString) override;
						virtual void draw3dText(const btVector3 & _location, const char * _textString) override;

						virtual void setDebugMode(int _debugMode) override;
						virtual int  getDebugMode() const override { return m_debugDrawMode; }

						//ends the rendering
						void end();
						//renders it
						void render(const glm::mat4& _view, const glm::mat4& _projection, float _lineWidth);

				private:
						DebugDrawModes m_debugDrawMode{ DebugDrawModes::DBG_NoDebug };

						VBO m_vbo{ 0 };
						IBO m_ibo{ 0 };
						VAO m_vao{ 0 };

						std::vector<DebugVertex> m_verts;
						std::vector<uint> m_indices;
						uint m_numElements{ 0 };

						std::weak_ptr<GLSLProgram> m_shader;
				};
		}
}

#endif // !BULLET_DEBUG_RENDERER_H
