#include "../include/Event.h"
#include "../include/Core.h"

USING_NAMESPACE

int gEventManager::head;
int gEventManager::tail;

gEvent::gEvent(const gEvent& other) {
	m_type = other.type();
}

gMouseEvent::gMouseEvent(Type t, Point p) : gInputEvent(t), x(p.x), y(p.y) {}

gMouseEvent::gMouseEvent(sf::Event ev) : gInputEvent(gEvent::None), x(), y() {
	using SE = sf::Event;
	// TODO: finish implementing these event types
	switch(ev.type) {
	case SE::MouseMoved:
		m_type = MouseMove;
		x = ev.mouseMove.x;
		y = ev.mouseMove.y;
		break;
	case SE::MouseButtonPressed:
		m_type = MouseButtonPress;
		x = ev.mouseButton.x;
		y = ev.mouseButton.y;
		break;
	case SE::MouseButtonReleased:
		m_type = MouseButtonRelease;
		break;
	default:
		m_type = None;
		break;
	}
}

gKeyEvent::gKeyEvent(Type t, int k, std::string txt) : gInputEvent(t), key(k), text(txt) {}

gKeyEvent::gKeyEvent(sf::Event ev) : gInputEvent(gEvent::None), key(), text() {
	using SE = sf::Event;
	switch(ev.type) {
	case SE::KeyPressed || SE::KeyReleased:
		switch(ev.type) {
		case SE::KeyPressed:
			m_type = KeyPress;
			break;
		case SE::KeyReleased:
			m_type = KeyRelease;
			break;
		}

		key = ev.key.code;
		alt = ev.key.alt;
		control = ev.key.control;
		shift = ev.key.shift;
		system = ev.key.system;
		break;
	default:
		m_type = gEvent::None;
		break;
	}
}

void gEventManager::init() {
	head = 0;
	tail = 0;
}

void gEventManager::dispatchEvent(EventPtr ev) {
	eventqueue.push_back(ev);
}

void gEventManager::processEv() {
	while (!eventqueue.empty()) {
		EventPtr ev = eventqueue.front();
		if (gApp) {
			gApp->event(gApp, ev);
		}
		eventqueue.erase(eventqueue.begin());
	}
}
