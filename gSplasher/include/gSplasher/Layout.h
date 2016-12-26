#pragma once

#include "Global.h"
#include "Core.h"
#include "Utils/Primitives.h"

#include <unordered_map>

NAMESPACE_BEGIN

class WidgetCore;
class Layout;


namespace priv {

#include <yoga/Yoga.h>
typedef YGNodeRef LayoutNode;

/// <summary>
/// Derived class can be managed by a layout
/// </summary>
class GSPLASHER_API LayoutCore : public Core {
public:
	// *structers
	LayoutCore(LayoutCore* parent = nullptr);
	virtual ~LayoutCore();

	// member methods

	//void setFixedWidth(int width);
	//void setFixedHeight(int height);

	virtual void event(EventPtr ev);
	virtual void update() = 0;
	//virtual gRect contentsRect();
	//virtual Rect contentsMargin();
	//int margin() const;

	//virtual Size minimumSize() const = 0;
	//virtual Size maximumSize() const = 0;
	//virtual bool isEmpty() const = 0;

	virtual Point pos() const;

	virtual void move(Point new_p);
	void move(int x, int y) { move(Point(x, y)); }

	virtual void resize(Size new_s);
	virtual void resize(int width, int height) { resize(Size(width, height)); }
	virtual Size size() const;

	virtual gRect geometry() const { return gRect(pos(), size()); }

	/// <summary>
	/// Returns the layout which handles this item
	/// </summary>
	/// <returns>Layout</returns>
	Layout* layout() const { return playout; }

protected:

	struct Properties
	{
		Size size;
		Orientation orientation = Orientation::Horizontal;
		bool reverse = false;
	};

	// members methods

	/// <summary>
	/// Move events are sent here
	/// </summary>
	/// <param name="ev">Move event</param>
	/// <remarks>The item is already at the new position at the time of event</remarks>
	virtual void moveEvent(MoveEventPtr ev) {}

	/// <summary>
	/// Resize events are sent here.
	/// </summary>
	/// <param name="ev">Resize event</param>
	/// <remarks>The resizing has not yet been processed at the time of event</remarks>
	virtual void resizeEvent(ResizeEventPtr ev);
	//virtual gRect setContentsRect(gRect r);

private:
	// data members

	bool is_layout = false;
	Layout* playout = nullptr;
	Layout* bound_layout = nullptr;
	LayoutNode node = nullptr;
	Properties properties;

	friend class Layout;
};

}


/// <summary>
/// Abstract layout class. Derive this class to make a custom layout. 
/// </summary>
class GSPLASHER_API Layout : public priv::LayoutCore {
	friend class WidgetCore;
public:
	// * structers
	explicit Layout(LayoutCore *parent = nullptr);
	virtual ~Layout() = default;

	// member methods
	//virtual LayoutCore* parent();
	void setWigdet(WidgetCore *new_parent);

	virtual void appendItem(LayoutCore *item, Alignment align = Alignment::Stretch);

	/// <summary>
	/// Take item out of layout. 
	/// </summary>
	/// <param name="item">item to take out</param>
	void takeItem(LayoutCore* item);
	//void remove(LayoutCore&);

	//int spacing() const { return _spacing; }
	//void setMargin();
	//void setSpacing(int s) { _spacing = s; invalidate(); }

	//virtual int count() const;
	//bool isEmpty() const;

	//Size minimumSize() const override { return Size(0, 0); };
	//Size maximumSize() const override { return Size(UINT32_MAX, UINT32_MAX); };
	//Size prefferedSize() const;

	//gRect contentsRect() const;
	virtual void invalidate();

protected:

	void event(EventPtr ev);
	void beginLayoutChange() const;
	void endLayoutChange() const;

private:
	// member methods
	void update() override {};
	void setFixedWidth(int width) {}
	void setFixedheight(int height) {}

	// data
	std::unordered_map<priv::LayoutNode, LayoutCore*> nodemap;

};

NAMESPACE_END