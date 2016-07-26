#pragma once

#include "glconf.h"

namespace mewa {
	namespace cam {
		class Camera
		{
		protected:
			glm::mat4 mViewMatrix;
			glm::mat4 mProjectionMatrix;
		public:
			Camera();
			virtual ~Camera();
			virtual glm::mat4 const& view() const;
			virtual glm::mat4 const& projection() const;

			virtual void update(double x, double y) = 0;
		};
	}
}

