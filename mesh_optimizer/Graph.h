#pragma once
#include <vector>
#include <map>
#include <set>
#include "Mesh.h"
namespace mewa {
	struct Triangle {
		Vertex* a;
		Vertex* b;
		Vertex* c;
		glm::vec3 normal;
		std::vector<Triangle>* neighbours;

		Triangle(Vertex* a, Vertex* b, Vertex* c) {
			assert(!(a == b));
			assert(!(a == c));
			assert(!(b == c));
			this->a = a;
			this->b = b;
			this->c = c;
			calcNormal();
		}

		void calcNormal();

		bool operator==(Triangle const& rhs) {
			return a == rhs.a
				&& b == rhs.b
				&& c == rhs.c;
		}
	};

	class Graph
	{
		std::vector<Vertex*> mVertices;
		std::vector<Triangle*> mTriangles;
		std::map<Vertex*, std::vector<Triangle*>> mIncidentTriangles;

		std::vector<Triangle*> getIncidentTriangles(Vertex* v);
		std::vector<Triangle*> getIncidentTriangles(Triangle* v);
		std::vector<Vertex*> getIncidentVertices(Vertex* v);
		
		void pushVertex(std::vector<Vertex*>& vertices, Vertex* v);
		Vertex* findVertex(std::vector<Vertex*> const& vertices, Vertex const* v);
		std::vector<Vertex*> intersect(std::vector<Vertex*> const& a, std::vector<Vertex*> const& b);

		void removeTriangle(Triangle* triangle, std::set<Vertex*> const& affectedVertices, std::vector<Triangle*>::iterator& iterator);
		void removeVertex(Vertex* vertex);

		bool allowModification(Triangle* a, Triangle* b);

		std::string writeMesh();
	public:
		Graph(Mesh* mesh);
		virtual ~Graph();

		bool collapse(Vertex* a, Vertex* b, std::vector<Triangle*>::iterator& iterator);
		std::string collapse();

		Mesh* mesh(GLuint material);
		std::vector<Vertex> vertices();
		std::vector<GLuint> indices();
	};
}

