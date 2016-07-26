#define _USE_MATH_DEFINES
#include "Scene.h"
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <functional>

using namespace mewa;

float prevX, prevY, theta, phi;
float sensitivity = 10;

void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (action == GLFW_REPEAT) {
		if (key == GLFW_KEY_KP_ADD) {
			--sensitivity;
			if (sensitivity <= 0) {
				sensitivity = 1;
			}
			std::cout << action << "sensitivity: " << 40 - sensitivity << std::endl;
		}
		if (key == GLFW_KEY_KP_SUBTRACT) {
			++sensitivity;
			if (sensitivity > 40) {
				sensitivity = 40;
			}
			std::cout << "sensitivity: " << 40 - sensitivity << std::endl;
		}
	}
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) exit(0);
	}


	if (action == GLFW_RELEASE) {

	}
}

void mouse_click(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (button = GLFW_MOUSE_BUTTON_2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}


Scene::Scene(GLFWwindow* window)
{
	using namespace std::placeholders;
	Event::startListening(window);
	Event::registerEventListener(Event::Type::MouseMoved, [this](Event const* ev) {
		auto event = static_cast<MouseEvent const*>(ev);
		std::cout << "(x, y) = (" << event->x << ", " << event->y << ")" << std::endl;
		mCamera->update(event->x, event->y);
	});
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_click);
}

void Scene::addObject(SceneObject const* obj) {
	mObjects.push_back(std::unique_ptr<SceneObject const>(obj));
}

void Scene::registerCamera(cam::Camera* camera) {
	mCamera = camera;
}

void Scene::draw() {
	for (int i = 0; i < mObjects.size(); ++i) {
		mObjects[i].get()->draw(mCamera);
	}
}

Scene::~Scene()
{
	delete mCamera;
}
