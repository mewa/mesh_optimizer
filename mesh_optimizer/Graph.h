#pragma once
#include <vector>

template <typename node_t>
class Graph
{
	std::vector<node_t> mNodes;
	std::vector<std::vector<bool> > mMatrix;
public:
	Graph(size_t numVertices);
	virtual ~Graph();

	void addNode(node_t const&);
	void linkNode(size_t const& posVA, size_t const& posVB);
	void removeNode(size_t const& pos);

	bool isLinked(size_t const& posVA, size_t const& posVB) const;
	std::vector<size_t> adjacentNodes(size_t const& pos) const;
};

#include "Graph.inl"