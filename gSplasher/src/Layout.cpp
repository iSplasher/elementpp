#include "gSplasher/Layout.h"
#include "gSplasher/Widget.h"

#include "LayoutImpl.h"


USING_NAMESPACE

gLayoutable::gLayoutable(gLayoutable* parent): gCore(parent) {
	c_data = std::make_shared<priv::ItemConstraints>();
}

gLayoutable::~gLayoutable() {}

gPoint gLayoutable::pos() const {
	return gPoint(c_data->x.int_value(), c_data->y.int_value());
}

void gLayoutable::move(gPoint new_p) {
	c_data->x.change_value(new_p.x);
	c_data->y.change_value(new_p.y);
}

void gLayoutable::resize(gSize new_s) {
	c_data->width.change_value(new_s.width);
	c_data->height.change_value(new_s.height);
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
	layoutitem_parent = new_parent;
	layouter->setWidget(new_parent);
}

void gLayout::add(gLayoutable* item, Alignment align) {
	item->layoutitem_parent = this;
	layouter->addItem(item);
	invalidate();
}

//gSize gLayout::prefferedSize() const {
//	// TODO: add proper margins;
//	return m_parent->size() - 2;
//}

