#pragma once
#include "Camera.h"

namespace mewa {
	namespace cam {
		class FirstPersonCamera :
			public Camera
		{
			float mLastTime;
			float mAngleX, mAngleY;
			float mPrevX, mPrevY;
			bool isFirst;
			GLFWwindow* mWindow;
			GLuint currentMode;

			void updateViewMatrix();
		public:
			static GLuint mode;
		
			FirstPersonCamera(GLFWwindow* window);
			virtual ~FirstPersonCamera();

			virtual void update(double x, double y);
			virtual void update();

			void up(float unit);
			void move(float unit);
			void strafe(float unit);
		};
	}
}
