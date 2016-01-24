#include "../include/Widget.h"
#include "../include/Event.h"

USING_NAMESPACE

gCoreWidget::gCoreWidget(gCoreWidget* parent) :
	style(0, 0, 200, 200),
	r_window(new sf::RenderWindow(sf::VideoMode(style.size.width, style.size.height), "gSplasher", sf::Style::None)) {
	r_window->setVerticalSyncEnabled(true);
	setTransparency(style.base_alpha);
	if (parent) {	
		parent_widget = std::make_shared<gCoreWidget>(parent);
	}

	if (parent == nullptr) {
		if (gApp != nullptr) {
			gApp->addWidget(this);
		}
	}
}

void gCoreWidget::update() {
	sf::Event ev;
	while (r_window->pollEvent(ev)) {
		gEvent g_ev(ev);
		// TODO: dispatch event here
	}
	r_window->clear(Color::Transparent);
	//paint();
	r_window->display();
}

bool gCoreWidget::setTransparency(unsigned char alpha) const {
	auto hWnd = r_window->getSystemHandle();
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}
