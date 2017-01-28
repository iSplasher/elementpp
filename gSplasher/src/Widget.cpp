#include "gSplasher/Widget.h"
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Painter.h"


USING_NAMESPACE

WidgetCore::WidgetCore(WidgetCore* parent) : LayoutCore(parent) {
	objectName = "Widget";
}

WidgetCore::~WidgetCore() {
	// TODO: delete paint context
}

void WidgetCore::paint(Painter& painter) {
}

void WidgetCore::update() {
	auto &painter = *parent_window->painter;
	painter.save();
	painter.origin = PointF(mapToWindow(Point(0, 0)));
	painter.current_widget = this;
	paint(painter);
	painter.restore();
}

void WidgetCore::event(EventPtr ev) {

	switch (ev->type()) {
	case Event::Type::MouseMove:
		mouseMoveEvent(std::static_pointer_cast<MouseEvent>(ev));
		break;
	case Event::Type::MouseButtonPress:
		mousePressEvent(std::static_pointer_cast<MouseEvent>(ev));
		break;
	case Event::Type::MouseButtonRelease:
		mouseReleaseEvent(std::static_pointer_cast<MouseEvent>(ev));
		break;
	}

	LayoutCore::event(ev);
}

Point WidgetCore::pos() {

	if (is_widget && !parent_widget) {
		return Point();
	}
	return LayoutCore::pos();
}

void WidgetCore::setParent(WidgetCore* new_parent) {
	parent_widget = new_parent;
	move(0, 0);
	if (new_parent) {
		parent_window = new_parent->parent_window;
	}
	else {
		parent_window = nullptr;
	}
	this->parent = new_parent;
}

void WidgetCore::setLayout(Layout& new_layout) {
	new_layout.setWigdet(this);
}

Point WidgetCore::mapToParent(Point _p) {
	return _p + pos();
}

Point WidgetCore::mapFromParent(Point _p) {
	return _p - pos();
}

Point WidgetCore::mapFromGlobal(Point p) {
	auto w = this;

	while (w) {
		p -= w->pos();
		w = w->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

Point WidgetCore::mapToGlobal(Point p) {
	auto w = this;

	while (w) {
		p += w->pos();
		w = w->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

Point WidgetCore::mapFromWindow(Point p) {
	if (is_window) {
		return p;
	}
	auto w = this;

	while (w) {
		p -= w->pos();
		w = w->parent_widget && w->parent_widget->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

Point WidgetCore::mapToWindow(Point p) {
	if (is_window) {
		return p;
	}
	auto w = this;

	while (w) {
		p += w->pos();
		w = w->parent_widget && w->parent_widget->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

void WidgetCore::mousePressEvent(MouseEventPtr ev) {
	if (drag.is_draggable && flags(ev->button & MouseButton::Left)) {
		move_state = MoveState::Moving;
		drag.start_mouse_pos = mapToGlobal(ev->pos);
		drag.start_pos = pos();
	}

	ev->ignore();
}

void WidgetCore::mouseMoveEvent(MouseEventPtr ev) {
	if (ev->pos.x >= 0 && ev->pos.x < size().width && ev->pos.y >= 0 && ev->pos.y < size().height) {
		under_mouse = true;
	}
	else {
		under_mouse = false;
	}

	if (drag.is_draggable && move_state == MoveState::Moving) {
		move(drag.start_pos + (mapToGlobal(ev->pos) - drag.start_mouse_pos));
	}

	ev->ignore();
}

void WidgetCore::mouseReleaseEvent(MouseEventPtr ev) {
	move_state = MoveState::Normal;
	ev->ignore();
}

