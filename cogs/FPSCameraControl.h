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
						FPSCameraControl();
						~FPSCameraControl();

						/**
						* The initialize component function
						*/
						void init() override;

						/**
						* The update component function
						*/
						void update(float _deltaTime) override;

						/**
						* The render component function
						*/
						void render() override;

				private:
						Transform* m_transform{ nullptr };
				};
		}
}
#endif // !FPS_CAMERA_CONTROL_H
