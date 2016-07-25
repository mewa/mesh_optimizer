#include "ShaderProgram.h"
#include <cstdio>

using namespace mewa;

ShaderProgram::ShaderProgram(GLchar const* vertexShaderSrc, GLchar const* fragmentShaderSrc)
	: ShaderProgram(vertexShaderSrc, NULL, fragmentShaderSrc)
{
}
ShaderProgram::ShaderProgram(GLchar const* vertexShaderSrc, GLchar const* geometryShaderSrc, GLchar const* fragmentShaderSrc)
{
	mProgram = glCreateProgram();

	mVertexShader = makeShader(GL_VERTEX_SHADER, vertexShaderSrc);
	mFragmentShader = makeShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
	if (geometryShaderSrc) {
		mGeometryShader = makeShader(GL_GEOMETRY_SHADER, geometryShaderSrc);
		glAttachShader(mProgram, mGeometryShader);
	}
	else {
		mGeometryShader = 0;
	}
	glAttachShader(mProgram, mVertexShader);
	glAttachShader(mProgram, mFragmentShader);

	glLinkProgram(mProgram);

	//Pobierz log b³êdów linkowania i wyœwietl
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(mProgram, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}
}

GLuint ShaderProgram::makeShader(GLenum shaderType, GLchar const* shaderSrc) {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSrc, NULL);

	glCompileShader(shader);

	//Pobierz log b³êdów kompilacji i wyœwietl
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}
	return shader;
}

GLuint ShaderProgram::get() const {
	return mProgram;
}

ShaderProgram::~ShaderProgram()
{
}
