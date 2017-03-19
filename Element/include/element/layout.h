#pragma once

#include "global.h"
#include "property.h"
#include "core.h"
#include "core/primitive.h"

#include <unordered_map>

using _LayoutNode = struct YGNode*;
using _LayoutConfig = struct YGConfig*;

NAMESPACE_BEGIN
class Widget;
class Layoutable;




class ELEMENT_API Layoutable : public Element {
public:

	// *structers
	explicit Layoutable( Layoutable* parent = nullptr );

	virtual ~Layoutable();

	// POSITION & SIZE 

	Property<Point> position;
	Property<Size> size;
	Property<Size> minSize;
	Property<Size> maxSize;
	PropertyView< Rect > geometry;

	// Margin

	Property<float> marginLeft;
	Property<float> marginTop;
	Property<float> marginRight;
	Property<float> marginBottom;
	//PropertyView< Rect > margin;

	// BORDER

	Property<float> borderLeft;
	Property<float> borderTop;
	Property<float> borderRight;
	Property<float> borderBottom;
	//PropertyView< Rect > border;

	// PADDING
	Property<float> paddingLeft;
	Property<float> paddingTop;
	Property<float> paddingRight;
	Property<float> paddingBottom;
	//PropertyView< Rect > padding;

	// STYLE

	Property<Justify> justifyContent;
	Property<Align> alignItems;
	Property<Align> alignSelf;
	Property<float> grow;
	Property<float> shrink;
	Property<float> basis;
	Property<Position> positionType;
	Property<Direction> direction;
	Property<bool> wrap;

	virtual void append(Layoutable* item);
	virtual void append(std::initializer_list<Layoutable*> item);

	void take(Layoutable* item);
	void invalidate();
	
	/**
	 * \brief Returns the layout this item is contained in
	 */
	virtual void update();

private:

	virtual void updateChildren();

	virtual void invalidated();

	void setGrow(float);
	void setShrink(float);
	void setBasis(float);
	void setMaxSize(Size);
	void setMinSize(Size);
	void setSize(Size);
	void setPosition(Point);
	void setMargin(float, Direction);
	void setBorder(float, Direction);
	void setPadding(float, Direction);

	// data members

	float old_grow = 1; // used by setSize to set fixed size

	bool dirty_layout = false; // layout has to recalculate
	bool calculating = false; // layout is calculating

	_LayoutNode node = nullptr;
	static _LayoutConfig layout_config;
	std::unordered_map< _LayoutNode, Layoutable* > nodemap;

	friend class Layout;
};


NAMESPACE_END

