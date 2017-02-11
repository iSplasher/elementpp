#include "element/layout.h"
#include "element/widget.h"

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

LayoutCore::LayoutCore(LayoutCore* parent) : Component(parent)
{
	if (parent && parent->is_layout)
	{
		auto l = static_cast<Layout*>(parent);
		l->appendItem(this);
	}
	this->parent = parent;
}

LayoutCore::~LayoutCore()
{
}

void LayoutCore::event(EventPtr ev)
{
	Component::event(ev);
}

void LayoutCore::update()
{
	updateChildren();
}

Point LayoutCore::pos()
{
	updateGeometry();
	return properties.position;
}

void LayoutCore::move(Point new_p)
{
	// check absolute
}

void LayoutCore::resize(Size new_s)
{
	resizeEvent(std::make_shared<ResizeEvent>(Event::Type::Resize, new_s, size()));
}

Size LayoutCore::size()
{
	updateGeometry();
	return properties.size;
}

void LayoutCore::resizeEvent(ResizeEventPtr ev)
{
	properties.size.width = ev->size.width;
	properties.size.height = ev->size.height;
	if (node)
	{
		YGNodeStyleSetWidth(node, properties.size.width);
		YGNodeStyleSetHeight(node, properties.size.height);
	}
	if (playout)
	{
		playout->invalidate();
	}

}

void LayoutCore::updateChildren()
{
	for (auto &c : children()) {
		static_cast<LayoutCore*>(c)->update();
	}
}

void LayoutCore::updateGeometry()
{
	if (dirty_layuot)
	{
		if (node)
		{
			auto &p = properties;
			p.position.x = YGNodeLayoutGetLeft(node);
			p.position.y = YGNodeLayoutGetTop(node);
			p.size.width = YGNodeLayoutGetWidth(node);
			p.size.height = YGNodeLayoutGetHeight(node);
		}
		dirty_layuot = false;
	}
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

	objectName = "Layout";
}

void Layout::setWigdet(WidgetCore* new_parent) {
	if (!new_parent->layout()) {
		new_parent->playout = this;
		this->parent = new_parent;
		new_parent->bound_layout = this;
		properties.size = new_parent->properties.size;
		playout = this;
	}
	else {
		std::cout << "This wigdet is already handled by a layout\n"; // TODO: log
	}
}

void Layout::appendItem(LayoutCore* item, Alignment align, float grow)
{
	if (item->layout() == this)
		return; // TODO: maybe warn?

	noOwnership(item);

	if (item->layout())
		item->layout()->takeItem(item);

	if (!item->node)
	{
		item->node = YGNodeNew();
	}

	auto n = YGNodeGetChildCount(node);
	YGNodeInsertChild(node, item->node, n);
	nodemap.insert({ item->node, item });

	applyItemProperties(item, align, grow);

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
	LayoutCore* p = nullptr;
	Component* pa = parent;
	if (parent)
		p = static_cast<LayoutCore*>(pa);

	if (p && p->bound_layout == this)
	{
		auto &pr = properties;
		auto r = p->contentGeometry();
		pr.size.width = r.width;
		pr.size.height = r.height;
		YGNodeCalculateLayout(node, pr.size.width, pr.size.height, YGDirectionInherit);
	}
	else
	{
		YGNodeCalculateLayout(node, YGUndefined, YGUndefined, YGDirectionInherit);
	}

	for (auto i : nodemap)
	{
		i.second->dirty_layuot = true;
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

void Layout::applyItemProperties(LayoutCore* item, Alignment align, float grow)
{
	auto prop = item->properties;
	if (grow > 0)
	{
		prop.grow = grow;
		prop.shrink = 0;
	}
	else if (grow < 0)
	{
		prop.grow = 0;
		prop.shrink = grow;
	}
	YGNodeStyleSetFlexGrow(item->node, prop.grow);
	YGNodeStyleSetFlexShrink(item->node, prop.shrink);

	YGNodeStyleSetMargin(item->node, YGEdgeLeft, prop.margin_left);
	YGNodeStyleSetMargin(item->node, YGEdgeTop, prop.margin_top);
	YGNodeStyleSetMargin(item->node, YGEdgeRight, prop.margin_right);
	YGNodeStyleSetMargin(item->node, YGEdgeBottom, prop.margin_bottom);

	YGNodeStyleSetBorder(item->node, YGEdgeLeft, prop.border_left);
	YGNodeStyleSetBorder(item->node, YGEdgeTop, prop.border_top);
	YGNodeStyleSetBorder(item->node, YGEdgeRight, prop.border_right);
	YGNodeStyleSetBorder(item->node, YGEdgeBottom, prop.border_bottom);

	YGNodeStyleSetPadding(item->node, YGEdgeLeft, prop.padding_left);
	YGNodeStyleSetPadding(item->node, YGEdgeTop, prop.padding_top);
	YGNodeStyleSetPadding(item->node, YGEdgeRight, prop.padding_right);
	YGNodeStyleSetPadding(item->node, YGEdgeBottom, prop.padding_bottom);
}

void Layout::noOwnership(LayoutCore* item)
{
	if (parent) {}
}
