#include "gSplasher/Layout.h"
#include "gSplasher/Widget.h"

#include <yoga/Yoga.h>


USING_NAMESPACE
USING_NAMESPACE_PRIV

YGFlexDirection getOrientation(Orientation o, bool reverse)
{
	switch (o)
	{
	case Orientation::Horizontal:
		return reverse ? YGFlexDirectionRowReverse : YGFlexDirectionRow;
	case Orientation::Vertical:
		return reverse ? YGFlexDirectionColumnReverse : YGFlexDirectionColumn;
	}
}

LayoutCore::LayoutCore(LayoutCore* parent) : Core(parent)
{
	Core::setParent(parent);
	if (parent && parent->is_layout)
	{
		auto l = static_cast<Layout*>(parent);
		l->appendItem(this);
	}
}

LayoutCore::~LayoutCore()
{
}

Point LayoutCore::pos() const
{
	Point p;
	if (node)
	{
		p.x =YGNodeLayoutGetLeft(node);
		p.y = YGNodeLayoutGetTop(node);
	}

	return p;
}

void LayoutCore::move(Point new_p)
{
	// check absolute
}

void LayoutCore::resize(Size new_s)
{
	auto &s = properties.size;
	if (node)
	{
		YGNodeStyleSetWidth(node, s.width);
		YGNodeStyleSetHeight(node, s.height);
		layout()->invalidate();
		s.width = YGNodeLayoutGetWidth(node);
		s.height = YGNodeLayoutGetHeight(node);
		return;
	}
	s = new_s;
}

Size LayoutCore::size() const
{
	return properties.size;
}

Layout::Layout(LayoutCore* parent) : LayoutCore(parent)
{
	is_layout = true;
	node = YGNodeNew();
	if (parent && !parent->is_layout)
	{
		this->setWigdet(static_cast<WidgetCore*>(parent));
	}

	if (parent && parent->is_layout)
	{
		auto l = static_cast<Layout*>(parent);
		properties = l->properties;
	}

	setObjectName("Layout");
}

void Layout::setWigdet(WidgetCore* new_parent) {
	if (!new_parent->layout()) {
		new_parent->playout = this;
		setParent(new_parent);
		new_parent->bound_layout = this;
		playout = this;
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
	nodemap.insert({ item->node, item });

	// TODO
	//switch (align)
	//{
	//}

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
		YGNodeCalculateLayout(node, properties.size.width, properties.size.height, YGDirectionInherit);
	}
	else
	{
		YGNodeCalculateLayout(node, YGUndefined, YGUndefined, YGDirectionInherit);
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

