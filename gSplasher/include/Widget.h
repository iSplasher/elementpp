#pragma once

#include "Global.h"
#include "Layout.h"
#include "Core.h"
#include "Utils/CoreUtils.h"
#include "Utils/Primitives.h"

#include <Windows.h>
#include <SFML/Graphics.hpp>


NAMESPACE_BEGIN

/// <summary>
/// Core widget. Can be used as a window or embedded in another widget. 
/// </summary>
class gCoreWidget : public gLayoutable {
	friend class gApplication;
public:
	// *structers
	explicit gCoreWidget(gCoreWidget *parent = nullptr);
	gCoreWidget(const gCoreWidget&);
	explicit gCoreWidget(const gWidgetProperties s, gCoreWidget *parent = nullptr);
	virtual ~gCoreWidget() = default;

	// member methods
	virtual void paint(Painter &painter, gWidgetProperties &widget_style);
	virtual void update() override;
	virtual void event(sf::Event ev = sf::Event());
	void setLayout(gLayout&);

	// data members
	gWidgetProperties style;

protected:
	// member methods
	static bool setTransparency(HWND hWnd, unsigned char alpha);

	// data members
	std::shared_ptr<gApplication> const *app;
	std::unique_ptr<sf::RenderWindow> r_window;
	gLayout &m_layout;
	gCoreWidget *parent_widget;
};

NAMESPACE_END