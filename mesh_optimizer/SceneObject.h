#pragma once

#include "tiny_obj_loader.h"
#include "glconf.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Drawable.h"
#include <string>
#include <vector>
#include <map>

void checkError();

namespace mewa {
	class SceneObject
		: public Drawable
	{
		std::string mName;
		tinyobj::attrib_t mAttributes;
		std::vector<tinyobj::shape_t> mShapes;
		GLuint* mVAOs;
		std::vector<ShaderProgram*> mShaderPrograms;
		std::vector<tinyobj::material_t> mMaterials;

		void makeShaderPrograms();
	public:
		SceneObject(std::string const& fname);
		virtual void draw(cam::Camera* camera) const;
		virtual ~SceneObject();
	private:
		virtual void initDrawing();
	};
}

