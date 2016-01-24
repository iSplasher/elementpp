#pragma once

#include "../Global.h"
#include "Primitives.h"

#include <SFML/Graphics.hpp>

NAMESPACE_BEGIN

/// <summary>
/// Defines size and position.
/// </summary>
struct GSPLASHER_API gCoreProperties {
	// *structers
	gCoreProperties() : gCoreProperties(0, 0, 0, 0){}
	gCoreProperties(Point p, Size s): size(s), pos(p) {}
	gCoreProperties(int x, int y, int w, int h): size(w, h), pos(x, y) {}

	// data members
	Size size;
	Point pos;
};


/// <summary>
/// Defines widget colors and font and position
/// </summary>
struct GSPLASHER_API gWidgetProperties : gCoreProperties {
	// *structers
	gWidgetProperties(int x, int y, int w, int h) :
		gCoreProperties(x, y, w, h) {}
	gWidgetProperties(Point p = Point(), Size s = Size()) :
		gWidgetProperties(p.x, p.y, s.width, s.height) {}

	// data members
	Color fg_color;
	Color bg_color;
	Color active_color;

	const float alpha_multiplier = 2.55; // multiply the desired transparency in percents. Lower = more transparent
	const float base_alpha = alpha_multiplier * 60; // 40% transparency
};

/// <summary>
/// A painter used to paint on a gCoreWidget
/// </summary>
using Painter = sf::Drawable;

NAMESPACE_END
