#include "gSplasher/Layout.h"
#include "gSplasher/Widget.h"

#include "LayoutImpl.h"


USING_NAMESPACE

gPoint gLayoutable::mapToParent(gPoint p) const {
	return layoutitem_parent ? p + pos() : p;
}

gPoint gLayoutable::mapFromParent(gPoint p) const {
	return layoutitem_parent ? p - pos() : p;
}

void gLayoutable::move(gPoint new_p) {
	//if (is_widget && parent_widget) {
	//	p = mapToParent(new_p);
	//} else {
	p = new_p;
	//}
}

void gLayoutable::resize(gSize new_s) {
	_size = new_s;
}

gLayout::gLayout(gCoreWidget* parent) : gLayoutable() {
	m_parent = parent;
	l_impl = std::make_shared<gLayoutImpl>(this);
	setWigdet(parent);
}

void gLayout::setWigdet(gCoreWidget* new_parent) {
	layoutitem_parent = new_parent;
	resize(new_parent->size());
	move(new_parent->pos());
}

void gLayout::add(gLayoutable* item) {
	item->layoutitem_parent = this;
}

//gSize gLayout::prefferedSize() const {
//	// TODO: add proper margins;
//	return m_parent->size() - 2;
//}

