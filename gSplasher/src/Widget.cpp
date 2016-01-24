#include "../include/Widget.h"
#include "../include/Event.h"

#include <Windows.h>

USING_NAMESPACE

gCoreWidget::gCoreWidget(gCoreWidget* parent) :
	style(0, 0, 200, 200),
	r_window(new sf::RenderWindow(sf::VideoMode(style.size.width, style.size.height), "gSplasher", sf::Style::None)) {
	r_window->setVerticalSyncEnabled(true);
	setTransparency(style.base_alpha);
	if (parent) {	
		parent_widget = std::make_shared<gCoreWidget>(parent);
	}

	if (parent == nullptr) {
		if (gApp != nullptr) {
			gApp->addWidget(this);
		}
	}
}

void gCoreWidget::update() {
	using SE = sf::Event;
	SE sfev;
	while (r_window->pollEvent(sfev)) {
		std::shared_ptr<gEvent> ev(nullptr);
		switch(sfev.type) {
		case SE::MouseMoved:
		case SE::MouseButtonPressed:
		case SE::MouseButtonReleased:
			ev.reset(new gMouseEvent(sfev));
			break;
		case SE::KeyPressed:
		case SE::KeyReleased:
			ev.reset(new gKeyEvent(sfev));
			break;
		default:
			ev.reset(new gEvent(sfev));
			break;
		}
		gApp->event_manager.dispatchEvent(ev);
	}
	r_window->clear(Color::Transparent);
	//paint();
	r_window->display();
}

void gCoreWidget::event(gCore* sender, EventPtr ev) {
	// TODO: send events to children here
	if (ev->type() == gEvent::MouseMove) {
		printf("Mouse moved");
	}
}

bool gCoreWidget::setTransparency(unsigned char alpha) const {
	auto hWnd = r_window->getSystemHandle();
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}
