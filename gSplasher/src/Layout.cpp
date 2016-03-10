#include "gSplasher/Layout.h"
#include "gSplasher/Widget.h"

#include "LayoutImpl.h"


USING_NAMESPACE

gLayoutable::gLayoutable(gLayoutable* parent): gCore(parent) {
	c_data = std::make_shared<priv::ItemConstraints>();
}

gLayoutable::~gLayoutable() {}

void gLayoutable::setFixedWidth(int width) {
	c_data->fixed_width = true;
	c_data->width.change_value(width);
	auto l = layout();
	if (l) {
		l->invalidate();
	}
}

void gLayoutable::setFixedHeight(int height) {
	c_data->fixed_height = true;
	c_data->height.change_value(height);
	auto l = layout();
	if (l) {
		l->invalidate();
	}
}

gCoreWidget* gLayoutable::parentWidget() const {
	auto core_p = parentCore();
	return core_p ? core_p->is_widget || core_p->is_window ? static_cast<gCoreWidget*>(core_p) : nullptr : nullptr;
}

gPoint gLayoutable::pos() const {
	return gPoint(c_data->x.int_value(), c_data->y.int_value());
}

void gLayoutable::move(gPoint new_p) {
	c_data->x.change_value(new_p.x);
	c_data->y.change_value(new_p.y);
}

void gLayoutable::resize(gSize new_s) {
	if (!layout()) {
		c_data->width.change_value(new_s.width);
		c_data->height.change_value(new_s.height);
	}
}

gSize gLayoutable::size() const {
	return gSize(c_data->width.int_value(), c_data->height.int_value());
}

gLayout::gLayout(gCoreWidget* parent) : gLayoutable(parent) {
	layouter = std::make_shared<priv::LayoutImpl>(this);
	if (parent) {
		setWigdet(parent);
	}
}

void gLayout::setWigdet(gCoreWidget* new_parent) {
	layouter->setWidget(new_parent);
}

void gLayout::add(gLayoutable* item, Alignment align) {
	item->containing_layout = this;
	item->setParent(parentWidget());
	layouter->addItem(item);
	invalidate();
}

void gLayout::setItemFixedWidth(priv::ItemData& data, int new_value) {
	data->width.change_value(new_value);

	// TODO: store the fixed constraints somewhere! Maybe in Map container?
	auto solver = layouter;
}
