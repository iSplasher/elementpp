#pragma once

#include "gSplasher/Widget.h"

NAMESPACE_BEGIN

class gTopBar : public gCoreWidget {
public:
	explicit gTopBar(gCoreWidget *p_window);

	void paint(gPainter &painter) override;
};

NAMESPACE_END
