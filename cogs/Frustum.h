#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

namespace cogs
{
		class BulletDebugRenderer;
		/**
		* \brief Defines a frustum, useful for culling and view detection
		*/
		class Frustum
		{
		public:
				Frustum();
				~Frustum();

				/**
				* \brief Sets internal camera properties. Needed for update()
				*/
				void setCamInternals(float _fov, float _aspectRatio, float _znear, float _zfar);

				/**
				* \brief Updates the frustum with the geometric information
				* \param[in] _position - the camera's world-space position
				* \param[in] _forward - the camera's world-space forward vector
				* \param[in] _forward - the camera's world-space right vector
				* \param[in] _up - the camera's world-space up vector
				*/
				void update(const glm::vec3& _position,
						const glm::vec3& _forward,
						const glm::vec3& _right,
						const glm::vec3& _up);

				/**
				* \brief Checks if a point is in the frustum
				* \param[in] _pos - The position of the point
				* \return true if it is in the frustum
				*/
				bool pointInFrustum(const glm::vec3& _pos) const;

				/**
				* \brief Checks if a sphere is in the frustum
				* \param[in] _pos - Center position of the sphere
				* \param[in] _radius - Radius of the sphere
				* \return true if it is in the frustum
				*/
				bool sphereInFrustum(const glm::vec3& _pos, float _radius) const;

				void render(BulletDebugRenderer* _renderer);

		private:
				enum Planes : unsigned int
				{
						RIGHT,
						LEFT,
						BOTTOM,
						TOP,
						FAR,
						NEAR
				};

				struct Plane
				{
						glm::vec3 normal;
						float d;

						void setNormalAndPoint(const glm::vec3& _normal, const glm::vec3& _point);
						void set3Points(const glm::vec3& _v1, const glm::vec3& _v2, const glm::vec3& _v3);
						void setCoefficients(float _a, float _b, float _c, float _d);
						float distance(const glm::vec3& _p) const;
				};

		private:
				float m_fov{ 0 }; ///< Vertical field of view in radians
				float m_aspectRatio{ 0.0f }; ///< Screen aspect ratio
				float m_znear{ 0.0f }; ///< Near clipping plane
				float m_zfar{ 0.0f }; ///< Far clipping plane
				float m_nearHeight{ 0.0f }; ///< Near plane height
				float m_nearWidth{ 0.0f }; ///< Near plane Width
				float m_farHeight{ 0.0f }; ///< Far plane height
				float m_farWidth{ 0.0f }; ///< Far plane Width

				glm::vec3 m_nearCenter;
				glm::vec3 m_farCenter;

				glm::vec3 m_nearTopLeft;
				glm::vec3 m_nearBottomLeft;
				glm::vec3 m_nearTopRight;
				glm::vec3 m_nearBottomRight;

				glm::vec3 m_farTopLeft;
				glm::vec3 m_farBottomLeft;
				glm::vec3 m_farTopRight;
				glm::vec3 m_farBottomRight;


				Plane m_planes[6] = {}; ///< The actual frustum data
		};
}

#endif // !FRUSTUM_H