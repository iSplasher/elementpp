#pragma once

#include "gSplasher/Widget.h"

NAMESPACE_BEGIN

class gTopBar : public CoreWidget {
public:
	gTopBar();
	~gTopBar();

	void paint(Painter &painter) override;
	void update();

private:
	void setWindow(gWindow *w);
	friend gWindow;
};

NAMESPACE_END
