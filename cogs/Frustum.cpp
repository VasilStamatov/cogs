#include "Frustum.h"
#include "BulletDebugRenderer.h"

namespace cogs
{
		Frustum::Frustum()
		{
		}
		Frustum::~Frustum()
		{
		}
		void Frustum::setCamInternals(float _fov, float _aspectRatio, float _znear, float _zfar)
		{
				//Camera info
				m_fov = _fov;
				m_aspectRatio = _aspectRatio;
				m_znear = _znear;
				m_zfar = _zfar;

				//Calculate the width and height of the near and far planes
				float tang = tan(m_fov * 0.5f);
				m_nearHeight = m_znear * tang;
				m_nearWidth = m_nearHeight * m_aspectRatio;
				m_farHeight = m_zfar * tang;
				m_farWidth = m_farHeight * m_aspectRatio;
		}

		void Frustum::update(const glm::vec3& _position,
				const glm::vec3& _forward,
				const glm::vec3& _right,
				const glm::vec3& _up)
		{
				m_nearCenter = _position + (_forward * m_znear);
				m_farCenter = _position + (_forward * m_zfar);

				m_nearTopLeft = m_nearCenter - _right * m_nearWidth + _up * m_nearHeight;
				m_nearBottomLeft = m_nearCenter - _right * m_nearWidth - _up * m_nearHeight;
				m_nearTopRight = m_nearCenter + _right * m_nearWidth + _up * m_nearHeight;
				m_nearBottomRight = m_nearCenter + _right * m_nearWidth - _up * m_nearHeight;

				m_farTopLeft = m_farCenter - _right * m_farWidth + _up * m_farHeight;
				m_farBottomLeft = m_farCenter - _right * m_farWidth - _up * m_farHeight;
				m_farTopRight = m_farCenter + _right * m_farWidth + _up * m_farHeight;
				m_farBottomRight = m_farCenter + _right * m_farWidth - _up * m_farHeight;

				// compute the six planes
				// the function set3Points assumes that the points
				// are given in counter clockwise order
				m_planes[TOP].set3Points(m_nearTopRight, m_nearTopLeft, m_farTopLeft);
				m_planes[BOTTOM].set3Points(m_nearBottomLeft, m_nearBottomRight, m_farBottomRight);
				m_planes[LEFT].set3Points(m_nearTopLeft, m_nearBottomLeft, m_farBottomLeft);
				m_planes[RIGHT].set3Points(m_nearBottomRight, m_nearTopRight, m_farBottomRight);
				m_planes[NEAR].setNormalAndPoint(_forward, m_nearCenter);
				m_planes[FAR].setNormalAndPoint(-_forward, m_farCenter);

				////Define near plane
				//m_planes[NEAR].setNormalAndPoint(_forward, m_nearCenter);
				////Define far plane
				//m_planes[FAR].setNormalAndPoint(-_forward, m_farCenter);
				//glm::vec3 aux{ 0.0f }, normal{ 0.0f };
				////Define top plane
				//aux = (m_nearCenter + _up * m_nearHeight) - _position;
				//aux = glm::normalize(aux);
				//normal = glm::cross(aux, _right);
				//m_planes[TOP].setNormalAndPoint(normal, m_nearCenter + _up * m_nearHeight);
				////Define bottom plane
				//aux = (m_nearCenter - _up * m_nearHeight) - _position;
				//aux = glm::normalize(aux);
				//normal = glm::cross(_right, aux);
				//m_planes[BOTTOM].setNormalAndPoint(normal, m_nearCenter - _up * m_nearHeight);
				////Define left plane
				//aux = (m_nearCenter - _right * m_nearWidth) - _position;
				//aux = glm::normalize(aux);
				//normal = glm::cross(aux, _up);
				//m_planes[LEFT].setNormalAndPoint(normal, m_nearCenter - _right * m_nearWidth);
				////Define right plane
				//aux = (m_nearCenter + _right * m_nearWidth) - _position;
				//aux = glm::normalize(aux);
				//normal = glm::cross(_up, aux);
				//m_planes[RIGHT].setNormalAndPoint(normal, m_nearCenter + _right * m_nearWidth);
		}

		bool Frustum::pointInFrustum(const glm::vec3 & _pos) const
		{
				for (size_t i = 0; i < 6; i++)
				{
						if (m_planes[i].distance(_pos) < 0.0f)
						{
								return false;
						}
				}
				return true;
		}

		bool Frustum::sphereInFrustum(const glm::vec3 & _pos, float _radius) const
		{
				float distance{ 0.0f };
				for (size_t i = 0; i < 6; i++)
				{
						distance = m_planes[i].distance(_pos);
						if (distance + _radius <= 0.0f)
						{
								return false;
						}
				}
				return true;
		}
		void Frustum::render(BulletDebugRenderer * _renderer)
		{
				//submit near plane
				_renderer->drawLine(btVector3(m_nearTopLeft.x, m_nearTopLeft.y, m_nearTopLeft.z),
						btVector3(m_nearBottomLeft.x, m_nearBottomLeft.y, m_nearBottomLeft.z),
						btVector3(1.0f, 1.0f, 1.0f));

				_renderer->drawLine(btVector3(m_nearBottomLeft.x, m_nearBottomLeft.y, m_nearBottomLeft.z),
						btVector3(m_nearBottomRight.x, m_nearBottomRight.y, m_nearBottomRight.z),
						btVector3(1.0f, 1.0f, 1.0f));

				_renderer->drawLine(btVector3(m_nearBottomRight.x, m_nearBottomRight.y, m_nearBottomRight.z),
						btVector3(m_nearTopRight.x, m_nearTopRight.y, m_nearTopRight.z),
						btVector3(1.0f, 1.0f, 1.0f));

				_renderer->drawLine(btVector3(m_nearTopRight.x, m_nearTopRight.y, m_nearTopRight.z),
						btVector3(m_nearTopLeft.x, m_nearTopLeft.y, m_nearTopLeft.z),
						btVector3(1.0f, 1.0f, 1.0f));

				//submit far plane
				_renderer->drawLine(btVector3(m_farTopLeft.x, m_farTopLeft.y, m_farTopLeft.z),
						btVector3(m_farBottomLeft.x, m_farBottomLeft.y, m_farBottomLeft.z),
						btVector3(1.0f, 1.0f, 1.0f));

				_renderer->drawLine(btVector3(m_farBottomLeft.x, m_farBottomLeft.y, m_farBottomLeft.z),
						btVector3(m_farBottomRight.x, m_farBottomRight.y, m_farBottomRight.z),
						btVector3(1.0f, 1.0f, 1.0f));

				_renderer->drawLine(btVector3(m_farBottomRight.x, m_farBottomRight.y, m_farBottomRight.z),
						btVector3(m_farTopRight.x, m_farTopRight.y, m_farTopRight.z),
						btVector3(1.0f, 1.0f, 1.0f));

				_renderer->drawLine(btVector3(m_farTopRight.x, m_farTopRight.y, m_farTopRight.z),
						btVector3(m_farTopLeft.x, m_farTopLeft.y, m_farTopLeft.z),
						btVector3(1.0f, 1.0f, 1.0f));

				//submit near to far plane connections
				_renderer->drawLine(btVector3(m_nearTopLeft.x, m_nearTopLeft.y, m_nearTopLeft.z),
						btVector3(m_farTopLeft.x, m_farTopLeft.y, m_farTopLeft.z),
						btVector3(1.0f, 1.0f, 1.0f));

				_renderer->drawLine(btVector3(m_nearBottomLeft.x, m_nearBottomLeft.y, m_nearBottomLeft.z),
						btVector3(m_farBottomLeft.x, m_farBottomLeft.y, m_farBottomLeft.z),
						btVector3(1.0f, 1.0f, 1.0f));

				_renderer->drawLine(btVector3(m_nearTopRight.x, m_nearTopRight.y, m_nearTopRight.z),
						btVector3(m_farTopRight.x, m_farTopRight.y, m_farTopRight.z),
						btVector3(1.0f, 1.0f, 1.0f));

				_renderer->drawLine(btVector3(m_nearBottomRight.x, m_nearBottomRight.y, m_nearBottomRight.z),
						btVector3(m_farBottomRight.x, m_farBottomRight.y, m_farBottomRight.z),
						btVector3(1.0f, 1.0f, 1.0f));
		}

		void Frustum::Plane::setNormalAndPoint(const glm::vec3 & _normal, const glm::vec3 & _point)
		{
				normal = glm::normalize(_normal);
				d = -(glm::dot(normal, _point));
		}

		void Frustum::Plane::set3Points(const glm::vec3 & _v1, const glm::vec3 & _v2, const glm::vec3 & _v3)
		{
				glm::vec3 aux1{ 0.0f }, aux2{ 0.0f };

				aux1 = _v1 - _v2;
				aux2 = _v3 - _v2;

				normal = glm::cross(aux2, aux1);
				normal = glm::normalize(normal);

				d = -(glm::dot(normal, _v2));
		}

		void Frustum::Plane::setCoefficients(float _a, float _b, float _c, float _d)
		{
				normal = glm::vec3(_a, _b, _c);
				float l = glm::length(normal);
				normal = glm::normalize(normal);
				d = _d / l;
		}

		float Frustum::Plane::distance(const glm::vec3 & _p) const
		{
				return (glm::dot(normal, _p) + d);
		}
}