#pragma once

#include <glm/glm.hpp>

namespace mewa {
	class Drawable {
	public:
		virtual void draw(glm::mat4 V, glm::mat4 P) const = 0;
		virtual ~Drawable() = 0;
	};
}