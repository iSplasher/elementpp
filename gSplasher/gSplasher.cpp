#include <SFML\Graphics.hpp>
#include <Windows.h>

void makeWindowTransparent(sf::RenderWindow &window) {
	HWND hwnd = window.getSystemHandle();
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, GetWindowLongPtr(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	}

inline void setWindowAlpha(sf::RenderWindow &window, sf::Uint8 alpha=255){
	SetLayeredWindowAttributes(window.getSystemHandle(), 0, alpha, LWA_ALPHA);
	}

int main() {

	const sf::Color windowColor{ sf::Color(255, 0, 0, 192) };
	const sf::Color windowColorGrabbed{ sf::Color(255, 255, 0, 64) };

	sf::RenderWindow window(sf::VideoMode(200, 200), "gSplasher", sf::Style::None);
	makeWindowTransparent(window);
	window.setVerticalSyncEnabled(true);
	sf::CircleShape shape(100.f);
	sf::Vector2i grabOffset;
	bool windowGrabbed = false;
	shape.setFillColor(sf::Color::Green);
	int winPosX = 1;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					grabOffset = window.getPosition() - sf::Mouse::getPosition();
					windowGrabbed = true;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					windowGrabbed = false;
				}
			}
			else if (event.type == sf::Event::MouseMoved) {
				if (windowGrabbed) {
					window.setPosition(sf::Mouse::getPosition()+grabOffset);
					}
			}
		}

		sf::Color clearColor{ windowColor };
		if (windowGrabbed){
			clearColor = windowColorGrabbed;
			}
		setWindowAlpha(window, 100);
		window.clear(clearColor);
		window.display();
	}

	return 0;
}

