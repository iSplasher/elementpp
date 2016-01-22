#pragma once

#include "../Global.h"

#include <SFML/Graphics.hpp>

NAMESPACE_BEGIN

using Point = sf::Vector2i;
using Color = sf::Color;

struct Size final : sf::Vector2u {
	Size(const unsigned w, const unsigned h) : sf::Vector2u(w, h), width(x), height(y) {}
	Size() : Size(0, 0) {}
	Size(const Size &other) : Size(other.width, other.height) {}
	~Size() = default;

	Size &operator+=(const int& i){
		width += i;
		height += i;
		return *this;
		}
	Size &operator+=(const Size& s){
		width += s.width;
		height += s.height;
		return *this;
		}

	// date members
	unsigned &width;
	unsigned &height;
};


struct Rect final : sf::IntRect {
	Rect() : sf::IntRect(){}
	Rect(const int x, const int y, const unsigned width, const unsigned height) : sf::IntRect(x,y,width,height) {}
	Rect(const Point p, const Size size) : sf::IntRect(p, static_cast<sf::Vector2i>(size)){}
	Rect(const Rect &other) : sf::IntRect(other){}

	bool isEmpty() const {
		return width + height == 0 ? true : false;
	}

};


NAMESPACE_END

