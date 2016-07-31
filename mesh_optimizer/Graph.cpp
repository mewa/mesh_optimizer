#include "Graph.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace mewa;

void mewa::Triangle::calcNormal() {
	glm::vec3 ab = b->Position - a->Position;
	glm::vec3 ac = c->Position - a->Position;
	auto cross = glm::cross(ab, ac);
	assert(!(cross.x == 0 && cross.y == 0 && cross.z == 0));
	normal = glm::normalize(cross);
}

Graph::Graph(Mesh* mesh)
{
	for (std::vector<GLuint>::const_iterator it = mesh->indices().begin(); it != mesh->indices().end(); it++) {
		auto a = &mesh->vertices()[*(it)];
		auto foundA = findVertex(mVertices, a);
		if (foundA) a = foundA;

		auto b = &mesh->vertices()[*(++it)];
		auto foundB = findVertex(mVertices, b);
		if (foundB) b = foundB;

		auto c = &mesh->vertices()[*(++it)];
		auto foundC = findVertex(mVertices, c);
		if (foundC) c = foundC;

		Triangle* t = new Triangle(a, b, c);
		mTriangles.push_back(t);

		auto cross = glm::cross(b->Position - a->Position, c->Position - a->Position);
		assert(!(cross.x == 0 && cross.y == 0 && cross.z == 0));


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

Vertex* Graph::findVertex(std::vector<Vertex*> const& vertices, Vertex const* v) {
	Vertex* found = NULL;
	for (int i = 0; i < vertices.size(); ++i) {
		auto other = vertices[i];
		if (*v == *other) {
			found = other;
			break;
		}
	}
	return found;
}

void Graph::pushVertex(std::vector<Vertex*>& vertices, Vertex* v) {
	if (!findVertex(vertices, v)) {
		vertices.push_back(v);
	}
}

Graph::~Graph()
{
}

std::vector<Triangle*> Graph::getIncidentTriangles(Vertex* v) {
	return mIncidentTriangles[v];
}
std::vector<Triangle*> Graph::getIncidentTriangles(Triangle* v) {
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

std::vector<Vertex*> Graph::getIncidentVertices(Vertex* v) {
	auto triangles = getIncidentTriangles(v);
	std::vector<Vertex*> vertices;
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

std::vector<Vertex*> Graph::intersect(std::vector<Vertex*> const& a, std::vector<Vertex*> const& b) {
	std::vector<Vertex*> intersection;
	for (auto it = a.begin(); it != a.end(); ++it) {
		if (findVertex(b, *it)) {
			intersection.push_back(*it);
		}
	}
	return intersection;
}

bool Graph::allowModification(Triangle* original, Triangle* triangle) {
	glm::vec3 ab = triangle->b->Position - triangle->a->Position;
	glm::vec3 ac = triangle->c->Position - triangle->a->Position;
	glm::vec3 bc = triangle->c->Position - triangle->b->Position;

	glm::vec3 oAb = original->b->Position - original->a->Position;
	glm::vec3 oAc = original->c->Position - original->a->Position;
	glm::vec3 oBc = original->c->Position - original->b->Position;

	original->calcNormal();
	auto normal = original->normal;
	triangle->calcNormal();
	auto normal2 = triangle->normal;

	auto l1 = glm::length(ab);
	auto l2 = glm::length(ac);
	auto l3 = glm::length(bc);
	auto l4 = glm::length(oAb);
	auto l5 = glm::length(oAc);
	auto l6 = glm::length(oBc);
	//assert(l1 < 0.5 && l2 < 0.5 && l2 < 0.5 && l3 < 0.5 && l4 < 0.5 && l5 < 0.5 && l6 < 0.5);
	auto cross = glm::cross(normal, normal2);
	bool ret = acos(dot(normal, normal2)) < glm::radians(90.0f)
		&& cross.x != 0 && cross.y != 0 && cross.z != 0;
	return ret;
}

bool Graph::collapse(Vertex* a, Vertex* b, std::vector<Triangle*>::iterator& iterator) {
	auto incidentA = getIncidentVertices(a);
	auto incidentB = getIncidentVertices(b);
	std::vector<Vertex*> intersection = intersect(incidentA, incidentB);

	std::set<Vertex*> affectedVertices;
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

	std::vector<Triangle*> toErase;
	bool normalsValid = true;
	std::map<Triangle*, Triangle*> replacements;

	for (auto it = triangles.begin(); it != triangles.end(); ++it) {
		int num = 0;
		Triangle* triangle = new Triangle((*it)->a, (*it)->b, (*it)->c);
		triangle->calcNormal();
		assert(glm::dot(triangle->normal, a->Position) != 0);

		if (*triangle->a == *a || *triangle->a == *b) {
			triangle->a = collapsed;
			++num;
		}
		if (*triangle->b == *a || *triangle->b == *b) {
			triangle->b = collapsed;
			++num;
		}
		if (*triangle->c == *a || *triangle->c == *b) {
			triangle->c = collapsed;
			++num;
		}

		if (num == 2 || allowModification(*it, triangle)) {
			if (num != 2)
				replacements[*it] = triangle;

			if (num > 1) {
				toErase.push_back(*it);
			}
		}
		else {
			normalsValid = false;
			break;
		}
	}
	if (normalsValid && toErase.size() == 2) {
		for (auto it = replacements.begin(); it != replacements.end(); ++it) {
			it->first->a = it->second->a;
			it->first->b = it->second->b;
			it->first->c = it->second->c;
			it->first->calcNormal();
			delete it->second;
		}
		for (auto it = toErase.begin(); it != toErase.end(); ++it) {
			removeTriangle(*it, affectedVertices, iterator);
		}
		removeVertex(a);
		removeVertex(b);
		mVertices.push_back(collapsed);
		return true;
	}

	return false;
}

void Graph::removeTriangle(Triangle* triangle, std::set<Vertex*> const& affectedVertices, std::vector<Triangle*>::iterator& iterator) {
	auto pos = std::find(mTriangles.begin(), mTriangles.end(), triangle);
	iterator = mTriangles.erase(pos);
	for (auto it = affectedVertices.begin(); it != affectedVertices.end(); ++it) {
		pos = std::find(mIncidentTriangles[*it].begin(), mIncidentTriangles[*it].end(), triangle);
		if (pos < mIncidentTriangles[*it].end())
			mIncidentTriangles[*it].erase(pos);
	}
}

void Graph::removeVertex(Vertex* vertex) {
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

	assert(indices().size() % 3 == 0);

	f.close();
	return fname;
}

std::vector<Vertex> Graph::vertices() {
	std::vector<Vertex> vertices;
	vertices.reserve(mVertices.size());
	for (auto it = mVertices.begin(); it != mVertices.end(); ++it) {
		auto incidentTriangles = mIncidentTriangles[*it];
		glm::vec3 norm(0);
		for (int i = 0; i < incidentTriangles.size(); ++i) {
			incidentTriangles[i]->calcNormal();
			norm += incidentTriangles[i]->normal;
		}
		norm = normalize(norm);
		(*it)->Normal = glm::vec3(1, 1, 1);
		vertices.push_back(**it);
	}
	return vertices;
}
std::vector<GLuint> Graph::indices() {
	std::vector<GLuint> indices;
	indices.reserve(mTriangles.size() * 3);
	std::map<Vertex*, size_t> vertexPosMap;

	GLuint pos = 0;

	for (auto it = mTriangles.begin(); it != mTriangles.end(); ++it) {
		if (vertexPosMap.find((*it)->a) == vertexPosMap.end()) {
			vertexPosMap[(*it)->a] = pos++;
		}
		if (vertexPosMap.find((*it)->b) == vertexPosMap.end()) {
			vertexPosMap[(*it)->b] = pos++;
		}
		if (vertexPosMap.find((*it)->c) == vertexPosMap.end()) {
			vertexPosMap[(*it)->c] = pos++;
		}
		size_t posA = vertexPosMap[(*it)->a];
		size_t posB = vertexPosMap[(*it)->b];
		size_t posC = vertexPosMap[(*it)->c];

		auto a = (*it)->a->Position;
		auto b = (*it)->b->Position;
		auto c = (*it)->c->Position;

		indices.push_back(posA);
		indices.push_back(posB);
		indices.push_back(posC);
	}
	assert(indices.size() % 3 == 0);
	assert(pos <= mVertices.size());
	return indices;
}

Mesh* Graph::mesh(GLuint material) {
	std::vector<GLuint> indices;
	std::vector<Vertex> vertices;
	indices.reserve(mTriangles.size() * 3);
	std::map<Vertex*, size_t> vertexPosMap;

	GLuint pos = 0;

	for (auto it = mTriangles.begin(); it != mTriangles.end(); ++it) {
		(*it)->calcNormal();
		glm::vec3 normal = (*it)->normal;
		assert(!(normal.x == 0 && normal.y == 0 && normal.z == 0));
		if (1 || vertexPosMap.find((*it)->a) == vertexPosMap.end()) {
			vertexPosMap[(*it)->a] = pos++;
			(*it)->a->Normal = normal;
			vertices.push_back(*(*it)->a);
		}
		if (1 || vertexPosMap.find((*it)->b) == vertexPosMap.end()) {
			vertexPosMap[(*it)->b] = pos++;
			(*it)->b->Normal = normal;
			vertices.push_back(*(*it)->b);
		}
		if (1 || vertexPosMap.find((*it)->c) == vertexPosMap.end()) {
			vertexPosMap[(*it)->c] = pos++;
			(*it)->c->Normal = normal;
			vertices.push_back(*(*it)->c);
		}
		size_t posA = vertexPosMap[(*it)->a];
		size_t posB = vertexPosMap[(*it)->b];
		size_t posC = vertexPosMap[(*it)->c];

		auto a = (*it)->a->Position;
		auto b = (*it)->b->Position;
		auto c = (*it)->c->Position;

		indices.push_back(posA);
		indices.push_back(posB);
		indices.push_back(posC);
	}
	Mesh* mesh = new Mesh(vertices, indices, material);
	return mesh;
}