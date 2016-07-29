#include "EdgeCollapseOperator.h"

using namespace mewa;
using namespace mewa::decimator;

EdgeCollapseOperator::EdgeCollapseOperator()
{
}


EdgeCollapseOperator::~EdgeCollapseOperator()
{
}

std::vector<Vertex> EdgeCollapseOperator::decimate(std::vector<Vertex> const& region, std::vector<size_t> const& regionIndices, Graph* graph) {
	std::vector<Vertex> vertices;
	vertices.push_back(region[0]);
	return vertices;
}
