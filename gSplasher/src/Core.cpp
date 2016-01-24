#include "../include/Core.h"
#include "../include/Widget.h"

USING_NAMESPACE

gApplication* gApplication::self = nullptr;

void gCore::event(gCore&, gEvent) {}

gApplication::gApplication() :
	gCore(), widgets(std::make_unique<CoreWidgetList>()),
	event_manager() {
	assert(self == nullptr);
	self = this;
	event_manager.init();
}

int gApplication::run() {
	while (processEv()) {
		event_manager.processEv();
	}
	return 0;
}

gApplication* gApplication::instance() {
	return self;
}

bool gApplication::processEv() const {
	auto widgets_open = 0;
	for (auto widget : *widgets) {

		if (widget->r_window->isOpen()) {
			widget->update();
			++widgets_open;
		}
	}
	return widgets_open ? true : false;
}

void gApplication::addWidget(gCoreWidget* widget) const {
	widgets->push_back(widget);
}
