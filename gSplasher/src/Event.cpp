#include "gSplasher/Event.h"
#include "gSplasher/Core.h"

USING_NAMESPACE

int gEventManager::head;
int gEventManager::tail;

gEvent::gEvent(const gEvent& other) {
	m_type = other.type();
}

gInputEvent::gInputEvent(const gInputEvent &other) : gEvent(other.type()) {
	alt = other.alt;
	control = other.control;
	shift = other.shift;
	system = other.system;
}

//gMouseEvent::gMouseEvent(Type t, Point p) : gInputEvent(t), x(p.x), y(p.y){}
//
//gMouseEvent::gMouseEvent(sf::Event ev) : gInputEvent(gEvent::None) {
	//using SE = sf::Event;

	//// TODO: finish implementing these event types
	//switch(ev.type) {
	//case SE::MouseButtonPressed:
	//case SE::MouseButtonReleased:
	//	if (ev.type == SE::MouseButtonPressed) {
	//	m_type = MouseButtonPress;
	//	}
	//	else {
	//	m_type = MouseButtonRelease;
	//	}

	//	button = ev.mouseButton.button;
	//	x = ev.mouseButton.x;
	//	y = ev.mouseButton.y;
	//	break;
	//default:
	//	m_type = None;
	//	break;
	//}
//}

//gMouseEvent::gMouseEvent(const gMouseEvent &other) :
//	gInputEvent(other.type(), other.alt, other.control, other.shift, other.system) {
//	button = other.button;
//	x = other.x;
//	y = other.y;
//}
//
//gKeyEvent::gKeyEvent(Type t, int k, std::string txt) : gInputEvent(t), key(k), text(txt) {}
//
//gKeyEvent::gKeyEvent(sf::Event ev) : gInputEvent(gEvent::None), key(), text() {
//	using SE = sf::Event;
//	switch(ev.type) {
//	case SE::KeyPressed || SE::KeyReleased:
//		switch(ev.type) {
//		case SE::KeyPressed:
//			m_type = KeyPress;
//			break;
//		case SE::KeyReleased:
//			m_type = KeyRelease;
//			break;
//		}
//
//		key = ev.key.code;
//		alt = ev.key.alt;
//		control = ev.key.control;
//		shift = ev.key.shift;
//		system = ev.key.system;
//		break;
//	default:
//		m_type = gEvent::None;
//		break;
//	}
//}

void gEventManager::init() {
	head = 0;
	tail = 0;
}

void gEventManager::dispatchEvent(gCore* receiver, EventPtr ev) {
	ev->receiver = receiver;

	// TODO: no need for pair
	eventqueue.push_back(std::make_pair(receiver, ev));
}

void gEventManager::processEv() {
	while (!eventqueue.empty()) {
		EventPair &ev = eventqueue.front();
		if (gApp) {
			gApp->sendEvent(ev.first, ev.second);
		}
		eventqueue.erase(eventqueue.begin());
	}
}