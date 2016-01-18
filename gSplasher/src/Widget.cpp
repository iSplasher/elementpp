#include "include/Widget.h"

USING_NAMESPACE

WidgetProperties::WidgetProperties() : BaseProperties() {
	size.setWidth(500);
	size.setHeight(500);
}

WidgetProperties::WidgetProperties(Color bg_color, Color fg_color, Color grab_color) : WidgetProperties() {
	base_color = bg_color;
	foreground_color = fg_color;
	grabbed_color = grab_color;
}

BaseWidget::BaseWidget(BaseWidget *_parent) :
sfwindow(new sf::RenderWindow(sf::VideoMode(style.size.width(), style.size.height()), "gSplasher", sf::Style::None)) {
	parent = _parent;
	sfwindow->setVerticalSyncEnabled(true);
	setTransparency(sfwindow->getSystemHandle(), style.base_alpha);
	style.is_widget = _parent == nullptr ? false : true;
	style.is_window = _parent == nullptr ? true : false;
	setShape();
}

BaseWidget::BaseWidget(const BaseWidget &other) : BaseWidget() {
}

// TODO: this
BaseWidget::BaseWidget(const WidgetProperties s, BaseWidget *_parent) : BaseWidget(_parent) {
	style = s;
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
					style.size += 10;
					sfwindow->setSize(sf::Vector2u(style.size.width(), style.size.height()));
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

void BaseWidget::setLayout(Layout &layout) {
	layout.setContentsRect(contentsRect());
	layout.changeLayoutableParent(this);
	layout.update();
}

void BaseWidget::paint() {
	paint(painter, style);
}

void BaseWidget::paint(Painter& p, WidgetProperties _style) {

}

// Windows only implementation for now
bool BaseWidget::setShape() {
	//if (sfwindow != nullptr) {
	//HRGN hRegion = CreateRoundRectRgn(0, 0, style.width, style.height, 25, 25);

	//SetWindowRgn(sfwindow->getSystemHandle(), hRegion, true);
	//DeleteObject(hRegion);
	//return true;
	//}
	return false;
}

// Windows only implementation for now
bool BaseWidget::setTransparency(HWND hWnd, unsigned char alpha) {
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}