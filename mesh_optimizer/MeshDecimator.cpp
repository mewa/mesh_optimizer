#include "MeshDecimator.h"

using namespace mewa;
using namespace mewa::decimator;

MeshDecimator::MeshDecimator(DecimationOperator* op)
{
	mOperator = op;
}


MeshDecimator::~MeshDecimator()
{
	delete mOperator;
	if (mGraph)
		delete mGraph;
}

void MeshDecimator::constructGraph(Mesh const& mesh) {
	if (mGraph)
		delete mGraph;
	mGraph = new Graph<Vertex>(mesh.vertexCount());
	for (auto it = mesh.vertices().begin(); it != mesh.vertices().end(); ++it) {
		mGraph->addNode(*it);
	}
	for (auto it = mesh.indices().begin(); it != mesh.indices().end();) {
		size_t a = *it++;
		size_t b = *it++;
		size_t c = *it++;
		mGraph->linkNode(a, b);
		mGraph->linkNode(a, c);
		mGraph->linkNode(b, c);
	}
}

Mesh MeshDecimator::decimate(Mesh const& mesh) {
	constructGraph(mesh);
	std::vector<Vertex> vertices;
	for (int i = 0; i < mesh.vertexCount(); ++i) {
		auto adjacent = mGraph->adjacentNodes(i);
		if (adjacent.size() > 0) {
			std::vector<Vertex> selectedVertices;
			selectedVertices.reserve(adjacent.size());
			for (auto it = adjacent.begin(); it != adjacent.end(); ++it) {
				selectedVertices.push_back(mesh.vertices()[*it]);
			}
			auto vec = mOperator->decimate(selectedVertices, mGraph);
			vertices.insert(vertices.end(), vec.begin(), vec.end());
		}
	}
	return mesh;
}

DecimationOperator::~DecimationOperator() {}
