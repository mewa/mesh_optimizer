#pragma once

#include <glm/glm.hpp>
#include "Camera.h"
#include "glconf.h"

namespace mewa {
	class Drawable {
	public:
		virtual void draw(GLuint shader) const = 0;
		virtual ~Drawable() = 0;
	};
}