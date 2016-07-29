#pragma once

#include "Mesh.h"
#include <vector>

namespace mewa {
	namespace decimator {
		class DecimationOperator;

		class MeshDecimator
		{
			std::vector<bool> mValidity;
			DecimationOperator* mOperator;
		public:
			MeshDecimator(DecimationOperator* op);
			virtual ~MeshDecimator();
			virtual Mesh decimate(Mesh const& mesh);
			virtual void constructGraph(Mesh const& mesh);
		};

		class DecimationOperator {
		public:
			virtual ~DecimationOperator();
			virtual std::vector<Vertex> decimate(std::vector<Vertex> const& region) = 0;
		};
	}
}
