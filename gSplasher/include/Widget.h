#pragma once

#include "Global.h"
#include "Layout.h"
#include "Core.h"
#include "Event.h"
#include "Utils/CoreUtils.h"

#include <SFML/Graphics.hpp>


NAMESPACE_BEGIN

/// <summary>
/// Core widget. Can be used as a window or embedded in another widget. 
/// </summary>
class GSPLASHER_API gCoreWidget { //: public gLayoutable {
	using WidgetPtr = std::shared_ptr<gCoreWidget>;
	friend class gApplication;
public:
	// *structers
	explicit gCoreWidget(gCoreWidget *parent = nullptr);
	//gCoreWidget(const gCoreWidget&);
	//explicit gCoreWidget(const gWidgetProperties s, gCoreWidget *parent = nullptr);
	virtual ~gCoreWidget() = default;

	// member methods
	//virtual void paint(Painter &painter, gWidgetProperties &widget_style) {}
	virtual void update();// override;
	virtual void event(gCore *sender, EventPtr ev); // override
	//void setLayout(gLayout&);

	// data members
	gWidgetProperties style;

protected:
	// member methods
	bool setTransparency(unsigned char alpha) const;

	// data members
	WidgetPtr parent_widget;
	std::unique_ptr<sf::RenderWindow> r_window;
	//gLayout &m_layout;

};

NAMESPACE_END