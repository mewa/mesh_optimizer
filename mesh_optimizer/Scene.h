#pragma once

#include "Model.h"
#include "Camera.h"
#include "Event.h"
#include "ShaderProgram.h"
#include <vector>
#include <memory>

namespace mewa {
	class Scene
	{
		std::vector<std::unique_ptr<Model>> mObjects;
		cam::Camera* mCamera;
		ShaderProgram* mShader;
		void mouse_pos_callback(Event* event);
	public:
		Scene(GLFWwindow* window);
		virtual ~Scene();

		void addObject(Model* obj);
		void registerCamera(cam::Camera* camera);
		void draw();

		void makeShaderProgram();
	};
}

