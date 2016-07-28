#pragma once
#include "ShaderProgram.h"
namespace mewa {
	class PlainShaderProgram :
		public ShaderProgram
	{
		static GLchar const* kVertexShaderSrc;
		static GLchar const* kFragmentShaderSrc;
	public:
		PlainShaderProgram();
		virtual ~PlainShaderProgram();
	};
}
