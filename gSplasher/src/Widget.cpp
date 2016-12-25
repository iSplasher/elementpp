#include "gSplasher/Widget.h"
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Painter.h"


USING_NAMESPACE

CoreWidget::CoreWidget(CoreWidget* parent) : LayoutCore(parent) {
	setObjectName("Widget");
}

CoreWidget::~CoreWidget() {
	// TODO: delete paint context
}

void CoreWidget::paint(Painter& painter) {
}

void CoreWidget::update() {
	auto &painter = *parent_window->painter;
	painter.save();
	painter.origin = gPointF(mapToWindow(gPoint(0, 0)));
	painter.current_widget = this;
	paint(painter);
	painter.restore();
	updateChildren();
}

void CoreWidget::event(EventPtr ev) {

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

gPoint CoreWidget::pos() const {

	if (is_widget && !parent_widget) {
		return gPoint();
	}
	return LayoutCore::pos();
}

void CoreWidget::setParent(CoreWidget* new_parent) {
	parent_widget = new_parent;
	move(0, 0);
	if (new_parent) {
		parent_window = new_parent->parent_window;
	}
	else {
		parent_window = nullptr;
	}
	LayoutCore::setParent(new_parent);
}

void CoreWidget::setLayout(Layout& new_layout) {
	new_layout.setWigdet(this);
}

gPoint CoreWidget::mapToParent(gPoint _p) const {
	return _p + pos();
}

gPoint CoreWidget::mapFromParent(gPoint _p) const {
	return _p - pos();
}

gPoint CoreWidget::mapFromGlobal(gPoint p) const {
	auto w = this;

	while (w) {
		p -= w->pos();
		w = w->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

gPoint CoreWidget::mapToGlobal(gPoint p) const {
	auto w = this;

	while (w) {
		p += w->pos();
		w = w->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

gPoint CoreWidget::mapFromWindow(gPoint p) const {
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

gPoint CoreWidget::mapToWindow(gPoint p) const {
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

void CoreWidget::mousePressEvent(MouseEventPtr ev) {
	if (drag.is_draggable && flags(ev->button & MouseButton::Left) ) {
		move_state = MoveState::Moving;
		drag.start_mouse_pos = mapToGlobal(ev->pos);
		drag.start_pos = pos();
	}

	ev->ignore();
}

void CoreWidget::mouseMoveEvent(MouseEventPtr ev) {
	if (ev->pos.x >= 0 && ev->pos.x < size().width && ev->pos.y >= 0 && ev->pos.y < size().height) {
		under_mouse = true;
	} else {
		under_mouse = false;
	}

	if (drag.is_draggable && move_state == MoveState::Moving) {
		move(drag.start_pos+(mapToGlobal(ev->pos) - drag.start_mouse_pos));
	}

	ev->ignore();
}

void CoreWidget::mouseReleaseEvent(MouseEventPtr ev) {
	move_state = MoveState::Normal;
	ev->ignore();
}

void CoreWidget::updateChildren() {
	for (auto &c : children()) {
		if (c->is_widget) {
			static_cast<CoreWidget*>(c)->update();
		}
	}
}
