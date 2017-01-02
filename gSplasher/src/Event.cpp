#include "gSplasher/Event.h"
#include "gSplasher/Core.h"

USING_NAMESPACE

int EventManager::head;
int EventManager::tail;

Event::Event(const Event& other) {
	m_type = other.type();
}

//MouseEvent::MouseEvent(Type t, Point p) : InputEvent(t), x(p.x), y(p.y){}
//
//MouseEvent::MouseEvent(sf::Event ev) : InputEvent(Event::None) {
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

//MouseEvent::MouseEvent(const MouseEvent &other) :
//	InputEvent(other.type(), other.alt, other.control, other.shift, other.system) {
//	button = other.button;
//	x = other.x;
//	y = other.y;
//}
//
//KeyEvent::KeyEvent(Type t, int k, std::string txt) : InputEvent(t), key(k), text(txt) {}
//
//KeyEvent::KeyEvent(sf::Event ev) : InputEvent(Event::None), key(), text() {
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
//		m_type = Event::None;
//		break;
//	}
//}

void EventManager::init() {
	head = 0;
	tail = 0;
}

void EventManager::dispatchEvent(Core* receiver, EventPtr ev) {
	ev->receiver = receiver;

	// TODO: no need for pair
	eventqueue.push_back(std::make_pair(receiver, ev));
}

void EventManager::processEv() {
	while (!eventqueue.empty()) {
		EventPair &ev = eventqueue.front();
		if (App) {
			App->sendEvent(ev.first, ev.second);
		}
		eventqueue.erase(eventqueue.begin());
	}
}
