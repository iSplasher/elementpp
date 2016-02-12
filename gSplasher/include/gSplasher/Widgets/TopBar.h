#pragma once

#include "gSplasher/Widget.h"

NAMESPACE_BEGIN

class gTopBar : public gCoreWidget {
public:
	explicit gTopBar(gWindow *p_window);
	~gTopBar();

	void paint(gPainter &painter);
};

NAMESPACE_END
