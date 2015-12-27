#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "include/Core.h"
#include "include/Wrapper.h"
#include "include/Global.h"

using namespace gspl;

std::shared_ptr<BaseFrame> BaseFrame::first_manager = nullptr;

BaseFrame::BaseFrame() : widgets(std::make_shared<std::vector<BaseWidget*>>()) {
}

BaseFrame::BaseFrame(BaseWidget &widget) : BaseFrame() {
	if (!first_manager) {
		first_manager = std::make_shared<BaseFrame>(*this);
	}

	widget.manager = this;
	widgets->push_back(&widget);
}

bool BaseFrame::process_events() const {

	// widgets
	auto widgets_open = 0;
	for (auto &widget : *widgets) {
		if (widget->sfwindow->isOpen()) {
			++widgets_open;
			widget->update();
		}

	}
	return widgets_open ? true : false;
}

bool BaseFrame::run() const {
	return process_events();
}

BaseStyle::BaseStyle() {
	base_font.loadFromFile("Quicksand-Regular.otf");
}

BaseStyle::BaseStyle(Color c): BaseStyle() {
	base_color = c;
}

WidgetStyle::WidgetStyle() : BaseStyle() {
	height = 500;
	width = 500;
}

WidgetStyle::WidgetStyle(Color bg_color, Color fg_color, Color grab_color): WidgetStyle() {
	base_color = bg_color;
	foreground_color = fg_color;
	grabbed_color = grab_color;
}

BaseWidget::BaseWidget(BaseWidget *_parent) : 
sfwindow(new sf::RenderWindow(sf::VideoMode(style.width, style.height), "gSplasher", sf::Style::None)) {
	parent = _parent;
	sfwindow->setVerticalSyncEnabled(true);
	sf::Image bg_img;
	setTransparency(sfwindow->getSystemHandle(), base_alpha);
	is_widget = _parent == nullptr ? false : true;
	is_window = _parent == nullptr ? true : false;
	setShape();
}

BaseWidget::BaseWidget(const BaseWidget &other) : BaseWidget() {
}

// TODO: this
BaseWidget::BaseWidget(const WidgetStyle s, BaseWidget *_parent) : BaseWidget(_parent) {
	style = s;
}

void BaseWidget::paint() const {
	sf::Text test{"Hello gSplasher!", style.base_font};
	sfwindow->draw(test);
}

void BaseWidget::paint(Painter&) const {
	
}

void BaseWidget::update() {
	event();
	Color clear_color{ style.base_color };
	if (is_grabbed) {
		clear_color = style.grabbed_color;
	}
	//setWindowAlpha(*sfwindow, 190);
	sfwindow->clear(sf::Color::Transparent);
	paint();
	sfwindow->display();
}

void BaseWidget::event(sf::Event ev) {
	if (sfwindow != nullptr) {
		while (sfwindow->pollEvent(ev)) {
			if (ev.type == sf::Event::Closed || ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
				sfwindow->close();
			}
			else if (ev.type == sf::Event::MouseButtonPressed) {
				if (ev.mouseButton.button == sf::Mouse::Left) {
					grabOffset = sfwindow->getPosition() - sf::Mouse::getPosition();
					is_grabbed = true;
					style.width += 10;
					style.height += 10;
					sfwindow->setSize(sf::Vector2u(style.width, style.height));
				}
			}
			else if (ev.type == sf::Event::MouseButtonReleased) {
				if (ev.mouseButton.button == sf::Mouse::Left) {
					is_grabbed = false;
				}
			}
			else if (ev.type == sf::Event::MouseMoved) {
				if (is_grabbed) {
					sfwindow->setPosition(sf::Mouse::getPosition() + grabOffset);
				}
			}
		}
	}
}

// Windows only implementation for now
bool BaseWidget::setShape() {
	if (sfwindow != nullptr) {
	HRGN hRegion = CreateRoundRectRgn(0, 0, style.width, style.height, 25, 25);

	SetWindowRgn(sfwindow->getSystemHandle(), hRegion, true);
	DeleteObject(hRegion);
	return true;
	}
	return false;
}

// Windows only implementation for now
bool BaseWidget::setTransparency(HWND hWnd, unsigned char alpha) {
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}