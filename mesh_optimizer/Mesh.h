#pragma once
#include <vector>
#include "glconf.h"
#include "Drawable.h"

namespace mewa {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
	};
	class Mesh
		: public Drawable
	{
		GLuint mVAO;
		unsigned mMaterialIndex;
		std::vector<Vertex> mVertices;
		std::vector<GLuint> mIndices;
		void init();
	public:
		Mesh(std::vector<Vertex> const&& vertices, std::vector<GLuint> const&& indices, unsigned materialIdx);
		virtual ~Mesh();

		unsigned material();

		virtual void draw(GLuint shader) const;
	};
}

