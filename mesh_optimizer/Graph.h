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
		std::vector<Vertex*> getIncidentVertices(Vertex const* v);
		void pushVertex(Vertex const* v);
		Vertex const* findVertex(Vertex const* v);
	public:
		Graph(Mesh const& mesh);
		virtual ~Graph();

		bool collapse(Vertex const* a, Vertex const* b);
		void collapse();
	};
}

