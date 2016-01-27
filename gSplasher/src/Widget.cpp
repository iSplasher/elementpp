#include "../include/Widget.h"
#include "../include/Event.h"

#include <Windows.h>

USING_NAMESPACE

gCoreWidget::gCoreWidget(gCoreWidget* parent) :
	gCore(parent),
	style(0, 0, 200, 200) {
	is_widget = true;
	parent_widget = parent;
}

void gCoreWidget::update() {
}

void gCoreWidget::event(EventPtr ev) {
	// TODO: send events to children here
	switch (ev->type()) {
	case gEvent::MouseMove:
		mouseMoveEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	case gEvent::MouseButtonPress:
		mousePressEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	case gEvent::MouseButtonRelease:
		mouseReleaseEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	}
}

Point gCoreWidget::pos() {
	return Point(0, 0);
}

void gCoreWidget::move(Point new_p) {
}

void gCoreWidget::resize(Size new_s) {
}

Point gCoreWidget::mapToGlobal(Point p) {
	auto c_pos = pos();
	Point n_pos(c_pos.x + p.x, c_pos.y + p.y);
	return n_pos;
}

void gCoreWidget::mousePressEvent(MouseEventPtr ev){
	printf("A button was pressed!");
	move_state = MoveState::Moving;
	move_offset = ev->pos();
}

void gCoreWidget::mouseMoveEvent(MouseEventPtr ev) {
	printf("x=%d y=%d\n", ev->x, ev->y);
	if (move_state == MoveState::Moving) {
		move(mapToGlobal(ev->pos()) - move_offset);
	}
}

void gCoreWidget::mouseReleaseEvent(MouseEventPtr ev) {
	printf("A button was released!");
	move_state = MoveState::Normal;
}

gWindow::gWindow(gWindow* parent) :
	gCoreWidget(parent),
	r_window(new sf::RenderWindow(sf::VideoMode(style.size.width, style.size.height), "gSplasher", sf::Style::None)) {
	
	r_window->setVerticalSyncEnabled(true);
	setTransparency(style.base_alpha);
	is_widget = false;
	is_window = true;
}

void gWindow::update() {
	using SE = sf::Event;
	SE sfev;
	while (r_window->pollEvent(sfev)) {
		EventPtr ev(nullptr);
		switch (sfev.type) {
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
		gApp->event_manager.dispatchEvent(this, ev);
	}
	r_window->clear(style.bg_color);
	//paint();
	r_window->display();
}

Point gWindow::pos() {
	return r_window->getPosition();
}

void gWindow::move(Point new_p) {
	r_window->setPosition(new_p);
}

void gWindow::resize(Size new_s) {
	r_window->setSize(new_s);
}

bool gWindow::setTransparency(unsigned char alpha) const {
	auto hWnd = r_window->getSystemHandle();
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}
