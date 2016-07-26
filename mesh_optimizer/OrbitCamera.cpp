#include "OrbitCamera.h"
#include <iostream>
#include <algorithm>
#include "Event.h"

using namespace mewa;
using namespace mewa::cam;

OrbitCamera::OrbitCamera()
{
	mTheta = 0;
	mPhi = 0;
	mRadius = 2.0f;

	mProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f); //Wylicz macierz rzutowania

	float eyeX = 0 + mRadius*cos(mPhi)*sin(mTheta);
	float eyeY = 0 + mRadius*sin(mPhi)*sin(mTheta);
	float eyeZ = 0 + mRadius*cos(mTheta);

	mViewMatrix = glm::lookAt(
		glm::vec3(eyeX, eyeY, eyeZ),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	mPrevX = 0;
	mPrevY = 0;


	Event::registerEventListener(Event::Type::MouseScrolled, [this](Event const* ev){
		auto event = static_cast<MouseScrolled const*>(ev);
		std::cout << "Ev " << event->y << std::endl;
		if (mRadius - event->y * 0.5 > 0)
			mRadius -= event->y * 0.5;
		update(mPrevX, mPrevY);
	});
}

void OrbitCamera::update(double x, double y)
{
	mTheta = glm::degrees(mTheta);
	mPhi = glm::degrees(mPhi);

	mTheta += x - mPrevX;
	mPhi += y - mPrevY;

	std::cout << "x: " << x << " y: " << y << std::endl;

	mPrevX = x;
	mPrevY = y;

	std::cout << "Theta: " << mTheta << " Phi: " << mPhi << std::endl;

	//mTheta = std::min(std::max(0.0f, mTheta), 360.0f);
	//mPhi = std::min(std::max(0.0f, mPhi), 180.0f);

	mTheta = glm::radians(mTheta);
	mPhi = glm::radians(mPhi);

	float eyeX = 0 + mRadius*cos(mPhi)*sin(mTheta);
	float eyeY = 0 + mRadius*sin(mPhi)*sin(mTheta);
	float eyeZ = 0 + mRadius*cos(mTheta);

	mViewMatrix = glm::lookAt(
		glm::vec3(eyeX, eyeY, eyeZ),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	mProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
}


OrbitCamera::~OrbitCamera()
{
}
