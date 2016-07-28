#pragma once

#include <map>
#include <vector>
#include <functional>
#include "glconf.h"

namespace mewa {
	class Event
	{
	public:
		enum Type {
			MouseMoved, KeyPressed, MouseScrolled
		};
	private:
		Event::Type mType;
		static std::map<Event::Type, std::vector<std::function<void(Event const*)> > > mEvents;
	protected:
		Event();
	public:
		static void registerEventListener(Event::Type type, std::function<void(Event const*)> callback);
		static void dispatchEvent(Event* event);
		static void startListening(GLFWwindow* window);
		virtual Event::Type getType() const = 0;
		virtual ~Event();
	};

	class KeyPressed
		: public Event {
	public:
		int const action;
		int const key;
		KeyPressed(int action, int key);
		virtual Event::Type getType() const;
	};

	class MouseEvent
		: public Event {
	public:
		double const x;
		double const y;
		MouseEvent(double x, double y);
		virtual Event::Type getType() const;
	};

	class MouseScrolled
		: public Event {
	public:
		double const x;
		double const y;
		MouseScrolled(double x, double y);
		virtual Event::Type getType() const;
	};
}
