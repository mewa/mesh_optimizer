#include "Mesh.h"

using namespace mewa;

Mesh::Mesh(std::vector<Vertex> const&& vertices, std::vector<GLuint> const&& indices, unsigned materialIdx)
{
	mVertices = vertices;
	mIndices = indices;
	mMaterialIndex = materialIdx;
	init();
}

void Mesh::init() {
	glGenVertexArrays(1, &mVAO);

	glBindVertexArray(mVAO);

	GLuint buffers[2];
	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mIndices.size(), mIndices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Mesh::draw(GLuint shader) const {
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

size_t Mesh::vertexCount() const {
	return mVertices.size();
}

unsigned Mesh::material() {
	return mMaterialIndex;
}

Mesh::~Mesh()
{
}
