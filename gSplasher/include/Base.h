// Contains base types
#pragma once

#include <SFML\Graphics.hpp>
#include <Windows.h>
#include <memory>
#include <vector>

#include "Wrapper.h"

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


struct Region final {
	using Point = sf::Vector2f;
	using iterator = std::vector<Point>::iterator;

	Region() : path() {
	}
	Region(const Region &other);
	~Region() = default;

	// starting  and ending points
	Point startPoint() const;
	Point endPoint() const;
	const iterator begin();
	const iterator end();
	// will return false or true wether point was made (point won't be added if it makes an irregular shape)
	bool addPoint(std::initializer_list<Point> p);
	bool addPoint(Point p);
	Region &operator<<(const Point &p);
	Region &operator+=(const Region &other);
	Region &operator-=(const Region &other);
	bool insertPoint(std::initializer_list<Point> p);
	bool insertPoint(Point p);
	// if region is inside this region
	bool contains(Region &r);

private:
	// removes points inside.. leaves outer points
	void cleanInside();
	// joins two region while cleaning the insides
	Region *concatenate(Region &r1, Region&r2);

	std::vector<Point> path;
};

Region operator+(const Region &lhs, const Region &rhs);
Region operator-(const Region &lhs, const Region &rhs);

struct BaseType {
	BaseType() = default;
	~BaseType() = default;
};

// Widget shape and other things
struct WidgetType {
	enum Shape {

	};

	WidgetType() = default;
	~WidgetType() = default;

	Region getRegion();

};

struct BaseStyle {
	BaseStyle() : base_color() {
	}
	BaseStyle(gspl::Color c) : base_color(c) {
	}
	gspl::Color base_color;
};

// Look, height and shape of widget
struct WidgetStyle : BaseStyle {
	WidgetStyle();
	WidgetStyle(Color bg_color, Color fg_color, Color grab_color) : BaseStyle(bg_color),
		foreground_color(fg_color), grabbed_color(grab_color), height(0), width(0) {
	}

	Color foreground_color;
	Color grabbed_color;
	int height;
	int width;
	WidgetType type;
};

// Widgets

class BaseWidget {
	friend class BaseManager;
public:
	BaseWidget();
	BaseWidget(const BaseWidget&);
	explicit BaseWidget(const WidgetStyle s) : style(s) {
	}
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

	static bool setShape(HWND hWnd, const sf::Image& image);
	static bool setTransparency(HWND hWnd, unsigned char alpha);

	// data members
	BaseManager const *manager = nullptr;
	std::unique_ptr<sf::RenderWindow> sfwindow;
	sf::Image bg_img;
	const float alpha_multiplier = 2.55; // multiply the desired transparency in percents. Lower = more transparent
	const float base_alpha = alpha_multiplier * 60; // 40% transparency
};
}

