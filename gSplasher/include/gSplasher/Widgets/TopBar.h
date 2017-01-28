#pragma once

#include "gSplasher/Widget.h"

NAMESPACE_BEGIN

class TopBar : public WidgetCore {
public:
	TopBar(RWindow *w = nullptr);
	~TopBar();

	void paint(Painter &painter) override;
	void update() override;

private:
	void setWindow(RWindow *w);
	friend class RWindow;
};

NAMESPACE_END
