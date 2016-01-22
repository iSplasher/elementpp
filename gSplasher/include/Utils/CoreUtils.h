#pragma once

#include "../Global.h"
#include "Primitives.h"

#include <SFML/Graphics.hpp>

NAMESPACE_BEGIN

/// <summary>
/// Defines size and position.
/// </summary>
struct gCoreProperties {
	// *structers
	gCoreProperties();
	gCoreProperties(Size s, Point p): size(s), pos(p) {}

	// data members
	Size size;
	Point pos;
};


/// <summary>
/// Defines widget colors and font and position
/// </summary>
struct gWidgetProperties : gCoreProperties {
	// *structers
	gWidgetProperties() : gCoreProperties(){}
	gWidgetProperties(Size s = Size(), Point p = Point()) : gCoreProperties(s, p) {}

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
class GSPLASHER_API Painter : public sf::Drawable {

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

NAMESPACE_END
