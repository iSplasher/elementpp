#include "../include/Widget.h"

gsp::gCoreWidget::gCoreWidget(gCoreWidget* parent) :
	style(Size(200, 200)),
	r_window(new sf::RenderWindow(sf::VideoMode(style.size.width, style.size.height), "gSplasher", sf::Style::None)) {
	r_window->setVerticalSyncEnabled(true);
	setTransparency(style.base_alpha);
	if (parent) {	
		parent_widget = std::make_shared<gCoreWidget>(parent);
	}

	if (parent == nullptr) {
		auto app = gsp::gApplication::instance();
		app->addWidget(this);
	}
}

void gsp::gCoreWidget::update() {
	r_window->clear(Color::Transparent);
	//paint();
	r_window->display();
}

bool gsp::gCoreWidget::setTransparency(unsigned char alpha) const {
	HWND hWnd = r_window->getSystemHandle();
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}
