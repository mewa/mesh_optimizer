#include "MeshDecimator.h"
#include <map>
#include "Graph.h"

using namespace mewa;
using namespace mewa::decimator;

MeshDecimator::MeshDecimator()
{
}


MeshDecimator::~MeshDecimator()
{
	if (mGraph)
		delete mGraph;
}

void MeshDecimator::constructGraph(Mesh* mesh) {
	if (mGraph) {
		delete mGraph;
		mGraph = NULL;
	}
	mGraph = new Graph(mesh);
}

Mesh* MeshDecimator::decimate(Mesh* mesh) {
	assert(mesh->indices().size() % 3 == 0);
	constructGraph(mesh);
	mGraph->collapse();
	//auto decimatedMesh = new Mesh(mGraph->vertices(), mGraph->indices(), mesh->material());
	//assert(decimatedMesh->indices().size() % 3 == 0);
	return mGraph->mesh(mesh->material());

	//float xMin = mesh.vertices()[0].Position.x;
	//float yMin = mesh.vertices()[0].Position.y;
	//float zMin = mesh.vertices()[0].Position.z;

	//float xMax = mesh.vertices()[0].Position.x;
	//float yMax = mesh.vertices()[0].Position.y;
	//float zMax = mesh.vertices()[0].Position.z;

	//std::map<Vertex, GLuint> idxMap;

	//for (auto it = mesh.vertices().begin() + 1; it != mesh.vertices().end(); ++it) {
	//	float x = it->Position.x;
	//	float y = it->Position.y;
	//	float z = it->Position.z;
	//	
	//	if (x < xMin)
	//		xMin = x;
	//	if (y < yMin)
	//		yMin = y;
	//	if (z < zMin)
	//		zMin = z;

	//	if (x > xMax)
	//		xMax = x;
	//	if (y > yMax)
	//		yMax = y;
	//	if (z > zMax)
	//		zMax = z;
	//}
	//glm::vec3 const vecMin(xMin, yMin, zMin);
	//glm::vec3 const vecMax(xMax, yMax, zMax);
	//glm::vec3 const diff = vecMax - vecMin;
	//float const xUnit = diff.x / 100.0f;
	//float const yUnit = diff.y / 100.0f;
	//float const zUnit = diff.z / 100.0f;

	//size_t const xSize = diff.x / xUnit + 1;
	//size_t const ySize = diff.y / yUnit + 1;
	//size_t const zSize = diff.z / zUnit + 1;

	//std::vector<std::vector<std::vector<std::vector<Vertex> > > > buckets(
	//	xSize, std::vector<std::vector<std::vector<Vertex> > > //1d
	//	(ySize, std::vector<std::vector<Vertex> > //2d
	//	(zSize) //3d
	//	));
	//std::vector<std::vector<std::vector<std::vector<GLuint> > > > idxBuckets(
	//	xSize, std::vector<std::vector<std::vector<GLuint> > > //1d
	//	(ySize, std::vector<std::vector<GLuint> > //2d
	//	(zSize) //3d
	//	));
	//for (auto it = mesh.vertices().begin(); it != mesh.vertices().end(); ++it) {
	//	size_t ix = (it->Position.x - vecMin.x) / xUnit;
	//	size_t iy = (it->Position.y - vecMin.y) / yUnit;
	//	size_t iz = (it->Position.z - vecMin.z) / zUnit;
	//	buckets[ix][iy][iz].push_back(*it);
	//}
	//for (int i = 0; i < xSize; ++i) {
	//	for (int j = 0; j < ySize; ++j) {
	//		for (int k = 0; k < zSize; ++k) {
	//			
	//		}
	//	}
	//}
	//return mesh;
}

DecimationOperator::~DecimationOperator() {}
