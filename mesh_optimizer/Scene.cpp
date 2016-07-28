#define _USE_MATH_DEFINES
#include "Scene.h"
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <functional>
#include "PlainShaderProgram.h"

using namespace mewa;

float prevX, prevY, theta, phi;
float sensitivity = 10;

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

		mCamera->update(event->x, event->y);
	});
	glfwSetMouseButtonCallback(window, mouse_click);
	makeShaderProgram();
}

void Scene::makeShaderProgram() {
	mShader = new PlainShaderProgram();
}

void Scene::addObject(Model* obj) {
	mObjects.push_back(std::unique_ptr<Model>(obj));
}

void Scene::registerCamera(cam::Camera* camera) {
	mCamera = camera;
}

void Scene::draw() {
	GLint glVLocation = glGetUniformLocation(mShader->get(), "V");
	GLint glPLocation = glGetUniformLocation(mShader->get(), "P");

	GLint viewPosLocation = glGetUniformLocation(mShader->get(), "viewPos");

	glUniformMatrix4fv(glVLocation, 1, false, glm::value_ptr(mCamera->view()));
	glUniformMatrix4fv(glPLocation, 1, false, glm::value_ptr(mCamera->projection()));

	glUniform3fv(viewPosLocation, 1, glm::value_ptr(mCamera->position()));

	for (int i = 0; i < mObjects.size(); ++i) {
		GLint lightColorLocation = glGetUniformLocation(mShader->get(), "light.ambient");
		GLint lightPositionLocation = glGetUniformLocation(mShader->get(), "light.position");
		GLint lightDiffuseLocation = glGetUniformLocation(mShader->get(), "light.diffuse");
		GLint lightSpecularLocation = glGetUniformLocation(mShader->get(), "light.diffuse");

		glUniform3f(lightColorLocation, 1, 1, 1);
		glUniform3f(lightPositionLocation, 3, 2, 5);
		glUniform3f(lightDiffuseLocation, 1, 1, 1);
		glUniform3f(lightSpecularLocation, 1, 1, 1);

		mCamera->update();
		mObjects[i].get()->draw(mShader->get());
	}
}

Scene::~Scene()
{
	delete mCamera;
}
