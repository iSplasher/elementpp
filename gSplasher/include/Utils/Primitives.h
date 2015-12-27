#pragma once
#include <SFML/Graphics.hpp>

namespace gspl
{
using Point = sf::Vector2f;
using Rect = sf::FloatRect;

struct Size final : private sf::Vector2u {
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
		}
	Size &operator+=(const Size& s){
		x += s.x;
		y += s.y;
		}
};

}

