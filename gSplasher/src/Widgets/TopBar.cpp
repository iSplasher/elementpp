#include "gSplasher/Widgets/TopBar.h"
#include "gSplasher/Utils/Painter.h"

USING_NAMESPACE


gTopBar::gTopBar(gCoreWidget* p_window) : gCoreWidget(p_window) {

	resize(p_window->size().width, 20);
}

void gTopBar::paint(gPainter& painter) {
	gPen p(painter);
	gBrush b(painter);
	p.setColor(gColor(178, 178, 178));
	p.setWidth(3);
	b.setColor(gColor(250, 250, 250));

	painter.drawRect(gRect(1, 1, gSizeT<int>(size())));
}
