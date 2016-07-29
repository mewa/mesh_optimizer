#include "Graph.h"

template <typename node_t>
Graph<node_t>::Graph(size_t numVertices)
{
	mNodes.reserve(numVertices);
	mMatrix.resize(numVertices);
	for (int i = 0; i < numVertices; ++i) {
		mMatrix[i].resize(numVertices, false);
	}
}

template <typename node_t>
Graph<node_t>::~Graph()
{
}

template <typename node_t>
void Graph<node_t>::addNode(node_t const& node) {
	mNodes.push_back(node);
}

template <typename node_t>
void Graph<node_t>::linkNode(size_t const& posVA, size_t const& posVB) {
	mMatrix[posVA][posVB] = true;
	mMatrix[posVB][posVA] = true;
}

template <typename node_t>
void Graph<node_t>::removeNode(size_t const& pos) {
	for (int i = 0; i < mNodes.size(); ++i) {
		mMatrix[pos][i] = false;
		mMatrix[i][pos] = false;
	}
}

template <typename node_t>
bool Graph<node_t>::isLinked(size_t const& posVA, size_t const& posVB) const {
	return mMatrix[posVA][posVB];
}

template <typename node_t>
std::vector<size_t> Graph<node_t>::adjacentNodes(size_t const& pos) const {
	std::vector<size_t> adjacent;
	for (int i = 0; i < mNodes.size(); ++i) {
		if (mMatrix[pos][i])
			adjacent.push_back(i);
	}
	return adjacent;
}
