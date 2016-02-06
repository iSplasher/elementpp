#pragma once

#include "gSplasher/Widget.h"

NAMESPACE_BEGIN

class GSPLASHER_API gPainter {
public:
	gPainter(gCoreWidget*);

	/// <summary>
	/// Begin painting
	/// </summary>
	void begin() const;

	/// <summary>
	/// End painting
	/// </summary>
	void end() const;

private:
	gCoreWidget *w = nullptr;

};

NAMESPACE_END