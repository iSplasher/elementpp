#pragma once

#include "Global.h"
#include "Layout.h"
#include "Core.h"

#include <Windows.h>


NAMESPACE_BEGIN

struct WidgetType : BaseType {
	enum Shape {

	};

	WidgetType() = default;
	~WidgetType() = default;
};

// Look, height and shape of widget
struct WidgetProperties : BaseProperties {
	WidgetProperties();
	WidgetProperties(Color bg_color, Color fg_color, Color grab_color);

	Color foreground_color;
	Color grabbed_color;
	WidgetType type;
	const float alpha_multiplier = 2.55; // multiply the desired transparency in percents. Lower = more transparent
	const float base_alpha = alpha_multiplier * 60; // 40% transparency
	bool is_widget;
	bool is_window;
};

class BaseWidget : public Layoutable {
	friend class BaseFrame;
public:
	explicit BaseWidget(BaseWidget *_parent = nullptr);
	BaseWidget(const BaseWidget&);
	explicit BaseWidget(const WidgetProperties s, BaseWidget *_parent = nullptr);
	virtual ~BaseWidget() = default;

	// member methods
	virtual void update() override;
	virtual void event(sf::Event ev = sf::Event());
	void setLayout(const Layout&);

	// data members
	BaseWidget *parent;
	bool is_grabbed = false;

	sf::Vector2i grabOffset;
	WidgetProperties style;
	Painter painter;

protected:
	// member methods
	virtual void paint();
	virtual void paint(Painter& p, WidgetProperties _style = WidgetProperties());

	bool setShape();
	static bool setTransparency(HWND hWnd, unsigned char alpha);

	// data members
	BaseFrame const *manager = nullptr;
	std::unique_ptr<sf::RenderWindow> sfwindow;
	Layout &m_layout;
};

NAMESPACE_END