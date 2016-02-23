#include "gSplasher/Layout.h"
#include "gSplasher/Widget.h"

#include "LayoutImpl.h"


USING_NAMESPACE

gLayoutable::gLayoutable(gLayoutable* parent): gCore(parent) {
}

void gLayoutable::move(gPoint new_p) {
	c_topleft = new_p;
}

void gLayoutable::resize(gSize new_s) {
	_size = new_s;
}

gLayout::gLayout(gCoreWidget* parent) : gLayoutable(parent) {
	l_impl = std::make_shared<gLayoutImpl>(this);
	if (parent) {
		setWigdet(parent);
	}
}

void gLayout::setWigdet(gCoreWidget* new_parent) {
	layoutitem_parent = new_parent;
	resize(new_parent->size());
	move(new_parent->pos());
}

void gLayout::add(gLayoutable* item, Alignment align) {
	item->layoutitem_parent = this;
}

//gSize gLayout::prefferedSize() const {
//	// TODO: add proper margins;
//	return m_parent->size() - 2;
//}

