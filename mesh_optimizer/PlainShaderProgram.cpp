#include "PlainShaderProgram.h"

using namespace mewa;

GLchar const* PlainShaderProgram::kVertexShaderSrc = GLSL(330,
	uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;
void main(void)	{
	gl_Position = P * V *  M * vec4(vertex, 1.0);
	FragPos = vec3(M * vec4(vertex, 1.0));
	Normal = vec3(M * vec4(normal, 0));
}
);

GLchar const* PlainShaderProgram::kFragmentShaderSrc = GLSL(
	330,
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
	vec3 ambient;
	vec3 position;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light lights[20];
uniform mat4 M;
uniform int numLights;
vec3 calcLight(Light light, vec3 viewDir, vec3 norm);
void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 norm = normalize(mat3(transpose(inverse(M))) * Normal);

	vec3 result = vec3(0);
	for (int i = 0; i < numLights; ++i) {
		result += calcLight(lights[i], viewDir, norm);
	}
	color = vec4(result, 1.0f);
}
vec3 calcLight(Light light, vec3 viewDir, vec3 norm) {
	// Ambient
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse 
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	return result;
}
);

PlainShaderProgram::PlainShaderProgram()
	: ShaderProgram(kVertexShaderSrc, kFragmentShaderSrc)
{
}


PlainShaderProgram::~PlainShaderProgram()
{
}
