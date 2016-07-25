#include "SceneObject.h"
#include <iostream>
#include <string>
#include "glconf.h"

using namespace mewa;

#define GLSL(version, shader)  "#version " #version "\n" #shader

void checkError() {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "Error " << error << std::endl << glewGetErrorString(error) << std::endl;
	}
}

SceneObject::SceneObject(std::string const& fname)
{
	mName = fname;
	std::cout << "Loading " << mName << std::endl;
	std::string err;
	bool ret = tinyobj::LoadObj(&mAttributes, &mShapes, &mMaterials, &err, fname.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}
	else {
		std::cout << "Materials: " << mMaterials.size() << std::endl;
		std::cout << "Shapes: " << mShapes.size() << std::endl;

		initDrawing();

	}
	std::cout << "Finished loading " << mName << std::endl;
}

float vertices[] = {
	0.0f, 0.5f, -1.0f, // Vertex 1 (X, Y)
	0.5f, -0.5f, -1.0f, // Vertex 2 (X, Y)
	-0.5f, -0.5f, -1.0f // Vertex 3 (X, Y)
};

void SceneObject::initDrawing() {
	int i = 0;
	mVAOs = new GLuint[mShapes.size()];

	makeShaderPrograms();

	glGenVertexArrays(mShapes.size(), mVAOs);

	for (auto it = mShapes.begin(); it != mShapes.end(); ++it, ++i) {
		auto mesh = it->mesh;
		std::cout << "VAO: " << mVAOs[i] << std::endl;
		std::cout << mName << ":\tLoading shape: " << it->name << std::endl;
		std::cout << mName << ":\tVertices: " << mAttributes.vertices.size() / 3 << std::endl;
		for (int i = 0; i < mAttributes.vertices.size() / 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				std::cout << mAttributes.vertices[i * 3 + j] << "\t";
			}
			std::cout << std::endl;
		}
		std::cout << mName << ":\tNormals: " << mAttributes.normals.size() / 3 << std::endl;
		std::cout << mName << ":\tIndices: " << mesh.indices.size() << std::endl;
		for (int i = 0; i < mesh.indices.size(); ++i) {
			std::cout << mesh.indices[i].vertex_index << ",\t";
		}

		glBindVertexArray(mVAOs[i]);

		GLuint buffers[2];
		glGenBuffers(2, buffers);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mAttributes.vertices.size(), mAttributes.vertices.data(), GL_STATIC_DRAW);

		if (mesh.indices.size() > 0) {
			GLuint* indices = new GLuint[mesh.indices.size()];
			for (int i = 0; i < mesh.indices.size(); ++i) {
				indices[i] = mesh.indices[i].vertex_index;
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.indices.size(), indices, GL_STATIC_DRAW);
		}

		for (int i = 0; i < mShaderPrograms.size(); ++i) {
			GLuint shader = mShaderPrograms[i]->get();
			glUseProgram(shader);

			GLint vertexLocation = glGetAttribLocation(shader, "vertex");
			std::cout << "vertexLocation: " << vertexLocation << std::endl;

			glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			checkError();
			glEnableVertexAttribArray(vertexLocation);
		}
	}

	glBindVertexArray(0);
}

void SceneObject::makeShaderPrograms() {
	GLchar const* vShaderSrc = GLSL(330,
		uniform mat4 M;
	uniform mat4 V;
	uniform mat4 P;
	in vec3 vertex;
	in vec3 normal;
	out vec4 position;
	void main(void) {
		position = P * V * M * vec4(vertex.xyz, 1.0);
		gl_Position = position;
	});

	GLchar const* fShaderSrc = GLSL(
		330,
		out vec4 color;
	in vec4 position;
	void main() {
		color = vec4(position.xyz, 1);
	}
	);

	mShaderPrograms.push_back(new ShaderProgram(vShaderSrc, fShaderSrc));
	int i = 0;
	for (auto it = mMaterials.begin(); it != mMaterials.end(); ++it, ++i) {
		std::cout << mName << ":\tLoading material: " << it->name << std::endl;

		mShaderPrograms.push_back(new ShaderProgram(vShaderSrc, fShaderSrc));

		std::cout << mName << ":\tShader program " << i << " created" << std::endl;
	}
	std::cout << mName << ":\tDefault shader program created. Programs total: " << mShaderPrograms.size() << std::endl;
}

void SceneObject::draw(glm::mat4 V, glm::mat4 P) const {
	for (int i = 0; i < mShapes.size(); ++i) {
		auto mesh = mShapes[i].mesh;

		for (auto it = mesh.material_ids.begin(); it != mesh.material_ids.end(); ++it) {
			GLuint shader;
			shader = mShaderPrograms[*it + 1]->get();

			glUseProgram(shader);
			glBindVertexArray(mVAOs[i]);

			GLint glMLocation = glGetUniformLocation(shader, "M");
			GLint glVLocation = glGetUniformLocation(shader, "V");
			GLint glPLocation = glGetUniformLocation(shader, "P");

			glUniformMatrix4fv(glMLocation, 1, false, glm::value_ptr(glm::mat4()));
			glUniformMatrix4fv(glVLocation, 1, false, glm::value_ptr(V));
			glUniformMatrix4fv(glPLocation, 1, false, glm::value_ptr(P));

			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, NULL);
			checkError();
		}
	}
	glBindVertexArray(0);
}

SceneObject::~SceneObject()
{
	delete[] mVAOs;
	for (int i = 0; i < mShaderPrograms.size(); ++i) {
		delete mShaderPrograms[i];
	}
}
