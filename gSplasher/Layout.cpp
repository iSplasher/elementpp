#include "include/Layout.h"
#include "include/Widget.h"

USING_NAMESPACE

Layout::Layout(BaseWidget *p) {
	if (!p) {
		return;
	}
	p->setLayout(*this);
}

void Layout::add(Layoutable &item) {
	
}

void Layout::update() {
	// TODO: do the calculations here
	for (auto m : *layout_members) {
		m->update();
	}
}

void Layout::changeLayoutableParent(const Layoutable *p) {
	m_parent = p;
}
