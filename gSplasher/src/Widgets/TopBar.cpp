#include "gSplasher/Widgets/TopBar.h"
#include "gSplasher/Utils/Painter.h"
#include "gSplasher/Window.h"

USING_NAMESPACE

TopBar::TopBar(Window* w) : WidgetCore(w)
{
	if (w)
		setWindow(w);
}

TopBar::~TopBar() {
}

void TopBar::paint(Painter &painter) {
	gPen p(painter);
	gBrush b(painter);
	p.setColor(gColor(178, 178, 178));
	p.setWidth(3);
	b.setColor(gColor(250, 250, 250));
	painter.drawRect(Rect(0, 0, size()));
}

void TopBar::update()
{
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

void TopBar::setWindow(Window* w) {
	if (w)
	{
		parent_window = w;
		//setFixedWidth(w->size().width);
		//setFixedHeight(20);
		resize(w->size().width, 20);
	}
}
