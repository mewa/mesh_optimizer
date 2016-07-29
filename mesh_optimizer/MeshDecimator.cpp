#include "MeshDecimator.h"
#include "Graph.h"

using namespace mewa;
using namespace mewa::decimator;

MeshDecimator::MeshDecimator(DecimationOperator* op)
{
	mOperator = op;
}


MeshDecimator::~MeshDecimator()
{
	delete mOperator;
}

void MeshDecimator::constructGraph(Mesh const& mesh) {
	
}

Mesh MeshDecimator::decimate(Mesh const& mesh) {
	constructGraph(mesh);
	return mesh;
}

DecimationOperator::~DecimationOperator() {}
