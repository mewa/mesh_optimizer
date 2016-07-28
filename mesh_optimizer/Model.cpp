#include "Model.h"
#include <assimp/postprocess.h> 
#include <assimp/Importer.hpp>
#include <iostream>
#include <string>
#include "glconf.h"

using namespace mewa;

void checkError() {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "Error " << error << std::endl << glewGetErrorString(error) << std::endl;
	}
}

Model::Model(std::string const& fname)
{
	mName = fname;
	std::cout << "Loading " << mName << std::endl;
	std::string err;

	loadAsset(fname);

	mModelMatrix = glm::mat4(1.0f);
	std::cout << "Finished loading " << mName << std::endl;
}

float vertices[] = {
	0.0f, 0.5f, -1.0f, // Vertex 1 (X, Y)
	0.5f, -0.5f, -1.0f, // Vertex 2 (X, Y)
	-0.5f, -0.5f, -1.0f // Vertex 3 (X, Y)
};

void Model::loadAsset(std::string const& fname) {
	Assimp::Importer importer;

	aiScene const* scene = importer.ReadFile(fname, aiProcess_GenNormals
		| aiProcess_Triangulate);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	aiNode * rootNode = scene->mRootNode;
	processNode(rootNode, scene);

	mMaterials.reserve(scene->mNumMaterials);
	for (int i = 0; i < scene->mNumMaterials; ++i) {
		aiMaterial const* aiMat = scene->mMaterials[i];
		aiColor3D ambient;
		aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		aiColor3D diffuse;
		aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aiColor3D specular;
		aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		float shininess;
		aiMat->Get(AI_MATKEY_SHININESS, shininess);

		mMaterials.push_back(Material(
			glm::vec3(ambient.r, ambient.g, ambient.b),
			glm::vec3(diffuse.r, diffuse.g, diffuse.b),
			glm::vec3(specular.r, specular.g, specular.b),
			shininess));
	}
}

void Model::processNode(aiNode * node, aiScene const* scene) {
	for (int i = 0; i < node->mNumMeshes; ++i) {
		processMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	for (int i = 0; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh const* aiMesh, aiScene const* scene) {
	std::vector<Vertex> vertices(aiMesh->mNumVertices);
	for (int i = 0; i < aiMesh->mNumVertices; ++i) {
		Vertex vertex;
		vertex.Position = glm::vec3(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);
		if (aiMesh->HasNormals()) {
			vertex.Normal = glm::vec3(aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z);
		}
		vertices[i] = vertex;
	}
	std::vector<GLuint> indices(aiMesh->mNumVertices);
	for (int i = 0; i < aiMesh->mNumFaces; ++i) {
		for (int j = 0; j < aiMesh->mFaces[i].mNumIndices; ++j) {
			indices.push_back(aiMesh->mFaces[i].mIndices[j]);
		}
	}
	mMeshes.push_back(Mesh(std::move(vertices), std::move(indices), aiMesh->mMaterialIndex));
}

void Model::useShader(GLuint shader) const {
	GLint curr;
	glGetIntegerv(GL_CURRENT_PROGRAM, &curr);
	if (curr != shader)
		glUseProgram(shader);
}

void Model::draw(GLuint shader) const {
	useShader(shader);
	for (int i = 0; i < mMeshes.size(); ++i) {
		auto mesh = mMeshes[i];
		auto material = mMaterials[mesh.material()];
		GLint glMLocation = glGetUniformLocation(shader, "M");

		glUniformMatrix4fv(glMLocation, 1, false, glm::value_ptr(mModelMatrix));

		GLint ambientLocation = glGetUniformLocation(shader, "material.ambient");
		GLint diffuseLocation = glGetUniformLocation(shader, "material.diffuse");
		GLint specularLocation = glGetUniformLocation(shader, "material.specular");
		GLint shininessLocation = glGetUniformLocation(shader, "material.shininess");

		glUniform3fv(ambientLocation, 1, glm::value_ptr(material.ambient()));
		glUniform3fv(diffuseLocation, 1, glm::value_ptr(material.diffuse()));
		glUniform3fv(specularLocation, 1, glm::value_ptr(material.specular()));
		glUniform1f(shininessLocation, material.shininess());

		mesh.draw(shader);
	}
	glBindVertexArray(0);
}

Model::~Model()
{
}
