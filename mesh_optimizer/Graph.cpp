#include "Graph.h"
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace mewa;

Graph::Graph(Mesh const& mesh)
{
	for (std::vector<GLuint>::const_iterator it = mesh.indices().begin(); it != mesh.indices().end(); it++) {
		auto a = &mesh.vertices()[*(it)];
		auto foundA = findVertex(mVertices, a);
		if (foundA) a = foundA;

		auto b = &mesh.vertices()[*(++it)];
		auto foundB = findVertex(mVertices, b);
		if (foundB) b = foundB;

		auto c = &mesh.vertices()[*(++it)];
		auto foundC = findVertex(mVertices, c);
		if (foundC) c = foundC;

		Triangle* t = new Triangle(a, b, c);
		mTriangles.push_back(t);


		pushVertex(mVertices, a);
		pushVertex(mVertices, b);
		pushVertex(mVertices, c);

		mIncidentTriangles[a].push_back(t);
		mIncidentTriangles[b].push_back(t);
		mIncidentTriangles[c].push_back(t);
	}
	//for (int i = 0; i < mTriangles.size(); ++i) {
	//	mTriangles[i].neighbours.push_back()
	//}
}

Vertex const* Graph::findVertex(std::vector<Vertex const*> const& vertices, Vertex const* v) {
	Vertex const* found = NULL;
	for (int i = 0; i < vertices.size(); ++i) {
		auto other = vertices[i];
		if (*v == *other) {
			found = other;
			break;
		}
	}
	return found;
}

void Graph::pushVertex(std::vector<Vertex const*>& vertices, Vertex const* v) {
	if (!findVertex(vertices, v)) {
		vertices.push_back(v);
	}
}

Graph::~Graph()
{
}

std::vector<Triangle*> Graph::getIncidentTriangles(Vertex const* v) {
	return mIncidentTriangles[v];
}
std::vector<Triangle*> Graph::getIncidentTriangles(Triangle const* v) {
	std::vector<Triangle*> vec;
	std::vector<Triangle*> triangles = getIncidentTriangles(v->a);
	vec.insert(vec.end(), triangles.begin(), triangles.end());
	triangles = getIncidentTriangles(v->b);
	vec.insert(vec.end(), triangles.begin(), triangles.end());
	triangles = getIncidentTriangles(v->c);
	vec.insert(vec.end(), triangles.begin(), triangles.end());
	vec.erase(std::find(vec.begin(), vec.end(), v));
	return vec;
}

std::vector<Vertex const*> Graph::getIncidentVertices(Vertex const* v) {
	auto triangles = getIncidentTriangles(v);
	std::vector<Vertex const*> vertices;
	for (auto it = triangles.begin(); it != triangles.end(); ++it) {
		pushVertex(vertices, (*it)->a);
		pushVertex(vertices, (*it)->b);
		pushVertex(vertices, (*it)->c);
	}
	auto result = std::find(vertices.begin(), vertices.end(), v);
	if (1 || result < vertices.end()) {
		vertices.erase(result);
	}
	return vertices;
}

void Graph::collapse() {
	for (auto it = mTriangles.begin(); it != mTriangles.end(); ++it) {
		auto a = (*it)->a;
		auto b = (*it)->b;
		auto c = (*it)->c;
		bool collapsed = collapse(a, b) || collapse(a, c) || collapse(b, c);
	}
}

std::vector<Vertex const*> Graph::intersect(std::vector<Vertex const*> const& a, std::vector<Vertex const*> const& b) {
	std::vector<Vertex const*> intersection;
	for (auto it = a.begin(); it != a.end(); ++it) {
		if (findVertex(b, *it)) {
			intersection.push_back(*it);
		}
	}
	return intersection;
}

bool Graph::collapse(Vertex const* a, Vertex const* b) {
	auto incidentA = getIncidentVertices(a);
	auto incidentB = getIncidentVertices(b);
	std::vector<Vertex const*> intersection = intersect(incidentA, incidentB);

	if (intersection.size() != 2)
		return false;

	return true;
}
