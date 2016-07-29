#include "Graph.h"
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace mewa;

Graph::Graph(Mesh const& mesh)
{
	for (std::vector<GLuint>::const_iterator it = mesh.indices().begin(); it != mesh.indices().end(); it++) {
		auto a = &mesh.vertices()[*(it)];
		auto foundA = findVertex(a);
		if (foundA) a = foundA;

		auto b = &mesh.vertices()[*(++it)];
		auto foundB = findVertex(b);
		if (foundB) b = foundB;

		auto c = &mesh.vertices()[*(++it)];
		auto foundC = findVertex(c);
		if (foundC) c = foundC;

		Triangle* t = new Triangle(a, b, c);
		mTriangles.push_back(t);


		pushVertex(a);
		pushVertex(b);
		pushVertex(c);

		mIncidentTriangles[a].push_back(t);
		mIncidentTriangles[b].push_back(t);
		mIncidentTriangles[c].push_back(t);
	}
	//for (int i = 0; i < mTriangles.size(); ++i) {
	//	mTriangles[i].neighbours.push_back()
	//}
}

Vertex const* Graph::findVertex(Vertex const* v) {
	Vertex const* found = NULL;
	for (int i = 0; i < mVertices.size(); ++i) {
		auto other = mVertices[i];
		if (*v == *other) {
			found = other;
			break;
		}
	}
	return found;
}

void Graph::pushVertex(Vertex const* v) {
	if (!findVertex(v)) {
		mVertices.push_back(v);
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
	vec.erase(std::find(vec.begin(), vec.end(),  v));
	return vec;
}

std::vector<Vertex*> Graph::getIncidentVertices(Vertex const* v) {
	auto triangles = getIncidentTriangles(v);
	std::vector<Vertex*> vertices;
	for (auto it = triangles.begin(); it != triangles.end(); ++it) {
		
	}
	return vertices;
}

void Graph::collapse() {
	for (auto it = mTriangles.begin(); it != mTriangles.end(); ++it) {
		//bool collapsed = collapse(it->a, it->b) || collapse(it->a, it->c) || collapse(it->b, it->c);
	}
}

bool Graph::collapse(Vertex const* a, Vertex const* b) {
	auto incidentA = getIncidentVertices(a);
	auto incidentB = getIncidentVertices(b);
	std::vector<Vertex*> intersection;
	std::set_intersection(incidentA.begin(), incidentA.end(), incidentB.begin(), incidentB.end(), 
		std::inserter(intersection, intersection.begin()));
	return true;
	intersection.erase(std::find(intersection.begin(), intersection.end(), a));
	intersection.erase(std::find(intersection.begin(), intersection.end(), b));

	if (intersection.size() != 2)
		return false;

	return true;
}
