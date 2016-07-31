#pragma once
#include "glconf.h"

namespace mewa {
	/*class Vertex
	{
	public:
		Vertex();
		Vertex(Vertex const& rhs);
		virtual ~Vertex();
		Vertex& operator=(Vertex const& rhs);

		glm::vec3 Position;
		glm::vec3 Normal;

		bool operator==(Vertex const& rhs) const;
	};*/
	struct Vertex {
		Vertex();
		Vertex(Vertex const& rhs);
		Vertex& operator=(Vertex const& rhs);

		glm::vec3 Position;
		glm::vec3 Normal;

		bool operator==(Vertex const& rhs) const;
	};
}
