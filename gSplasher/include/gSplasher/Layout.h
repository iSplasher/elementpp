#pragma once

#include "Global.h"
#include "Core.h"
#include "Utils/Primitives.h"

NAMESPACE_BEGIN

class gCoreWidget;
class gLayout;

namespace priv {
class LayoutImpl;
struct ItemConstraints;
using ItemData = std::shared_ptr<ItemConstraints>;
using Layouter = std::shared_ptr<LayoutImpl>;
}

/// <summary>
/// Derived class can be managed by a layout
/// </summary>
class GSPLASHER_API gLayoutable : public gCore {
public:
	// *structers
	gLayoutable(gLayoutable* parent = nullptr);
	virtual ~gLayoutable();

	// member methods

	void setFixedWidth(int width);
	void setFixedHeight(int height);

	/// <summary>
	/// Returns pointer to parent gCoreWidget or nullptr if parent is not a widget or there is no parent.
	/// </summary>
	/// <returns>gCoreWidget or nullptr </returns>
	gCoreWidget* parentWidget() const;

	virtual void event(EventPtr ev);
	virtual void update() = 0;
	//virtual gRect contentsRect();
	//virtual Rect contentsMargin();
	int margin() const { return _margin; }

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

	gLayout* layout() const {
		return containing_layout;
	}

protected:
	// members methods
	virtual void resizeEvent(ResizeEventPtr ev);
	//virtual gRect setContentsRect(gRect r);

private:
	// data members
	gLayout* containing_layout = nullptr;
	bool in_layout = false;
	priv::ItemData c_data;
	int _margin = 0;


	friend class gLayout;
	friend class priv::LayoutImpl;
};

//class SpaceFill : public gLayoutable {
//public:
//	SpaceFill(Size& s) : size(s) {}
//	~SpaceFill();
//
//	void update();
//	Rect boundingBox();
//	Size prefferedSize();
//	Size minimumSize();
//	Size maximumSize();
//	bool isEmpty();
//private:
//	Size size;
//	Rect rect;
//};

/// <summary>
/// Abstract layout class. Derive this class to make a custom layout. 
/// </summary>
class GSPLASHER_API gLayout : public gLayoutable {
	friend class gCoreWidget;
public:
	// * structers
	explicit gLayout(gCoreWidget *parent = nullptr);
	virtual ~gLayout() = default;

	// member methods
	//virtual gLayoutable* parent();
	void setWigdet(gCoreWidget *new_parent) const;

	virtual void add(gLayoutable *item, Alignment align=Alignment::HCenter|Alignment::VCenter);

	//gCoreWidget& takeWidget();
	//gLayout& takeLayout();
	//void remove(gLayoutable&);

	int spacing() const { return _spacing; }
	//void setMargin();
	void setSpacing(int s) { _spacing = s; invalidate(); }

	//virtual int count() const;
	//bool isEmpty() const;

	//gSize minimumSize() const override { return gSize(0, 0); };
	//gSize maximumSize() const override { return gSize(UINT32_MAX, UINT32_MAX); };
	//gSize prefferedSize() const;

	//gRect contentsRect() const;
	virtual void invalidate();

protected:
	// data members
	priv::Layouter layouter;


private:
	// member methods
	void update() override {};
	void setFixedWidth(int width) {}
	void setFixedheight(int height) {}

	// data
	int _spacing = 5;
};

NAMESPACE_END