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

Scene::Scene(GLFWwindow* window)
{
	mCamera = 0;
	mWindow = window;
	using namespace std::placeholders;
	Event::startListening(window);
	Event::registerEventListener(Event::Type::MouseMoved, [this](Event const* ev) {
		auto event = static_cast<MouseEvent const*>(ev);

		if (mCamera)
			mCamera->update(event->x, event->y);
	});
	makeShaderProgram();
}

void Scene::makeShaderProgram() {
	mShader = new PlainShaderProgram();
}

void Scene::addObject(Model* obj) {
	mObjects.push_back(std::unique_ptr<Model>(obj));
	size_t vertexCount = 0;
	for (auto it = mObjects.begin(); it != mObjects.end(); ++it) {
		vertexCount += it->get()->vertexCount();
	}
	char buf[64];
	sprintf(buf, "Vertices: %lu", vertexCount);
	glfwSetWindowTitle(mWindow, buf);
}

void Scene::removeObject(Model* obj) {
	auto pos = std::find_if(mObjects.begin(), mObjects.end(), [&](std::unique_ptr<Model>& ptr){
		return obj == ptr.get();
	});
	mObjects.erase(pos);
	size_t vertexCount = 0;
	for (auto it = mObjects.begin(); it != mObjects.end(); ++it) {
		vertexCount += it->get()->vertexCount();
	}
	char buf[64];
	sprintf(buf, "Vertices: %lu", vertexCount);
	glfwSetWindowTitle(mWindow, buf);
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
		GLint lightNumLocation = glGetUniformLocation(mShader->get(), "numLights");

		glUniform1i(lightNumLocation, 2);

		GLint lightColorLocation = glGetUniformLocation(mShader->get(), "lights[0].ambient");
		GLint lightPositionLocation = glGetUniformLocation(mShader->get(), "lights[0].position");
		GLint lightDiffuseLocation = glGetUniformLocation(mShader->get(), "lights[0].diffuse");
		GLint lightSpecularLocation = glGetUniformLocation(mShader->get(), "lights[0].specular");

		glUniform3f(lightColorLocation, 1, 1, 1);
		glUniform3f(lightPositionLocation, 3, 2, 5);
		glUniform3f(lightDiffuseLocation, 0.2, 0.2, 0.2);
		glUniform3f(lightSpecularLocation, 1, 1, 1);


		lightColorLocation = glGetUniformLocation(mShader->get(), "lights[1].ambient");
		lightPositionLocation = glGetUniformLocation(mShader->get(), "lights[1].position");
		lightDiffuseLocation = glGetUniformLocation(mShader->get(), "lights[1].diffuse");
		lightSpecularLocation = glGetUniformLocation(mShader->get(), "lights[1].specular");

		glUniform3f(lightColorLocation, 0, 0, 0);
		glUniform3fv(lightPositionLocation, 1, glm::value_ptr(mCamera->position()));
		glUniform3f(lightDiffuseLocation, 0.8, 0.8, 0.8);
		glUniform3f(lightSpecularLocation, 0, 0, 0);

		mCamera->update();
		mObjects[i].get()->draw(mShader->get());
	}
}

Scene::~Scene()
{
	delete mCamera;
}
