#include "include/Layout.h"
#include "include/Widget.h"

USING_NAMESPACE

Layout::Layout(BaseWidget *p) {
	if (!p) {
		return;
	}
	p->setLayout(*this);
}

void Layout::update() {
	for (auto m : *layout_members) {
		m->update();
	}
}
