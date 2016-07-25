// 6ix.cpp : Defines the entry point for the console application.
//

#include "tiny_obj_loader.h"
#include "glconf.h"
#include <cstdio>
#include <iostream>
#include "Scene.h"

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

	/*GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	auto vShaderSrc = GLSL(330,
		in vec3 position;
	void main(void) {
		gl_Position = vec4(position.xy, 0.0, 1.0);
	}
	);
	glShaderSource(vShader, 1, &vShaderSrc, NULL);
	glCompileShader(vShader);

	int infologLength = 0;
	char* infoLog;
	int charsWritten = 0;

	glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(vShader, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}

	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	auto fShaderSrc = GLSL(330,
		out vec4 color;
	void main(void) {
		color = vec4(1, 0, 0, 1);
	}
	);
	glShaderSource(fShader, 1, &fShaderSrc, NULL);
	glCompileShader(fShader);

	infologLength = 0;
	infoLog;
	charsWritten = 0;

	glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(fShader, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}


	glUseProgram(program);

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(1, &bufVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufVertices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3, new GLuint[]{ 0, 1, 2 }, GL_STATIC_DRAW);

	glGenBuffers(1, &bufVertices);
	glBindBuffer(GL_ARRAY_BUFFER, bufVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLint posAttrib = glGetAttribLocation(program, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	checkError();
	glEnableVertexAttribArray(posAttrib);

	glBindVertexArray(0);*/

	scene = new mewa::Scene(window);
	scene->addObject(new mewa::SceneObject("untitled.obj"));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		drawFrame(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

