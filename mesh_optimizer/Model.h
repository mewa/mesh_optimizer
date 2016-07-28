#pragma once

#include "glconf.h"
#include <assimp/scene.h>
#include "ShaderProgram.h"
#include "Camera.h"
#include "Drawable.h"
#include "Mesh.h"
#include "Material.h"
#include <string>
#include <vector>
#include <map>

void checkError();

namespace mewa {
	class Model
		: public Drawable
	{
		std::string mName;
		glm::mat4 mModelMatrix;
		std::vector<Mesh> mMeshes;
		std::vector<Material> mMaterials;

		void useShader(GLuint shader) const;

		void loadAsset(std::string const& fname);
		void processNode(aiNode * node, aiScene const* scene);
		void processMesh(aiMesh const* mesh, aiScene const* scene);
	public:
		Model(std::string const& fname);
		virtual void draw(GLuint shader) const;
		virtual ~Model();
	};
}

