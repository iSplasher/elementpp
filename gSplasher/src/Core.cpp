#include "../include/Core.h"
#include "../include/Widget.h"

gsp::gApplication* gsp::gApplication::singleton = nullptr;

int gsp::gApplication::run() const {
	while (processEv()) {

	}
	return 0;
}

gsp::gApplication* gsp::gApplication::instance() {
	if (singleton == nullptr) {
		singleton = new gApplication();
	}

	return singleton;
}

bool gsp::gApplication::processEv() const {
	auto widgets_open = 0;
	for (auto widget : *widgets) {

		if (widget->r_window->isOpen()) {
			widget->update();
			++widgets_open;
		}
	}
	return widgets_open ? true : false;
}

void gsp::gApplication::addWidget(gCoreWidget* widget) const {
	widgets->push_back(widget);
}
