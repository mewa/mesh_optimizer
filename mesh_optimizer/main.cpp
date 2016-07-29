// 6ix.cpp : Defines the entry point for the console application.
//

#include "glconf.h"
#include <cstdio>
#include <iostream>
#include "Scene.h"
#include "FirstPersonCamera.h"
#include "MeshDecimator.h"

mewa::Scene* scene;

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

GLFWwindow* initGLFW() {
	/* Initialize the library */
	if (!glfwInit())
		exit(33);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(600, 600, "Hello World", NULL, NULL);
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

void drawFrame(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBindVertexArray(vao);
	scene->draw();
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	glfwSwapBuffers(window);
	calcFPS(window);
}

int main(int argc, char* argv[])
{
	GLFWwindow* window;

	window = initGLFW();
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	initializeGLEW();

	glClearColor(0.3, 0.3, 0.3, 1); //Czyœæ ekran na czarno	
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie Z-Bufora

	scene = new mewa::Scene(window);
	scene->addObject(new mewa::Model("cyborg.obj"));
	scene->registerCamera(new mewa::cam::FirstPersonCamera(window));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		drawFrame(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

