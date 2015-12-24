// Contains SFML wrappers
#pragma once
#include <SFML/Graphics.hpp>

namespace gspl
{
struct Color : sf::Color {
	Color() : sf::Color(0, 0, 0) {}
	Color(sf::Uint8 red, sf::Uint8 green, sf::Uint8 blue, sf::Uint8 alpha = 255) : sf::Color(red, green, blue, alpha) {
	}
	explicit Color(sf::Uint8 color) : sf::Color(color) {
	}
};

class Painter : public sf::Drawable {

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

}
