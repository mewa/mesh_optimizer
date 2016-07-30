#include "Graph.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

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
	if (result < vertices.end()) {
		vertices.erase(result);
	}
	return vertices;
}

std::string Graph::collapse() {
	std::cout << "Reducing mesh with " << mVertices.size() << " vertices and "
		<< mTriangles.size() << " triangles" << std::endl;
	for (auto it = mTriangles.begin(); it != mTriangles.end();) {
		auto a = (*it)->a;
		auto b = (*it)->b;
		auto c = (*it)->c;
		std::vector<Triangle*>::const_iterator oldIt = it;
		bool collapsed = collapse(a, b, it);
		collapsed = collapse(a, c, it) || collapsed;
		collapsed = collapse(b, c, it) || collapsed;
		if (!collapsed)
			++it;
	}
	std::cout << "Decimated mesh with " << mVertices.size() << " vertices and "
		<< mTriangles.size() << " triangles" << std::endl;
	return writeMesh();
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

bool Graph::collapse(Vertex const* a, Vertex const* b, std::vector<Triangle*>::iterator& iterator) {
	auto incidentA = getIncidentVertices(a);
	auto incidentB = getIncidentVertices(b);
	std::vector<Vertex const*> intersection = intersect(incidentA, incidentB);

	std::set<Vertex const*> affectedVertices;
	affectedVertices.insert(incidentA.begin(), incidentA.end());
	affectedVertices.insert(incidentB.begin(), incidentB.end());

	if (intersection.size() != 2)
		return false;

	Vertex* collapsed = new Vertex();
	collapsed->Position = (a->Position + b->Position) / 2.0f;

	std::set<Triangle*> triangles;
	auto trianglesA = getIncidentTriangles(a);
	auto trianglesB = getIncidentTriangles(b);
	triangles.insert(trianglesA.begin(), trianglesA.end());
	triangles.insert(trianglesB.begin(), trianglesB.end());

	bool ret = false;

	for (auto it = triangles.begin(); it != triangles.end();) {
		int num = 0;
		Triangle triangle((*it)->a, (*it)->b, (*it)->c);
		auto normal = (*it)->normal;

		if (triangle.a == a || triangle.a == b) {
			triangle.a = collapsed;
			++num;
		}
		if (triangle.b == a || triangle.b == b) {
			triangle.b = collapsed;
			++num;
		}
		if (triangle.c == a || triangle.c == b) {
			triangle.c = collapsed;
			++num;
		}
		glm::vec3 ab = triangle.b->Position - triangle.a->Position;
		glm::vec3 ac = triangle.c->Position - triangle.a->Position;
		glm::vec3 bc = triangle.c->Position - triangle.b->Position;

		if (!glm::dot(normal, ab) && !glm::dot(normal, ac) && !glm::dot(normal, bc)) {
			(*it)->a = triangle.a;
			(*it)->b = triangle.b;
			(*it)->c = triangle.c;
			(*it)->calcNormal();

			if (num > 1) {
				removeTriangle(*it, affectedVertices, iterator);
				removeVertex(a);
				removeVertex(b);
				it = triangles.erase(it);
				ret = true;
			}
			else {
				++it;
			}
		}
		else {
			++it;
		}
	}

	return ret;
}

void Graph::removeTriangle(Triangle const* triangle, std::set<Vertex const*> const& affectedVertices, std::vector<Triangle*>::iterator& iterator) {
	auto pos = std::find(mTriangles.begin(), mTriangles.end(), triangle);
	iterator = mTriangles.erase(pos);
	for (auto it = affectedVertices.begin(); it != affectedVertices.end(); ++it) {
		pos = std::find(mIncidentTriangles[*it].begin(), mIncidentTriangles[*it].end(), triangle);
		if (pos < mIncidentTriangles[*it].end())
			mIncidentTriangles[*it].erase(pos);
	}
}

void Graph::removeVertex(Vertex const* vertex) {
	auto pos = std::find(mVertices.begin(), mVertices.end(), vertex);
	if (pos < mVertices.end()) {
		mVertices.erase(pos);
		mIncidentTriangles.erase(vertex);
	}
}

std::string Graph::writeMesh() {
	std::string fname;
	std::stringstream ss, nss;
	ss << "decimated_" << time(0) << ".obj";
	fname = ss.str();
	std::cout << "Writing decimated mesh to " << fname << std::endl;

	std::ofstream f(fname, std::ofstream::out);
	GLuint pos = 1;
	GLuint normPos = 1;
	std::map<Vertex const*, size_t> vertexPosMap;

	ss.str(std::string());
	for (auto it = mTriangles.begin(); it != mTriangles.end(); ++it) {
		// normals
		auto a = (*it)->a;
		auto b = (*it)->b;
		auto c = (*it)->c;

		auto incidentTriangles = mIncidentTriangles[(*it)->a];
		glm::vec3 norm(0);
		for (int i = 0; i < incidentTriangles.size(); ++i) {
			incidentTriangles[i]->calcNormal();
			norm += incidentTriangles[i]->normal;
		}
		norm = normalize(norm);
		nss << "vn\t"
			<< norm.x << "\t" << norm.y << "\t" << norm.z << std::endl;
		++normPos;

		incidentTriangles = mIncidentTriangles[(*it)->b];
		norm = glm::vec3(0);
		for (int i = 0; i < incidentTriangles.size(); ++i) {
			incidentTriangles[i]->calcNormal();
			norm += incidentTriangles[i]->normal;
		}
		norm = normalize(norm);
		nss << "vn\t"
			<< norm.x << "\t" << norm.y << "\t" << norm.z << std::endl;
		++normPos;

		incidentTriangles = mIncidentTriangles[(*it)->c];
		norm = glm::vec3(0);
		auto size = incidentTriangles.size();
		for (int i = 0; i < incidentTriangles.size(); ++i) {
			incidentTriangles[i]->calcNormal();
			norm += incidentTriangles[i]->normal;
		}
		norm = normalize(norm);
		nss << "vn\t"
			<< norm.x << "\t" << norm.y << "\t" << norm.z << std::endl;
		++normPos;

		// positions

		if (vertexPosMap.find((*it)->a) == vertexPosMap.end()) {
			vertexPosMap[(*it)->a] = pos++;
			f << "v\t"
				<< (*it)->a->Position.x << "\t"
				<< (*it)->a->Position.y << "\t"
				<< (*it)->a->Position.z << std::endl;
		}
		if (vertexPosMap.find((*it)->b) == vertexPosMap.end()) {
			vertexPosMap[(*it)->b] = pos++;
			f << "v\t"
				<< (*it)->b->Position.x << "\t"
				<< (*it)->b->Position.y << "\t"
				<< (*it)->b->Position.z << std::endl;
		}
		if (vertexPosMap.find((*it)->c) == vertexPosMap.end()) {
			vertexPosMap[(*it)->c] = pos++;
			f << "v\t"
				<< (*it)->c->Position.x << "\t"
				<< (*it)->c->Position.y << "\t"
				<< (*it)->c->Position.z << std::endl;
		}
		size_t posA = vertexPosMap[(*it)->a];
		size_t posB = vertexPosMap[(*it)->b];
		size_t posC = vertexPosMap[(*it)->c];
		ss << "f\t"
			<< posA << "//" << normPos - 3 << "\t"
			<< posB << "//" << normPos - 2 << "\t"
			<< posC << "//" << normPos - 1 << std::endl;
	}
	std::string faces = ss.str();
	std::string normals = nss.str();
	f << normals << faces;

	f.close();
	return fname;
}

//std::vector<Vertex const> Graph::vertices() {
//	std::vector<Vertex const> vertices;
//	vertices.reserve(mVertices.size());
//	for (auto it = mVertices.begin(); it != mVertices.end(); ++it) {
//		vertices.push_back(**it);
//	}
//	return vertices;
//}
//std::vector<GLuint const> Graph::indices() {
//	std::vector<GLuint const> indices;
//	indices.reserve(mTriangles.size() * 3);
//	std::map<Vertex const*, size_t> vertexPosMap;
//
//	GLuint pos = 0;
//
//	for (auto it = mTriangles.begin(); it != mTriangles.end(); ++it) {
//		if (vertexPosMap.find((*it)->a) == vertexPosMap.end()) {
//			vertexPosMap[(*it)->a] = pos++;
//		}
//		if (vertexPosMap.find((*it)->b) == vertexPosMap.end()) {
//			vertexPosMap[(*it)->b] = pos++;
//		}
//		if (vertexPosMap.find((*it)->c) == vertexPosMap.end()) {
//			vertexPosMap[(*it)->c] = pos++;
//		}
//		size_t posA = vertexPosMap[(*it)->a];
//		size_t posB = vertexPosMap[(*it)->b];
//		size_t posC = vertexPosMap[(*it)->c];
//		indices.push_back(posA);
//		indices.push_back(posB);
//		indices.push_back(posC);
//	}
//	return indices;
//}