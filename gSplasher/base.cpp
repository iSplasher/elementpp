#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "include/Base.h"
#include "include/Wrapper.h"
#include "include/Global.h"

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

Region::Region(const Region& other) {
	
}

Region::Point Region::startPoint() const {
	return path.empty() ? Point(0, 0) : path[0];
}

Region::Point Region::endPoint() const {
	return path.empty() ? Point(0, 0) : path[path.size()-1];
}

const Region::iterator Region::begin() {
	return path.begin();
}

const Region::iterator Region::end() {
	return path.end();
}

bool Region::addPoint(std::initializer_list<Point> p) {
	auto status = false;
	for (auto x : p) {
		if (x.x > Desktop::WIDTH || x.y > Desktop::HEIGHT) {
			status = true;
		}
		path.push_back(x);
	}
	return status;
}

bool Region::addPoint(Point p) {
	
}

bool BaseManager::run() const {
	return process_events();
}

WidgetStyle::WidgetStyle() : BaseStyle() {
	height = 0;
	width = 0;
}

BaseWidget::BaseWidget() : sfwindow(new sf::RenderWindow(sf::VideoMode(200, 200), "gSplasher", sf::Style::None)) {
	sfwindow->setVerticalSyncEnabled(true);
	sf::Image bg_img;
	bg_img.loadFromFile("image.png");
	setShape(sfwindow->getSystemHandle(), bg_img);
	setTransparency(sfwindow->getSystemHandle(), base_alpha);
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
	//setWindowAlpha(*sfwindow, 190);
	sfwindow->clear(sf::Color::Transparent);
	paint();
	sfwindow->display();
}

bool BaseWidget::setShape(HWND hWnd, const sf::Image& image) {
	const sf::Uint8* pixelData = image.getPixelsPtr();
	HRGN hRegion = CreateRectRgn(0, 0, image.getSize().x, image.getSize().y);

	// Determine the visible region
	for (unsigned int y = 0; y < image.getSize().y; y++) {
		for (unsigned int x = 0; x < image.getSize().x; x++) {
			if (pixelData[y * image.getSize().x * 4 + x * 4 + 3] == 0) {
				HRGN hRegionPixel = CreateRectRgn(x, y, x + 1, y + 1);
				CombineRgn(hRegion, hRegion, hRegionPixel, RGN_XOR);
				DeleteObject(hRegionPixel);
			}
		}
	}

	SetWindowRgn(hWnd, hRegion, true);
	DeleteObject(hRegion);
	return true;
}

bool BaseWidget::setTransparency(HWND hWnd, unsigned char alpha) {
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}