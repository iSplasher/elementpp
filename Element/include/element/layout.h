#pragma once

#include "global.h"
#include "property.h"
#include "core.h"
#include "core/primitive.h"

#include <unordered_map>

NAMESPACE_BEGIN
class Widget;
class Layout;

PRIV_NAMESPACE_BEGIN
#include <yoga/Yoga.h>

class Layoutable;

using LayoutNode = YGNodeRef;
using LayoutablePtr = std::unique_ptr< Layoutable >;


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

	Property<Alignment> alignment;
	Property<float> grow;
	Property<float> shrink;
	Property<bool> absolutePosition;

	
	/**
	 * \brief Returns the layout this item is contained in
	 */
	const Accessor< Layout*, Layoutable > layout;


	virtual void update();

private:

	virtual void updateChildren();

	virtual void invalidated();

	virtual void applyStyle();

	void setGrow(float);
	void setShrink(float);
	void setMaxSize(Size);
	void setMinSize(Size);
	void setSize(Size);
	void setPosition(Point);
	void setAlignment(Alignment);
	void setMargin(float, Direction);
	void setBorder(float, Direction);
	void setpadding(float, Direction);

	Layout* getLayout() const;

	// data members

	float old_grow = 1; // used by setSize to set fixed size

	bool dirty_layout = false; // layout has to recalculate
	bool calculating = false; // layout is calculating
	bool parent_has_calculated = false; // used to avoid an uncalculated node retrieving itself

	Layout* in_layout = nullptr; // layout this item is contained in
	Layout* bound_layout = nullptr; // layout that manages items for this widget
	LayoutNode node = nullptr;

	friend class Layout;
};


NAMESPACE_END


/**
 * \brief Layout class. Derive this class to make a custom layout. 
 */
class ELEMENT_API Layout : public PRIV_NAMESPACE::Layoutable {
	friend class Widget;
public:
	// * structers
	explicit Layout();

	virtual ~Layout() = default;

	// POSITION & SIZE 

	PropertyView<Point> position;
	PropertyView<Size> size;
	PropertyView<Size> minSize;
	PropertyView<Size> maxSize;

	// STYLE

	Property<Orientation> orientation;
	Property<bool> wrap;

	const Accessor< Widget* , Layout > widget;


	// member methods

	virtual void append( PRIV_NAMESPACE::Layoutable* item, Alignment align = Alignment::Default );
	virtual void append( std::initializer_list<PRIV_NAMESPACE::Layoutable*> item, Alignment align = Alignment::Default );

	void update() override;

	/// <summary>
	/// Take item out of layout. 
	/// </summary>
	/// <param name="item">item to take out</param>
	void take( PRIV_NAMESPACE::Layoutable* item );

	//void remove(LayoutCore&);

	//virtual int count() const;
	//bool isEmpty() const;

	virtual void invalidate();

protected:

	//void beginLayoutChange() const;

	//void endLayoutChange() const;

private:

	// member methods
	Widget* getWidget() const { return _widget; }
	void setWidget(Widget*);

	void setOrientation(Orientation);
	void setWrap(bool);

	void invalidated() override;

	void applyStyle() override;

	// data
	std::unordered_map< priv::LayoutNode, PRIV_NAMESPACE::Layoutable* > nodemap;
	Widget* _widget = nullptr;
};


NAMESPACE_END
