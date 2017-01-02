﻿#pragma once

#include "gSplasher/Widget.h"

NAMESPACE_BEGIN

class TopBar : public WidgetCore {
public:
	TopBar(Window *w = nullptr);
	~TopBar();

	void paint(Painter &painter) override;
	void update() override;

private:
	void setWindow(Window *w);
	friend class Window;
};

NAMESPACE_END
