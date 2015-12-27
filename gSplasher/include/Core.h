// Contains base types
#pragma once

#include "Wrapper.h"
#include "Utils/Region.h"
#include "Utils/Primitives.h"

#include <SFML\Graphics.hpp>
#include <Windows.h>
#include <memory>
#include <vector>

namespace gspl
{

class BaseFrame {
	friend class BaseWidget;
public:
	BaseFrame();
	BaseFrame(BaseWidget&);
	bool run() const;
	virtual ~BaseFrame() = default;
private:
	bool process_events() const;

	std::shared_ptr<std::vector<BaseWidget*>> widgets;
	static std::shared_ptr<BaseFrame> first_manager;
};

struct BaseType {
	BaseType() = default;
	~BaseType() = default;
};

// Widget shape and other things
struct WidgetType : BaseType {
	enum Shape {

	};

	WidgetType() = default;
	~WidgetType() = default;

	Region getRegion();

};

struct BaseProperties {
	using Font = sf::Font;
	BaseProperties();

	BaseProperties(Color c);
	Color base_color;
	Font base_font;
	Size size;
	Point pos;
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

// Widget & Layout

class Core {
	friend class Layout;
public:
	Core();
	virtual ~Core() = default;

	virtual void update() = 0;

private:
	bool in_layout;
	Rect available_space;
protected:
	Size requested_size;
	Point requested_pos;
};

class Layout : public Core {
	friend class BaseWidget;
public:
	Layout();
	virtual ~Layout() = 0;
	void add(BaseWidget &w);
	void add(Layout &l);
	BaseWidget& takeWidget();
	Layout& takeLayout();

protected:
	Core &parent;
	std::shared_ptr<std::vector<Core*>> layout_members;

	unsigned margin;
private:
	void init();
	void update() override;
};

class BaseWidget : public Core {
	friend class BaseFrame;
public:
	explicit BaseWidget(BaseWidget *_parent=nullptr);
	BaseWidget(const BaseWidget&);
	explicit BaseWidget(const WidgetProperties s, BaseWidget *_parent=nullptr);
	virtual ~BaseWidget() = default;

	// member methods
	virtual void update() override;
	virtual void event(sf::Event ev=sf::Event());
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
	virtual void paint(Painter& p, WidgetProperties _style=WidgetProperties());

	bool setShape();
	static bool setTransparency(HWND hWnd, unsigned char alpha);

	// data members
	BaseFrame const *manager = nullptr;
	std::unique_ptr<sf::RenderWindow> sfwindow;
	Layout &m_layout;
};
}



