#pragma once

#include "global.h"
#include "property.h"
#include "core.h"
#include "core/primitive.h"

#include <unordered_map>

NAMESPACE_BEGIN
class Widget;
class Layout;

using WidgetPtr = std::unique_ptr< Widget >;
using LayoutPtr = std::unique_ptr< Layout >;

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

	const Property<Point> position;
	const Property<Size> size;
	const PropertyView< Rect > geometry;
	//const PropertyView< Rect > contentGeometry;

	/// <summary>
	/// Returns the layout which handles this item
	/// </summary>
	/// <returns>Layout</returns>
	const Accessor< Layout*, Layoutable > layout;


	virtual void update();

	//virtual Rect contentsRect();
	//virtual Rect contentsMargin();
	//int margin() const;

	//virtual Size minimumSize() const = 0;
	//virtual Size maximumSize() const = 0;
	//virtual bool isEmpty() const = 0;

protected:

	struct Properties {
		Point position;
		Size size;
		Size min_size;
		Size max_size;
		Orientation orientation = Orientation::Horizontal;

		float margin_left = 1;
		float margin_top = 1;
		float margin_right = 1;
		float margin_bottom = 1;

		float padding_left = 1;
		float padding_top = 1;
		float padding_right = 1;
		float padding_bottom = 1;

		float border_left = 0;
		float border_top = 0;
		float border_right = 0;
		float border_bottom = 0;

		float grow = 1;
		float shrink = 0;
		bool reverse = false;
	};


	// members methods

private:

	virtual void updateChildren();

	void updateGeometry();

	// data members

	bool dirty_layuot = false; // item geometry has been invalidated

	Layout* playout = nullptr; // containing layout
	Layout* bound_layout = nullptr; // setWidget on layout
	LayoutNode node = nullptr;
	Properties properties;

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

	// member methods

	//virtual LayoutCore* parent();
	const Accessor< Widget* , Layout > widget;

	virtual void appendItem( PRIV_NAMESPACE::Layoutable* item, Alignment align = Alignment::Default, float grow = 1 );

	/// <summary>
	/// Take item out of layout. 
	/// </summary>
	/// <param name="item">item to take out</param>
	void takeItem( PRIV_NAMESPACE::Layoutable* item );

	//void remove(LayoutCore&);

	//int spacing() const { return _spacing; }
	//void setMargin();
	//void setSpacing(int s) { _spacing = s; invalidate(); }

	//virtual int count() const;
	//bool isEmpty() const;

	//Size minimumSize() const override { return Size(0, 0); };
	//Size maximumSize() const override { return Size(UINT32_MAX, UINT32_MAX); };
	//Size prefferedSize() const;

	//Rect contentsRect() const;
	virtual void invalidate();

protected:

	//void beginLayoutChange() const;

	//void endLayoutChange() const;

private:

	// member methods
	void update() override {};

	void applyItemProperties( PRIV_NAMESPACE::Layoutable* item, Alignment align, float grow );

	// data
	std::unordered_map< priv::LayoutNode, PRIV_NAMESPACE::Layoutable* > nodemap;
	Widget* _widget;

};


NAMESPACE_END
