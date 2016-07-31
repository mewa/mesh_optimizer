#pragma once
#include <vector>
#include "glconf.h"
#include "Drawable.h"
#include "Vertex.h"

namespace mewa {
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
		Mesh(std::vector<Vertex> const& vertices, std::vector<GLuint> const& indices, unsigned materialIdx);
		virtual ~Mesh();

		unsigned material() const;
		size_t vertexCount() const;
		std::vector<Vertex>& vertices();
		std::vector<GLuint>& indices();

		virtual void draw(GLuint shader) const;
	};
}


