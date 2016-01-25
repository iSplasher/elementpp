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
class GSPLASHER_API gCoreWidget : gCore { //: public gLayoutable {
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
	virtual void event(EventPtr ev);
	//void setLayout(gLayout&);

	// data members
	gWidgetProperties style;

protected:
	// member methods
	bool setTransparency(unsigned char alpha) const;
	virtual void mousePressEvent(MouseEventPtr ev);
	virtual void mouseMoveEvent(MouseEventPtr ev);
	virtual void mouseReleaseEvent(MouseEventPtr ev);

	// data members
	gCoreWidget *parent_widget;
	std::unique_ptr<sf::RenderWindow> r_window;
	//gLayout &m_layout;

};

NAMESPACE_END