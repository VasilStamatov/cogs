#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"

namespace cogs
{
		namespace ecs
		{
				enum class ProjectionType
				{
						ORTHOGRAPHIC,
						PERSPECTIVE
				};

				class CameraComponent : public Component
				{
				public:
						/**
								*	\brief Constructor for the camera component
								* \param _projType - the projection type of the camera
								* \param _screenWidth - screen width
								* \param _screenHeight - screen height
								*/
						CameraComponent(const ProjectionType& _projType = ProjectionType::ORTHOGRAPHIC,
								int _screenWidth = 1024, int _screenHeight = 576);
						~CameraComponent();

						virtual void init() override;
						virtual void update(float _deltaTime) override;
						virtual void render() override;

						/**
								*	\brief Sets the camera position to a specific one
								* \param _newPos - the new position vector
								*/
						void setPosition(const glm::vec3& _newPos) { m_transform->setPosition(_newPos); m_needsMatrixUpdate = true; }

						/**
								* \brief offsets the position of the camera
								* \param _offset - the offset which the position will add
								*/
						void translate(const glm::vec3& _offset) { m_transform->translate(_offset); m_needsMatrixUpdate = true; }

						/**
								* \brief Sets the camera orientation to a specific one
								* \param _eulerAngles - the euler angles to rotate
								*/
						void setRotation(const glm::vec3& _eulerAngles) { m_transform->setRotation(_eulerAngles); m_needsMatrixUpdate = true; }

						/**
								* \brief rotates the camera
								* \param _eulerAngles - the euler angles to rotate
								*/
						void rotate(const glm::vec3& _eulerAngles) { m_transform->rotate(_eulerAngles); m_needsMatrixUpdate = true; }

						/**
								* \brief rotates the camera to look at a certain point
								* \param _eulerAngles - the horizontal offset (degrees)
								*/
						void lookAt(const glm::vec3& _eulerAngles) { m_transform->lookAt(_eulerAngles); m_needsMatrixUpdate = true; }

						/**
								* \brief Changes the FoV of the perspective matrix and updates it
								* \param _value - the new FoV value
								*/
						void setFoV(int _value);

						/**
								* \brief Changes the size of the orthographic matrix and updates it
								* \param _value - the new size value
								*/
						void setSize(float _value);

						/**
						* \brief Changes the near plane
						* \param _value - the new near value
						*/
						void setNearPlane(float _value);

						/**
						* \brief Changes the far plane
						* \param _value - the new far value
						*/
						void setFarPlane(float _value);

						/**
								* \brief Gets the aspect ratio of the screen
								* \return the division of SW / SH
								*/
						float getAspectRatio() const noexcept { return (float)m_screenWidth / (float)m_screenHeight; }

						/**
								* \brief Sets the screen width and height for the camera and updates the proj matrix
								* \param _screenWidth - the new width
								* \param _screenHeight - the new height
								*/
						void resize(int _screenWidth, int _screenHeight);

						/**
								* \brief Gets the projection matrix of the camera
								* \return the projection matrix
								*/
						const glm::mat4& getProjectionMatrix() const noexcept;

						/**
								* \brief Gets the view matrix of the camera
								* \return the view matrix
								*/
						const glm::mat4& getViewMatrix() const noexcept { return m_viewMatrix; }

						/**
								* \brief Gets the local position vec3 of the camera
								* \return the position vec3
								*/
						glm::vec3 getPosition() const noexcept { return m_transform->getTransformedPos(); }

						/**
								* \brief Gets the local orientation quat of the camera
								* \return the orientation quat
								*/
						glm::quat getOrientation() const noexcept { return m_transform->getTransformedRot(); }

						/**
								* \brief Sets the projection type of the matrix
								*/
						void setProjectionType(const ProjectionType& _projType) { m_projType = _projType; }

				private:
						ProjectionType m_projType{ ProjectionType::ORTHOGRAPHIC }; ///< the projection type of the camera
						glm::mat4 m_orthoMatrix{ 1.0f }; ///< orthographic matrix for ortho camera
						glm::mat4 m_perspMatrix{ 1.0f }; ///< perspective matrix for perspective camera
						glm::mat4 m_viewMatrix{ 1.0f }; ///< Camera matrix
						ecs::Transform* m_transform{ nullptr }; ///< the transform of the camera
						float m_size{ 5.0f }; ///< the size of the ortho camera (zoom)
						int m_fov{ 60 }; ///< the field of view of the perspective camera

						bool m_needsMatrixUpdate{ true }; ///< flag which is true when the view matrix needs to be updated

						int m_screenWidth{ 1024 }; ///< The width of the sdl screen
						int m_screenHeight{ 576 }; ///< The height of the sdl screen

						float m_nearPlane{ 0.1f }; ///< the near clipping plane
						float m_farPlane{ 100.0f }; ///< the far clipping plane
				};
		}
}
#endif // !CAMERA_H