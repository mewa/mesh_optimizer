#pragma once

#include <glm/glm.hpp>
#include "Camera.h"

namespace mewa {
	class Drawable {
	public:
		virtual void draw(cam::Camera* camera) const = 0;
		virtual ~Drawable() = 0;
	};
}