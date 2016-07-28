#include "Event.h"

using namespace mewa;

Event::Event()
{
}

std::map<Event::Type, std::vector<std::function<void(Event const*)> > > Event::mEvents = std::map<Event::Type, std::vector<std::function<void(Event const*)> > >();

void Event::registerEventListener(Event::Type type, std::function<void(Event const*)> callback) {
	mEvents[type].push_back(callback);
}

void Event::dispatchEvent(Event* event) {
	auto listeners = mEvents[event->getType()];
	for (auto it = listeners.begin(); it != listeners.end(); ++it) {
		(*it)(event);
	}
	delete event;
}

Event::~Event()
{
}

void mousePosCallback(GLFWwindow* window, double x, double y) {
	Event::dispatchEvent(new MouseEvent(x, y));
}

void mouseScrollCallback(GLFWwindow* window, double x, double y) {
	Event::dispatchEvent(new MouseScrolled(x, y));
}

void keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE)
		exit(0);
	Event::dispatchEvent(new KeyPressed(action, key));
}

void Event::startListening(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetKeyCallback(window, keyPressedCallback);
}

KeyPressed::KeyPressed(int action, int key)
	: Event(), action(action), key(key)
{
}

Event::Type KeyPressed::getType() const {
	return Event::Type::KeyPressed;
}

MouseEvent::MouseEvent(double x, double y)
	: Event(), x(x), y(y)
{
}

Event::Type MouseEvent::getType() const {
	return Event::Type::MouseMoved;
}

MouseScrolled::MouseScrolled(double x, double y)
	: Event(), x(x), y(y)
{
}

Event::Type MouseScrolled::getType() const {
	return Event::Type::MouseScrolled;
}
