#pragma once
#include "glconf.h"

namespace mewa {
	class Material
	{
		glm::vec3 mAmbient;
		glm::vec3 mDiffuse;
		glm::vec3 mSpecular;
		float mShininess;
	public:
		Material(glm::vec3 const&& ambient, glm::vec3 const&& diffuse, 
			glm::vec3 const&& specular, float shininess);
		virtual ~Material();

		glm::vec3 const& ambient();
		glm::vec3 const& diffuse();
		glm::vec3 const& specular();
		float shininess();
	};
}

