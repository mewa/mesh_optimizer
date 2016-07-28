#define _USE_MATH_DEFINES
#include "Scene.h"
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <functional>

using namespace mewa;

#define GLSL(version, shader)  "#version " #version "\n" #shader

float prevX, prevY, theta, phi;
float sensitivity = 10;

void mouse_click(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (button = GLFW_MOUSE_BUTTON_2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

Scene::Scene(GLFWwindow* window)
{
	using namespace std::placeholders;
	Event::startListening(window);
	Event::registerEventListener(Event::Type::MouseMoved, [this](Event const* ev) {
		auto event = static_cast<MouseEvent const*>(ev);

		mCamera->update(event->x, event->y);
	});
	glfwSetMouseButtonCallback(window, mouse_click);
	makeShaderProgram();
}

void Scene::makeShaderProgram() {
	GLchar const* vShaderSrc = GLSL(330,
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

	GLchar const* fShaderSrc = GLSL(
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

	mShader = new ShaderProgram(vShaderSrc, fShaderSrc);
}

void Scene::addObject(Model* obj) {
	mObjects.push_back(std::unique_ptr<Model>(obj));
}

void Scene::registerCamera(cam::Camera* camera) {
	mCamera = camera;
}

void Scene::draw() {
	GLint glVLocation = glGetUniformLocation(mShader->get(), "V");
	GLint glPLocation = glGetUniformLocation(mShader->get(), "P");

	GLint viewPosLocation = glGetUniformLocation(mShader->get(), "viewPos");

	glUniformMatrix4fv(glVLocation, 1, false, glm::value_ptr(mCamera->view()));
	glUniformMatrix4fv(glPLocation, 1, false, glm::value_ptr(mCamera->projection()));

	glUniform3fv(viewPosLocation, 1, glm::value_ptr(mCamera->position()));

	for (int i = 0; i < mObjects.size(); ++i) {
		GLint lightColorLocation = glGetUniformLocation(mShader->get(), "light.ambient");
		GLint lightPositionLocation = glGetUniformLocation(mShader->get(), "light.position");
		GLint lightDiffuseLocation = glGetUniformLocation(mShader->get(), "light.diffuse");
		GLint lightSpecularLocation = glGetUniformLocation(mShader->get(), "light.diffuse");

		glUniform3f(lightColorLocation, 1, 1, 1);
		glUniform3f(lightPositionLocation, 3, 2, 5);
		glUniform3f(lightDiffuseLocation, 1, 1, 1);
		glUniform3f(lightSpecularLocation, 1, 1, 1);

		mCamera->update();
		mObjects[i].get()->draw(mShader->get());
	}
}

Scene::~Scene()
{
	delete mCamera;
}
