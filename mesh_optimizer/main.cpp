// 6ix.cpp : Defines the entry point for the console application.
//

#include "tiny_obj_loader.h"
#include "glconf.h"
#include <cstdio>
#include <iostream>
#include "Scene.h"
#include "OrbitCamera.h"

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
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(33);
	}
	return window;
}

#define GLSL(version, shader)  "#version " #version "\n" #shader

void drawFrame(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBindVertexArray(vao);
	scene->draw();
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	glfwSwapBuffers(window);
}

int main(int argc, char* argv[])
{
	GLFWwindow* window;

	window = initGLFW();
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	initializeGLEW();

	glClearColor(0, 0.3, 0.3, 1); //Czyœæ ekran na czarno	
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie Z-Bufora

	scene = new mewa::Scene(window);
	scene->addObject(new mewa::SceneObject("Handgun_obj.obj"));
	scene->registerCamera(new mewa::cam::OrbitCamera());

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		drawFrame(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

