// Contains base types
#pragma once

#include <SFML\Graphics.hpp>
#include <Windows.h>
#include <memory>
#include <vector>

#include "wrapper.h"

inline void makeWindowTransparent(sf::RenderWindow &window) {
	HWND hwnd = window.getSystemHandle();
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, GetWindowLongPtr(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
}

inline void setWindowAlpha(sf::RenderWindow &window, sf::Uint8 alpha = 255) {
	SetLayeredWindowAttributes(window.getSystemHandle(), 0, alpha, LWA_ALPHA);
}

namespace gspl
{

class BaseManager {
	friend class BaseWidget;
public:
	BaseManager();
	BaseManager(BaseWidget&);
	bool run() const;
	virtual ~BaseManager() = default;
private:
	bool process_events() const;

	std::shared_ptr<std::vector<BaseWidget*>> widgets;
	static std::shared_ptr<BaseManager> first_manager;
};

// Styles
struct BaseStyle {
	BaseStyle() : base_color() {
	}
	BaseStyle(gspl::Color c) : base_color(c) {
	}
	gspl::Color base_color;
};

struct WidgetStyle : BaseStyle {
	WidgetStyle() : BaseStyle() {
	}
	WidgetStyle(Color bg_color, Color fg_color, Color grab_color) : BaseStyle(bg_color),
		foreground_color(fg_color), grabbed_color(grab_color) {
	}

	Color foreground_color;
	Color grabbed_color;
};

// Widgets

class BaseWidget {
	friend class BaseManager;
public:
	BaseWidget();
	BaseWidget(const BaseWidget&);
	explicit BaseWidget(const WidgetStyle s) : style(s){}
	virtual ~BaseWidget() = default;

	// member methods
	virtual void update() const;

	// data members
	bool is_grabbed = false;

	sf::Vector2i grabOffset;
	WidgetStyle style;
	Painter painter;

protected:
	// member methods
	virtual void paint() const;
	virtual void paint(Painter&) const;

	// data members
	BaseManager const *manager = nullptr;
	std::unique_ptr<sf::RenderWindow> sfwindow;
};
}

