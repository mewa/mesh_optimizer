#pragma once
#include <vector>
#include <map>
#include <set>
#include "Mesh.h"
namespace mewa {
	struct Triangle {
		Vertex const* a;
		Vertex const* b;
		Vertex const* c;
		glm::vec3 normal;
		std::vector<Triangle>* neighbours;

		Triangle(Vertex const* a, Vertex const* b, Vertex const* c) {
			this->a = a;
			this->b = b;
			this->c = c;
		}

		bool operator==(Triangle const& rhs) {
			return a == rhs.a
				&& b == rhs.b
				&& c == rhs.c;
		}
	};

	class Graph
	{
		std::vector<Vertex const*> mVertices;
		std::vector<Triangle*> mTriangles;
		std::map<Vertex const*, std::vector<Triangle*>> mIncidentTriangles;

		std::vector<Triangle*> getIncidentTriangles(Vertex const* v);
		std::vector<Triangle*> getIncidentTriangles(Triangle const* v);
		std::vector<Vertex const*> getIncidentVertices(Vertex const* v);
		
		void pushVertex(std::vector<Vertex const*>& vertices, Vertex const* v);
		Vertex const* findVertex(std::vector<Vertex const*> const& vertices, Vertex const* v);
		std::vector<Vertex const*> intersect(std::vector<Vertex const*> const& a, std::vector<Vertex const*> const& b);

		void removeTriangle(Triangle const* triangle, std::set<Vertex const*> const& affectedVertices, std::vector<Triangle*>::iterator& iterator);
		void removeVertex(Vertex const* vertex);

		std::string writeMesh();
	public:
		Graph(Mesh const& mesh);
		virtual ~Graph();

		bool collapse(Vertex const* a, Vertex const* b, std::vector<Triangle*>::iterator& iterator);
		std::string collapse();

		//std::vector<Vertex const> vertices();
		//std::vector<GLuint const> indices();
	};
}

