#include "PlainShaderProgram.h"

using namespace mewa;

GLchar const* PlainShaderProgram::kVertexShaderSrc = GLSL(330,
	uniform mat4 M; \n
	uniform mat4 V; \n
	uniform mat4 P; \n

	layout(location = 0) in vec3 vertex; \n
	layout(location = 1) in vec3 normal; \n

	out vec3 Normal; \n
	out vec3 FragPos; \n
	void main(void)	{
	gl_Position = P * V *  M * vec4(vertex, 1.0); \n
		FragPos = vec3(M * vec4(vertex, 1.0)); \n
		Normal = vec3(M * vec4(normal, 0)); \n
}
);

GLchar const* PlainShaderProgram::kFragmentShaderSrc = GLSL(
	330,
struct Material {
	\n
		vec3 ambient; \n
		vec3 diffuse; \n
		vec3 specular; \n
		float shininess; \n
}; \n
struct Light {
	\n
		vec3 ambient; \n
		vec3 position; \n
		vec3 diffuse; \n
		vec3 specular; \n
};

in vec3 FragPos; \n
in vec3 Normal; \n

out vec4 color; \n

uniform vec3 viewPos; \n
uniform Material material; \n
uniform Light light; \n
uniform mat4 M; \n

void main()\n
{ \n
// Ambient
vec3 ambient = light.ambient * material.ambient; \n

// Diffuse 
vec3 norm = normalize(mat3(transpose(inverse(M))) * Normal); \n
vec3 lightDir = normalize(light.position - FragPos); \n
float diff = max(dot(norm, lightDir), 0.0); \n
vec3 diffuse = light.diffuse * (diff * material.diffuse); \n

// Specular
vec3 viewDir = normalize(viewPos - FragPos); \n
vec3 reflectDir = reflect(-lightDir, norm); \n
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); \n
vec3 specular = light.specular * (spec * material.specular); \n

vec3 result = ambient + diffuse + specular; \n
color = vec4(result, 1.0f); \n
}\n
);

PlainShaderProgram::PlainShaderProgram()
	: ShaderProgram(kVertexShaderSrc, kFragmentShaderSrc)
{
}


PlainShaderProgram::~PlainShaderProgram()
{
}
