#include "gSplasher/Widgets/TopBar.h"
#include "gSplasher/Utils/Painter.h"
#include "gSplasher/Window.h"

USING_NAMESPACE

gTopBar::gTopBar(): gCoreWidget() {
	setFixedWidth(0);
	setFixedHeight(0);
}

gTopBar::~gTopBar() {
}

void gTopBar::paint(gPainter &painter) {
	gPen p(painter);
	gBrush b(painter);
	p.setColor(gColor(178, 178, 178));
	p.setWidth(3);
	b.setColor(gColor(250, 250, 250));
	painter.drawRect(gRect(0, 0, size()));
}

void gTopBar::update() {
	if (parent_window->painter) {
		auto &painter = *parent_window->painter;
		auto x = painter.top_margin;
		painter.top_margin = 0;
		painter.save();
		paint(painter);
		painter.restore();
		painter.top_margin = x;
	}
}

void gTopBar::event(EventPtr ev) {
	std::cout << "Topbar event received!\n";
}

void gTopBar::setWindow(gWindow* w) {
	parent_window = w;
	setFixedWidth(w->size().width);
	setFixedHeight(20);
}
