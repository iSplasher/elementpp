#include <memory>
#include <iostream>
#include "base.h"

using namespace gspl;


std::shared_ptr<WindowManager> WindowManager::base_manager = nullptr;

WindowManager::WindowManager() : windows(std::make_shared<std::vector<BaseWindow>>()) {
}

WindowManager::WindowManager(BaseWindow &window) : WindowManager() {
	if (!base_manager) {
		base_manager = std::make_shared<WindowManager>(*this);
	}

	window.manager = this;
	windows->push_back(window);
}

bool WindowManager::process_events() {

	// windows
	int windows_open = 0;
	std::cout << windows->size();
	for (auto window : *windows) {
		if (window.sfwindow->isOpen()) {
			++windows_open;
			sf::Event ev;
			while (window.sfwindow->pollEvent(ev)) {
				if (ev.type == sf::Event::Closed || ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
					window.sfwindow->close();
				}
				else if (ev.type == sf::Event::MouseButtonPressed) {
					if (ev.mouseButton.button == sf::Mouse::Left) {
						window.grabOffset = window.sfwindow->getPosition() - sf::Mouse::getPosition();
						window.is_grabbed = true;
					}
				}
				else if (ev.type == sf::Event::MouseButtonReleased) {
					if (ev.mouseButton.button == sf::Mouse::Left) {
						window.is_grabbed = false;
					}
				}
				else if (ev.type == sf::Event::MouseMoved) {
					if (window.is_grabbed) {
						window.sfwindow->setPosition(sf::Mouse::getPosition() + window.grabOffset);
					}
				}
			}

			sf::Color clear_color{ window.base_window_color };
			if (window.is_grabbed) {
				clear_color = window.base_window_color_grabbed;
			}
			setWindowAlpha(*window.sfwindow, 100);
			window.sfwindow->clear(clear_color);
			window.sfwindow->display();
		}

	}
	return windows_open ? true : false;
}

bool WindowManager::run() {
	return process_events();
}

BaseWindow::BaseWindow() : sfwindow(new sf::RenderWindow(sf::VideoMode(200, 200), "gSplasher", sf::Style::None)) {
	makeWindowTransparent(*sfwindow);
	sfwindow->setVerticalSyncEnabled(true);
};

BaseWindow::BaseWindow(const BaseWindow &other) {

}