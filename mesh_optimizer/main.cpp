// 6ix.cpp : Defines the entry point for the console application.
//

#include "glconf.h"
#include <cstdio>
#include <iostream>
#include "Scene.h"
#include "FirstPersonCamera.h"
#include "MeshDecimator.h"
#include "EdgeCollapseOperator.h"
#include <thread>
#include "Event.h"

GLuint bufVertices;
GLuint vao;

void initializeGLEW() {
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: Nie uda³o siê zainicjowaæ biblioteki GLEW. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void printError() {

}

GLFWwindow* initGLFW(GLFWwindow* share) {
	/* Initialize the library */
	if (!glfwInit())
		exit(33);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(600, 600, "Hello World", NULL, share);
	if (!window)
	{
		glfwTerminate();
		exit(33);
	}
	return window;
}

#define GLSL(version, shader)  "#version " #version "\n" #shader

double diff = 0;
long frameCount = 0;
double lastTime = glfwGetTime();
char buf[20];
float fps;

void calcFPS(GLFWwindow* window) {
	diff = glfwGetTime() - lastTime;
	frameCount++;
	if (diff > 1.0) {
		fps = frameCount / diff / 100;
		frameCount = 0;
		sprintf(buf, "FPS: %.3f", fps);
		std::cout << buf << std::endl;
		lastTime += 1.0;
	}
}

void drawFrame(GLFWwindow* window, mewa::Scene* scene) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->draw();

	glfwSwapBuffers(window);
	calcFPS(window);
}

GLFWwindow* win2;
int const spacing = 20;

void posCallback(GLFWwindow* window, int x, int y) {
	int width;
	glfwGetWindowSize(window, &width, NULL);
	glfwSetWindowPos(win2, x + width + spacing, y);
	glfwShowWindow(win2);
	glfwShowWindow(window);
}

int decimationLevel = 0;

void decimateModel(mewa::Model*& model, mewa::Scene* scene) {
	mewa::decimator::MeshDecimator decimator;
	auto decimatedModel = new mewa::Model();
	int i = 0;
	for (auto it = model->meshes().begin(); it != model->meshes().end(); ++it) {
		std::cout << "Decimating mesh " << ++i << " out of " << model->meshes().size() << std::endl;
		decimatedModel->addMesh(decimator.decimate(*it));
	}
	for (auto it = model->materials().begin(); it != model->materials().end(); ++it) {
		decimatedModel->addMaterial(*it);
	}
	scene->removeObject(model);
	scene->addObject(decimatedModel);
	model = decimatedModel;
}

void decimated(GLFWwindow* w, mewa::cam::Camera* cam, std::string file) {
	GLFWwindow* window = initGLFW(w);
	win2 = window;
	int x, y, width;
	glfwGetWindowPos(w, &x, &y);
	glfwGetWindowSize(w, &width, NULL);
	glfwSetWindowPos(win2, x + width + spacing, y);
	glfwShowWindow(window);
	glfwShowWindow(w);
	glfwSetWindowPosCallback(w, posCallback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	initializeGLEW();

	glClearColor(0.2, 0.5, 0.5, 1); //Czyœæ ekran na czarno	
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie Z-Bufora

	auto scene = new mewa::Scene(window);

	auto model = new mewa::Model(file.c_str());
	scene->addObject(model);
	scene->registerCamera(cam);

	int currentDecimationLevel = decimationLevel;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (decimationLevel != currentDecimationLevel) {
			decimateModel(model, scene);
			currentDecimationLevel = decimationLevel;
		}
		drawFrame(window, scene);

		glfwPollEvents();
	}

	glfwTerminate();
}

int main(int argc, char* argv[])
{
	GLFWwindow* window;

	window = initGLFW(NULL);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	initializeGLEW();

	glClearColor(0.2, 0.5, 0.5, 1); //Czyœæ ekran na czarno	
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie Z-Bufora

	auto scene = new mewa::Scene(window);

	std::string file = "Avent.obj";

	auto model = new mewa::Model(file);
	scene->addObject(model);
	auto cam = new mewa::cam::FirstPersonCamera(window);
	scene->registerCamera(cam);

	std::thread decimatedWindowThread(&decimated, window, cam, file);

	mewa::Event::registerEventListener(mewa::Event::Type::KeyPressed, [](mewa::Event const* ev) {
		auto event = static_cast<mewa::KeyPressed const*>(ev);
		if (event->action == GLFW_RELEASE && event->key == GLFW_KEY_ENTER) {
			std::cout << "Decimation level: " << ++decimationLevel << std::endl;
		}
	});

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		drawFrame(window, scene);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

