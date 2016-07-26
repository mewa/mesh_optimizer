#pragma once

#include "SceneObject.h"
#include "Camera.h"
#include "Event.h"
#include <vector>
#include <memory>

namespace mewa {
	class Scene
	{
		std::vector<std::unique_ptr<SceneObject const>> mObjects;
		cam::Camera* mCamera;
		void mouse_pos_callback(Event* event);
	public:
		Scene(GLFWwindow* window);
		virtual ~Scene();

		void addObject(SceneObject const* obj);
		void registerCamera(cam::Camera* camera);
		void draw();
	};
}

