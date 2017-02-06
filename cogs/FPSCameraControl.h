#ifndef FPS_CAMERA_CONTROL_H
#define	FPS_CAMERA_CONTROL_H

#include "Transform.h"

namespace cogs
{
		namespace ecs
		{
				/**
				* A component used to give the camera a fps-styled controls (wasd movement and rotation with the mouse)
				*/
				class FPSCameraControl : public Component
				{
				public:
						FPSCameraControl(float _moveSpeed);
						~FPSCameraControl();

						/**
						* The initialize component function
						*/
						void init() override;

						/**
						* The update component function
						*/
						void update(float _deltaTime) override;

				private:
						std::weak_ptr<Transform> m_transform;
						float m_moveSpeed{ 1.0f };
				};
		}
}
#endif // !FPS_CAMERA_CONTROL_H
