#pragma once
#include "Camera.h"

namespace mewa {
	namespace cam {
		class OrbitCamera
			: public Camera
		{
			float mTheta, mPhi;
			float mPrevX, mPrevY;
			float mRadius;
		public:
			OrbitCamera();
			virtual ~OrbitCamera();
			virtual void update(double x, double y);
			virtual void update();
		};
	}
}

