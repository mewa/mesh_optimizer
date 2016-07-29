#include "FirstPersonCamera.h"
#include "Event.h"
#include <iostream>

using namespace mewa;
using namespace mewa::cam;

GLuint FirstPersonCamera::mode = GL_FILL;

void mouse_click(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		FirstPersonCamera::mode = GL_FILL;
	}
	else if (button = GLFW_MOUSE_BUTTON_2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		FirstPersonCamera::mode = GL_LINE;
	}
}

FirstPersonCamera::FirstPersonCamera(GLFWwindow* window)
{
	mWindow = window;
	isFirst = true;
	mLastTime = glfwGetTime();
	mAngleX = 3.14;
	mAngleY = 0;
	mPosition = glm::vec3(0, 0, 5);
	mProjectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f);
	updateViewMatrix();
	Event::registerEventListener(Event::Type::MouseMoved, [this](Event const* ev) {
		auto event = static_cast<MouseEvent const*>(ev);
		update(event->x, event->y);
	});
	Event::registerEventListener(Event::Type::KeyPressed, [this](Event const* ev) {
		auto event = static_cast<KeyPressed const*>(ev);
	});
	glfwSetMouseButtonCallback(window, mouse_click);
}

void FirstPersonCamera::update() {
	glPolygonMode(GL_FRONT_AND_BACK, mode);
	float unit = 5.0f;
	if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS) {
		move(unit);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS) {
		move(-unit);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS) {
		strafe(-unit);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS) {
		strafe(unit);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
		up(unit);
	}
	mLastTime = glfwGetTime();
}

void FirstPersonCamera::updateViewMatrix() {
	glm::vec3 direction(
		cos(mAngleY) * sin(mAngleX),
		sin(mAngleY),
		cos(mAngleY) * cos(mAngleX)
		);
	glm::vec3 right = glm::vec3(
		sin(mAngleX - 3.14f / 2.0f),
		0,
		cos(mAngleX - 3.14f / 2.0f)
		);
	glm::vec3 up = glm::cross(right, direction);
	mViewMatrix = glm::lookAt(mPosition,
		mPosition + direction,
		up);
}


FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::up(float unit) {
	float deltaTime = glfwGetTime() - mLastTime;
	glm::vec3 up = glm::vec3(0, 1, 0);
	mPosition += unit * up * deltaTime;
	updateViewMatrix();
}

void FirstPersonCamera::move(float unit) {
	float deltaTime = glfwGetTime() - mLastTime;
	glm::vec3 direction(
		cos(mAngleY) * sin(mAngleX),
		sin(mAngleY),
		cos(mAngleY) * cos(mAngleX)
		);
	mPosition += unit * direction * deltaTime;
	updateViewMatrix();
}

void FirstPersonCamera::strafe(float unit) {
	float deltaTime = glfwGetTime() - mLastTime;
	glm::vec3 right = glm::vec3(
		sin(mAngleX - 3.14f / 2.0f),
		0,
		cos(mAngleX - 3.14f / 2.0f)
		);
	mPosition += unit * right * deltaTime;
	updateViewMatrix();
}

void FirstPersonCamera::update(double x, double y) {
	if (isFirst) {
		mPrevX = x;
		mPrevY = y;
		isFirst = false;
	}

	mAngleX += glm::radians(mPrevX - x);
	mAngleY += glm::radians(mPrevY - y);

	updateViewMatrix();

	mPrevX = x;
	mPrevY = y;
}
