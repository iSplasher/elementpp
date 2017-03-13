#pragma once

#include "global.h"
#include "core/primitive.h"

NAMESPACE_BEGIN
class Element;
class Widget;


struct Event {
	Event( Widget* w ) : widget( w ) {}

	Widget* widget = nullptr;
};


struct MouseEvent : Event {

	MouseEvent( Point position = Point(), MouseButton button = MouseButton::None, Widget* widget = nullptr )
		: Event( widget ),
		  position( position ),
		  button( button ) {}

	Point position;
	MouseButton button;
};


NAMESPACE_END
