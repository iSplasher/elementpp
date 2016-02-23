#pragma once

#include "gSplasher/Layout.h"

NAMESPACE_BEGIN

template<Orientation O>
class GSPLASHER_API gBoxLayout : public gLayout {
public:
	gBoxLayout(gCoreWidget *parent) : gLayout(parent) {}
	
	void add(gLayoutable *item, Alignment align=Alignment::HCenter|Alignment::VCenter) override;
	void invalidate();
};

NAMESPACE_END
