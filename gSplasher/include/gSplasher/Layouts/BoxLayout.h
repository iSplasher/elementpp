#pragma once

#include "gSplasher/Layout.h"

NAMESPACE_BEGIN

template<Orientation O>
class GSPLASHER_API gBoxLayout : public gLayout {
public:
	gBoxLayout(gCoreWidget *parent) : gLayout(parent) {}
	
	void add(gLayoutable *item);
	void invalidate();
};

NAMESPACE_END
