#pragma once

#include "gSplasher/Widget.h"

NAMESPACE_BEGIN

class gTopBar : public gCoreWidget {
public:
	gTopBar();
	~gTopBar();

	void paint(gPainter &painter) override;
	void update();

private:
	void setWindow(gWindow *w);
	friend gWindow;
};

NAMESPACE_END
