#include "Material.h"

using namespace mewa;

Material::Material(glm::vec3 const&& ambient, glm::vec3 const&& diffuse,
	glm::vec3 const&& specular, float shininess)
{
	mAmbient = ambient;
	mDiffuse = diffuse;
	mSpecular = specular;
	mShininess = shininess;
}

glm::vec3 const& Material::ambient() {
	return mAmbient;
}
glm::vec3 const& Material::diffuse() {
	return mDiffuse;
}
glm::vec3 const& Material::specular() {
	return mSpecular;
}

float Material::shininess() {
	return mShininess;
}

Material::~Material()
{
}
