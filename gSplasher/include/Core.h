// Contains base types
#pragma once

#include "Wrapper.h"
#include "Utils/Region.h"

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

struct BaseStyle {
	using Font = sf::Font;
	BaseStyle();

	BaseStyle(Color c);
	Color base_color;
	Font base_font;
};

// Look, height and shape of widget
struct WidgetStyle : BaseStyle {
	WidgetStyle();
	WidgetStyle(Color bg_color, Color fg_color, Color grab_color);

	Color foreground_color;
	Color grabbed_color;
	int height;
	int width;
	WidgetType type;
};

// Widgets

class BaseWidget {
	friend class BaseFrame;
public:
	explicit BaseWidget(BaseWidget *_parent=nullptr);
	BaseWidget(const BaseWidget&);
	explicit BaseWidget(const WidgetStyle s, BaseWidget *_parent=nullptr);
	virtual ~BaseWidget() = default;

	// member methods
	virtual void update();
	virtual void event(sf::Event ev=sf::Event());

	// data members
	BaseWidget *parent;
	bool is_grabbed = false;

	sf::Vector2i grabOffset;
	WidgetStyle style;
	Painter painter;

protected:
	// member methods
	virtual void paint() const;
	virtual void paint(Painter&) const;

	bool setShape();
	static bool setTransparency(HWND hWnd, unsigned char alpha);

	// data members
	BaseFrame const *manager = nullptr;
	std::unique_ptr<sf::RenderWindow> sfwindow;
	sf::Image bg_img;
	const float alpha_multiplier = 2.55; // multiply the desired transparency in percents. Lower = more transparent
	const float base_alpha = alpha_multiplier * 60; // 40% transparency
	bool is_widget;
	bool is_window;
};
}

