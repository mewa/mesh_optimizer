#include "EdgeCollapseOperator.h"

using namespace mewa;
using namespace mewa::decimator;

EdgeCollapseOperator::EdgeCollapseOperator()
{
}


EdgeCollapseOperator::~EdgeCollapseOperator()
{
}

std::vector<Vertex> EdgeCollapseOperator::decimate(std::vector<Vertex> const& region, Graph<Vertex>* graph) {
	return region;
}
