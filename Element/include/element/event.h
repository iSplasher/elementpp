#pragma once

#include "global.h"
#include "core/primitive.h"

NAMESPACE_BEGIN
class Element;
class Widget;

struct MouseEvent {

	MouseEvent( Point position = Point(), MouseButton button = MouseButton::None, Widget* widget = nullptr )
		: position( position ),
		  button( button ),
		  widget( widget ) {}

	Point position;
	MouseButton button;
	Widget* widget = nullptr;



};



NAMESPACE_END
