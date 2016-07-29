#pragma once

#include "Mesh.h"
#include <vector>
#include "Graph.h"

namespace mewa {
	namespace decimator {
		class DecimationOperator;

		class MeshDecimator
		{
			Graph<Vertex>* mGraph = NULL;
			DecimationOperator* mOperator = NULL;
		public:
			MeshDecimator(DecimationOperator* op);
			virtual ~MeshDecimator();
			virtual Mesh decimate(Mesh const& mesh);
			virtual void constructGraph(Mesh const& mesh);
		};

		class DecimationOperator {
		public:
			virtual ~DecimationOperator();
			virtual std::vector<Vertex> decimate(std::vector<Vertex> const& region, Graph<Vertex>* graph) = 0;
		};
	}
}
