#include "include/Core.h"
#include "include/Wrapper.h"
#include "include/Global.h"
#include "include/Widget.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

USING_NAMESPACE

std::shared_ptr<BaseFrame> BaseFrame::first_manager = nullptr;

BaseFrame::BaseFrame() : widgets(std::make_shared<std::vector<BaseWidget*>>()) {
}

BaseFrame::BaseFrame(BaseWidget &widget) : BaseFrame() {
	if (!first_manager) {
		first_manager = std::make_shared<BaseFrame>(*this);
	}

	widget.manager = this;
	widgets->push_back(&widget);
}

bool BaseFrame::process_events() const {

	// widgets
	auto widgets_open = 0;
	for (auto &widget : *widgets) {
		if (widget->sfwindow->isOpen()) {
			++widgets_open;
			widget->update();
		}

	}
	return widgets_open ? true : false;
}

bool BaseFrame::run() const {
	return process_events();
}

BaseProperties::BaseProperties() {
	base_font.loadFromFile("Quicksand-Regular.otf");
}

BaseProperties::BaseProperties(Color c) : BaseProperties() {
	base_color = c;
}