#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "base.h"
#include "wrapper.h"

using namespace gspl;


std::shared_ptr<BaseManager> BaseManager::first_manager = nullptr;

BaseManager::BaseManager() : widgets(std::make_shared<std::vector<BaseWidget*>>()) {
}

BaseManager::BaseManager(BaseWidget &widget) : BaseManager() {
	if (!first_manager) {
		first_manager = std::make_shared<BaseManager>(*this);
	}

	widget.manager = this;
	widgets->push_back(&widget);
}

bool BaseManager::process_events() const {

	// widgets
	auto widgets_open = 0;
	for (auto &widget : *widgets) {
		if (widget->sfwindow->isOpen()) {
			++widgets_open;
			sf::Event ev;
			while (widget->sfwindow->pollEvent(ev)) {
				if (ev.type == sf::Event::Closed || ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
					widget->sfwindow->close();
				}
				else if (ev.type == sf::Event::MouseButtonPressed) {
					if (ev.mouseButton.button == sf::Mouse::Left) {
						widget->grabOffset = widget->sfwindow->getPosition() - sf::Mouse::getPosition();
						widget->is_grabbed = true;
					}
				}
				else if (ev.type == sf::Event::MouseButtonReleased) {
					if (ev.mouseButton.button == sf::Mouse::Left) {
						widget->is_grabbed = false;
					}
				}
				else if (ev.type == sf::Event::MouseMoved) {
					if (widget->is_grabbed) {
						widget->sfwindow->setPosition(sf::Mouse::getPosition() + widget->grabOffset);
					}
				}
			}

			widget->update();
		}

	}
	return widgets_open ? true : false;
}

bool BaseManager::run() const {
	return process_events();
}

BaseWidget::BaseWidget() : sfwindow(new sf::RenderWindow(sf::VideoMode(200, 200), "gSplasher", sf::Style::None)) {
	makeWindowTransparent(*sfwindow);
	sfwindow->setVerticalSyncEnabled(true);
};

BaseWidget::BaseWidget(const BaseWidget &other) : BaseWidget() {
}

void BaseWidget::paint() const {
	sf::Font font;
	font.loadFromFile("Quicksand-Regular.otf");
	sf::Text test{"Hello gSplasher!", font};
	sfwindow->draw(test);
}

void BaseWidget::paint(Painter&) const {
	
}

void BaseWidget::update() const {
	Color clear_color{ style.base_color };
	if (is_grabbed) {
		clear_color = style.grabbed_color;
	}
	setWindowAlpha(*sfwindow, 190);
	sfwindow->clear(clear_color);
	paint();
	sfwindow->display();
}
