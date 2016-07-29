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
	glfwShowWindow(window);
}

void decimated(GLFWwindow* w, mewa::cam::Camera* cam) {
	GLFWwindow* window = initGLFW(w);
	win2 = window;
	int x, y, width;
	glfwGetWindowPos(w, &x, &y);
	glfwGetWindowSize(w, &width, NULL);
	glfwSetWindowPos(win2, x + width + spacing, y);
	glfwShowWindow(w);
	glfwSetWindowPosCallback(w, posCallback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	initializeGLEW();

	glClearColor(0.3, 0.3, 0.3, 1); //Czyœæ ekran na czarno	
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie Z-Bufora

	mewa::decimator::MeshDecimator decimator(new mewa::decimator::EdgeCollapseOperator());

	auto scene = new mewa::Scene(window);

	auto model = new mewa::Model("cube.obj");

	auto decimatedModel = new mewa::Model();
	for (auto it = model->materials().begin(); it != model->materials().end(); ++it) {
		decimatedModel->addMaterial(*it);
	}
	for (auto it = model->meshes().begin(); it != model->meshes().end(); ++it) {
		decimatedModel->addMesh(decimator.decimate(*it));
	}

	scene->addObject(decimatedModel);
	scene->registerCamera(cam);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
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

	glClearColor(0.3, 0.3, 0.3, 1); //Czyœæ ekran na czarno	
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie Z-Bufora

	mewa::decimator::MeshDecimator decimator(new mewa::decimator::EdgeCollapseOperator());

	auto scene = new mewa::Scene(window);

	auto model = new mewa::Model("cube.obj");

	auto decimatedModel = new mewa::Model();
	for (auto it = model->materials().begin(); it != model->materials().end(); ++it) {
		decimatedModel->addMaterial(*it);
	}
	for (auto it = model->meshes().begin(); it != model->meshes().end(); ++it) {
		decimatedModel->addMesh(decimator.decimate(*it));
	}

	scene->addObject(model);
	auto cam = new mewa::cam::FirstPersonCamera(window);
	scene->registerCamera(cam);

	std::thread decimatedWindowThread(&decimated, window, cam);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		drawFrame(window, scene);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

