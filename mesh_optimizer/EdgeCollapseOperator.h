#pragma once
#include "MeshDecimator.h"
namespace mewa {
	namespace decimator {
		class EdgeCollapseOperator :
			public DecimationOperator
		{
		public:
			EdgeCollapseOperator();
			virtual ~EdgeCollapseOperator();
			virtual std::vector<Vertex> decimate(std::vector<Vertex> const& region, std::vector<size_t> const& regionIndices, Graph* graph);
		};
	}
}
