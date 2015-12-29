#pragma once
#include "include/Global.h"

#include <SFML/Graphics.hpp>

NAMESPACE_BEGIN

using Point = sf::Vector2i;
struct Size final : sf::Vector2u {
	Size() : sf::Vector2u() {
	}
	Size(const Size &other) : sf::Vector2u(other) {}
	Size(const unsigned w, const unsigned h) : sf::Vector2u(w, h) {
	}
	~Size() = default;

	void setWidth(const unsigned w) {
		x = w;
	}
	void setHeight(const unsigned h) {
		y = h;
	}

	unsigned width() const {
		return x;
	}

	unsigned height() const {
		return y;
	}

	Size &operator+=(const int& i){
		x += i;
		y += i;
		return *this;
		}
	Size &operator+=(const Size& s){
		x += s.x;
		y += s.y;
		return *this;
		}
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

