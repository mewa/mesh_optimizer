#pragma once
#include <vector>
#include "glconf.h"
#include "Drawable.h"

namespace mewa {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;

		bool operator==(Vertex const& rhs) const {
			return std::abs(this->Position.x - rhs.Position.x) < FLT_EPSILON
				&& std::abs(this->Position.y - rhs.Position.y) < FLT_EPSILON
				&& std::abs(this->Position.z - rhs.Position.z) < FLT_EPSILON;
		}
	};
	class Mesh
		: public Drawable
	{
		GLuint mVAO;
		unsigned mMaterialIndex;
		std::vector<Vertex const> mVertices;
		std::vector<GLuint const> mIndices;
		void init();
	public:
		Mesh(std::vector<Vertex const> const&& vertices, std::vector<GLuint const> const&& indices, unsigned materialIdx);
		Mesh(std::vector<Vertex const> const& vertices, std::vector<GLuint const> const& indices, unsigned materialIdx);
		virtual ~Mesh();

		unsigned material() const;
		size_t vertexCount() const;
		std::vector<Vertex const> const& vertices() const;
		std::vector<GLuint const> const& indices() const;

		virtual void draw(GLuint shader) const;
	};
}


