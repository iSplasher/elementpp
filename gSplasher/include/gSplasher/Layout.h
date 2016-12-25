#pragma once

#include "Global.h"
#include "Core.h"
#include "Utils/Primitives.h"

#include <unordered_map>

NAMESPACE_BEGIN

class CoreWidget;
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

	//virtual gSize minimumSize() const = 0;
	//virtual gSize maximumSize() const = 0;
	//virtual bool isEmpty() const = 0;

	virtual gPoint pos() const;

	virtual void move(gPoint new_p);
	void move(int x, int y) { move(gPoint(x, y)); }

	virtual void resize(gSize new_s);
	virtual void resize(int width, int height) { resize(gSize(width, height)); }
	virtual gSize size() const;

	virtual gRect geometry() const { return gRect(pos(), size()); }

	/// <summary>
	/// Returns the layout which handles this item
	/// </summary>
	/// <returns>Layout</returns>
	Layout* layout() const { return playout; }

protected:
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

	friend class Layout;
};

}


/// <summary>
/// Abstract layout class. Derive this class to make a custom layout. 
/// </summary>
class GSPLASHER_API Layout : public priv::LayoutCore {
	friend class CoreWidget;
public:
	// * structers
	explicit Layout(LayoutCore *parent = nullptr);
	virtual ~Layout() = default;

	// member methods
	//virtual LayoutCore* parent();
	void setWigdet(CoreWidget *new_parent);

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

	//gSize minimumSize() const override { return gSize(0, 0); };
	//gSize maximumSize() const override { return gSize(UINT32_MAX, UINT32_MAX); };
	//gSize prefferedSize() const;

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