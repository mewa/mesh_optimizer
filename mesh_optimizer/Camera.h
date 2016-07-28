#pragma once

#include "glconf.h"

namespace mewa {
	namespace cam {
		class Camera
		{
		protected:
			glm::vec3 mPosition;
			glm::mat4 mViewMatrix;
			glm::mat4 mProjectionMatrix;
		public:
			Camera();
			virtual ~Camera();
			glm::mat4 const& view() const;
			glm::mat4 const& projection() const;
			glm::vec3 const& position() const;

			virtual void update(double x, double y) = 0;
			virtual void update() = 0;
		};
	}
}

