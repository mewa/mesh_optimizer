#pragma once

#include "SceneObject.h"
#include <vector>
#include <memory>

namespace mewa {
	class Scene
	{
		std::vector<std::unique_ptr<SceneObject const>> mObjects;
	public:
		Scene(GLFWwindow* window);
		virtual ~Scene();

		void addObject(SceneObject const* obj);
		void draw();
	};
}

