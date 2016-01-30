#include "../include/Widget.h"
#include "../include/Event.h"
#include "platform/WinImpl.h"

#include <iostream>

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
	style.size = new_s;
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
	resize(style.size.width+5, style.size.height+5);
}

void gCoreWidget::mouseMoveEvent(MouseEventPtr ev) {
	//printf("x=%d y=%d\n", ev->global_x, ev->global_y);
	if (ev->x >= 0 && ev->x < width() && ev->y >= 0 && ev->y < height()) {
		under_mouse = true;
	} else {
		under_mouse = false;
	}

	if (move_state == MoveState::Moving) {
		move(mapToGlobal(ev->pos()) - move_offset);
	}

	if (underMouse()) {
		style.bg_color = Color::Green;
	} else {
		style.bg_color = Color::Transparent;
	}
}

void gCoreWidget::mouseReleaseEvent(MouseEventPtr ev) {
	printf("A button was released!");
	move_state = MoveState::Normal;
}

gWindow::gWindow(gWindow* parent) :
	gCoreWidget(parent) {
	WinImpl win_impl;
	gWindowHandle handle = win_impl.createHandle(
		style.pos.x,
		style.pos.y,
		style.size.width,
		style.size.height);

	r_window.reset(new sf::RenderWindow(handle));
	r_window->setVerticalSyncEnabled(true);
	win_impl.setTransparency(handle, style.base_alpha);
	win_impl.redraw(
		handle,
		style.pos.x,
		style.pos.y,
		style.size.width,
		style.size.height);
	is_widget = false;
	is_window = true;
}

gWindow::~gWindow() {
}

void gWindow::update() {
	WinImpl::processEvents();
	using SE = sf::Event;
	SE sfev;
	while (r_window->pollEvent(sfev)) {
		EventPtr ev(nullptr);
		switch (sfev.type) {
		case SE::MouseButtonPressed:
		case SE::MouseButtonReleased:
			ev.reset(new gMouseEvent(sfev));
			break;
		case SE::KeyPressed:
		case SE::KeyReleased:
			ev.reset(new gKeyEvent(sfev));
			break;
		default:
			goto continue_exec;
			ev.reset(new gEvent(sfev));
			break;
		}
		gApp->dispatchEvent(this, ev);
	}

continue_exec:
	generateMouseMove();
	r_window->clear(style.bg_color);
	//paint();
	r_window->display();
}

Point gWindow::pos() {
	return r_window->getPosition();
}

void gWindow::move(Point new_p) {
	r_window->setPosition(new_p);
	style.pos = new_p;
}

void gWindow::resize(Size new_s) {
	r_window->setSize(new_s);
	style.size = new_s;
}

void gWindow::generateMouseMove() {
	// TODO: set a class instance switch? enableMouseTracking..?
	if (r_window) {
		Point pos = Mouse::getPosition();
		if (pos.x != _old_mouse_x || pos.y != _old_mouse_y) {
			_old_mouse_x = pos.x;
			_old_mouse_y = pos.y;
			Point r_pos = Mouse::getPosition(*r_window);
			MouseEventPtr m_ev = std::make_shared<gMouseEvent>(gEvent::MouseMove, r_pos);
			gApp->dispatchEvent(this, m_ev); // TODO: make this instantous? 
		}
	}
}

