#define _USE_MATH_DEFINES
#include "Scene.h"
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>

using namespace mewa;

float speed_y, speed_x, angle_x, angle_y;

void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_y = -3.14;
		if (key == GLFW_KEY_RIGHT) speed_y = 3.14;
		if (key == GLFW_KEY_UP) speed_x = -3.14;
		if (key == GLFW_KEY_DOWN) speed_x = 3.14;
	}


	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_y = 0;
		if (key == GLFW_KEY_RIGHT) speed_y = 0;
		if (key == GLFW_KEY_UP) speed_x = 0;
		if (key == GLFW_KEY_DOWN) speed_x = 0;
	}
}

Scene::Scene(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
}

void Scene::addObject(SceneObject const* obj) {
	mObjects.push_back(std::unique_ptr<SceneObject const>(obj));
}

void Scene::draw() {
	angle_x += speed_x*glfwGetTime(); //Zwiêksz k¹t o prêdkoœæ k¹tow¹ razy czas jaki up³yn¹³ od poprzedniej klatki
	angle_y += speed_y*glfwGetTime(); //Zwiêksz k¹t o prêdkoœæ k¹tow¹ razy czas jaki up³yn¹³ od poprzedniej klatki
	glfwSetTime(0);

	glm::mat4 P = glm::perspective((float)(90.0 * M_PI / 180), 1.0f, 0.1f, 10.0f); //Wylicz macierz rzutowania
	P = glm::rotate(P, angle_y, glm::vec3(0, 1, 0));
	P = glm::rotate(P, angle_x, glm::vec3(1, 0, 0));
	glm::mat4 V = glm::lookAt( //Wylicz macierz widoku
		glm::vec3(2.5f, 3.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));
	for (int i = 0; i < mObjects.size(); ++i) {
		mObjects[i].get()->draw(V, P);
	}
}

Scene::~Scene()
{
}
