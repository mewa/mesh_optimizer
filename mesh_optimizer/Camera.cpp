#include "Camera.h"

using namespace mewa;
using namespace mewa::cam;

Camera::Camera()
{
}

glm::mat4 const& Camera::projection() const {
	return mProjectionMatrix;
}

glm::mat4 const& Camera::view() const {
	return mViewMatrix;
}

glm::vec3 const& Camera::position() const {
	return mPosition;
}

Camera::~Camera()
{
}
