#pragma once

#include "global.h"
#include "core/primitive.h"

NAMESPACE_BEGIN
class Element;
class Widget;

struct MouseEvent {

	MouseEvent( PointF position = PointF(), MouseButton button = MouseButton::None, Widget* widget = nullptr )
		: position( position ),
		  button( button ),
		  widget( widget ) {}

	PointF position;
	MouseButton button;
	Widget* widget = nullptr;



};



NAMESPACE_END
