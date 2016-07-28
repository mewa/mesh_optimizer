#pragma once
#include "glconf.h"

#define GLSL(version, shader)  "#version " #version "\n" #shader

namespace mewa {
	class ShaderProgram {
		GLuint mProgram;
		GLuint mVertexShader;
		GLuint mGeometryShader;
		GLuint mFragmentShader;

		GLuint makeShader(GLenum shaderType, GLchar const* shaderSrc);
	public:
		ShaderProgram(GLchar const* vertexShaderSrc, GLchar const* fragmentShaderSrc);
		ShaderProgram(GLchar const* vertexShaderSrc, GLchar const* geometryShaderSrc, GLchar const* fragmentShaderSrc);
		virtual ~ShaderProgram();

		GLuint get() const;
	};
}