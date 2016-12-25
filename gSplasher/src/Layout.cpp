#include "gSplasher/Layout.h"
#include "gSplasher/Widget.h"

#include <yoga/Yoga.h>


USING_NAMESPACE
USING_NAMESPACE_PRIV


LayoutCore::LayoutCore(LayoutCore* parent) : Core(parent)
{
	Core::setParent(parent);
	if (parent && parent->is_layout)
	{
		static_cast<Layout*>(parent)->appendItem(this);
	}
}

LayoutCore::~LayoutCore()
{
}


Layout::Layout(LayoutCore* parent) : LayoutCore(parent)
{
	is_layout = true;
	node = YGNodeNew();
	if (parent && !parent->is_layout)
	{
		this->setWigdet(static_cast<CoreWidget*>(parent));
	}
	setObjectName("Layout");
}

void Layout::setWigdet(CoreWidget* new_parent) {
	if (!new_parent->layout()) {
		new_parent->playout = this;
		setParent(new_parent);
		new_parent->bound_layout = this;
	}
	else {
		std::cout << "This wigdet is already handled by a layout\n"; // TODO: log
	}
}

void Layout::appendItem(LayoutCore* item, Alignment align)
{
	if (item->layout() == this)
		return; // TODO: maybe warn?

	if (item->layout())
		item->layout()->takeItem(item);

	if (!item->node)
	{
		item->node = YGNodeNew();
	}

	auto n = YGNodeGetChildCount(node);
	YGNodeInsertChild(node, item->node, n);
	nodemap.insert({item->node, item});

	// TODO
	switch (align)
	{
	}

	invalidate();
}

void Layout::takeItem(LayoutCore* item)
{
	if (item->layout() == this)
	{
		YGNodeRemoveChild(node, item->node);
		nodemap.erase(item->node);
		item->playout = nullptr;
	}
}

void Layout::invalidate() {
	if (static_cast<LayoutCore*>(parentCore())->bound_layout == this)
	{
		
	} else
	{
		YGNodeCalculateLayout(node, YGUndefined, YGUndefined, YGUndefined);
	}
}

void Layout::event(EventPtr ev) {
	switch (ev->type()) {
	case Event::Type::Layout:
		std::cout << "invalidate event" << std::endl;
		invalidate();
		break;
	}
	LayoutCore::event(ev);
}

