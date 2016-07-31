#include "Vertex.h"

using namespace mewa;

Vertex::Vertex()
{
}

Vertex::Vertex(Vertex const& rhs) {
	*this = rhs;
}
//
//Vertex::~Vertex()
//{
//}

Vertex& Vertex::operator=(Vertex const& rhs) {
	Position = rhs.Position;
	Normal = rhs.Normal;
	return *this;
}

bool Vertex::operator==(Vertex const& rhs) const {
	return std::abs(this->Position.x - rhs.Position.x) < 0.000001
		&& std::abs(this->Position.y - rhs.Position.y) < 0.000001
		&& std::abs(this->Position.z - rhs.Position.z) < 0.000001;
}
