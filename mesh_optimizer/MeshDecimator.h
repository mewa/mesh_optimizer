#pragma once

#include "Mesh.h"
#include <vector>
#include "Graph.h"

namespace mewa {
	namespace decimator {
		class DecimationOperator;

		class MeshDecimator
		{
			Graph* mGraph = NULL;
		public:
			MeshDecimator();
			virtual ~MeshDecimator();
			virtual Mesh decimate(Mesh const& mesh);
			virtual void constructGraph(Mesh const& mesh);
		};

		class DecimationOperator {
		public:
			virtual ~DecimationOperator();
			virtual std::vector<Vertex> decimate(std::vector<Vertex> const& region, std::vector<size_t> const& regionIndices, Graph* graph) = 0;
		};
	}
}
