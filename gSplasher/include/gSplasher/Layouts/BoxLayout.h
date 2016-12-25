#pragma once

#include "gSplasher/Layout.h"

NAMESPACE_BEGIN

template<Orientation O>
class GSPLASHER_API gBoxLayout : public Layout {
public:
	gBoxLayout(CoreWidget *parent = nullptr) : Layout(parent) {}

	void add(LayoutCore *item, Alignment align = Alignment::HCenter | Alignment::VCenter) override;
	void invalidate();
};

NAMESPACE_END
