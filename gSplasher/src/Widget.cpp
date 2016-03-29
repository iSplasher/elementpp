#include "gSplasher/Widget.h"
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Painter.h"


USING_NAMESPACE

gCoreWidget::gCoreWidget(gCoreWidget* parent) : gLayoutable(parent) {
	is_widget = true;
	setParent(parent);
	setObjectName("gWidget");
}

gCoreWidget::~gCoreWidget() {
	// TODO: delete paint context
}

void gCoreWidget::paint(gPainter& painter) {
}

void gCoreWidget::update() {
	auto &painter = *parent_window->painter;
	painter.save();
	painter.origin = gPointF(mapToWindow(gPoint(0, 0)));
	painter.current_widget = this;
	paint(painter);
	painter.restore();
	updateChildren();
}

void gCoreWidget::event(EventPtr ev) {
	switch (ev->type()) {
	case gEvent::Type::MouseMove:
		mouseMoveEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	case gEvent::Type::MouseButtonPress:
		mousePressEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	case gEvent::Type::MouseButtonRelease:
		mouseReleaseEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	}

	gLayoutable::event(ev);
}

gPoint gCoreWidget::pos() const {

	if (is_widget && !parent_widget) {
		return gPoint();
	}
	return gLayoutable::pos();
}

void gCoreWidget::setParent(gCoreWidget* new_parent) {
	parent_widget = new_parent;
	move(0, 0);
	if (new_parent) {
		parent_window = new_parent->parent_window;
	}
	else {
		parent_window = nullptr;
	}
	gLayoutable::setParent(new_parent);
}

void gCoreWidget::setLayout(gLayout& new_layout) {
	new_layout.setWigdet(this);
}

gPoint gCoreWidget::mapToParent(gPoint _p) const {
	return _p + pos();
}

gPoint gCoreWidget::mapFromParent(gPoint _p) const {
	return _p - pos();
}

gPoint gCoreWidget::mapFromGlobal(gPoint p) const {
	auto w = this;

	while (w) {
		p -= w->pos();
		w = w->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

gPoint gCoreWidget::mapToGlobal(gPoint p) const {
	auto w = this;

	while (w) {
		p += w->pos();
		w = w->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

gPoint gCoreWidget::mapFromWindow(gPoint p) const {
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

gPoint gCoreWidget::mapToWindow(gPoint p) const {
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

void gCoreWidget::mousePressEvent(MouseEventPtr ev) {
	if (drag.is_draggable && flags(ev->button & MouseButton::Left) ) {
		move_state = MoveState::Moving;
		drag.start_mouse_pos = mapToGlobal(ev->pos);
		drag.start_pos = pos();
	}
}

void gCoreWidget::mouseMoveEvent(MouseEventPtr ev) {
	if (ev->pos.x >= 0 && ev->pos.x < size().width && ev->pos.y >= 0 && ev->pos.y < size().height) {
		under_mouse = true;
	} else {
		under_mouse = false;
	}

	if (drag.is_draggable && move_state == MoveState::Moving) {
		move(drag.start_pos+(mapToGlobal(ev->pos) - drag.start_mouse_pos));
	}
}

void gCoreWidget::mouseReleaseEvent(MouseEventPtr ev) {
	move_state = MoveState::Normal;
}

void gCoreWidget::updateChildren() {
	for (auto &c : children()) {
		if (c->is_widget) {
			static_cast<gCoreWidget*>(c)->update();
		}
	}
}

