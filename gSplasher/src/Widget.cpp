#include "../include/Widget.h"
#include "../include/Event.h"

#include <Windows.h>

USING_NAMESPACE

gCoreWidget::gCoreWidget(gCoreWidget* parent) :
	gCore(parent),
	style(0, 0, 200, 200),
	r_window(new sf::RenderWindow(sf::VideoMode(style.size.width, style.size.height), "gSplasher", sf::Style::None)) {
	is_widget = true;
	r_window->setVerticalSyncEnabled(true);
	setTransparency(style.base_alpha);
	if (parent) {	
		parent_widget = parent;
	}
}

void gCoreWidget::update() {
	using SE = sf::Event;
	SE sfev;
	while (r_window->pollEvent(sfev)) {
		EventPtr ev(nullptr);
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

void gCoreWidget::event(EventPtr ev) {
	// TODO: send events to children here
	switch (ev->type()) {
	case gEvent::MouseMove:
		mouseMoveEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	}
}

bool gCoreWidget::setTransparency(unsigned char alpha) const {
	auto hWnd = r_window->getSystemHandle();
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}

void gCoreWidget::mousePressEvent(MouseEventPtr ev){
}

void gCoreWidget::mouseMoveEvent(MouseEventPtr ev) {
	printf("x=%d y=%d\n", ev->x, ev->y);
}

void gCoreWidget::mouseReleaseEvent(MouseEventPtr ev) {
}
