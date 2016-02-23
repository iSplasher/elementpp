#pragma once

#include "Global.h"
#include "Core.h"
#include "Utils/Primitives.h"

namespace rhea
{
using Simplex = class simplex_solver;
}

NAMESPACE_BEGIN

using Simplex = rhea::Simplex;
class gCoreWidget;
class gLayout;
class gLayoutImpl;

/// <summary>
/// Derived class can be managed by a layout
/// </summary>
class GSPLASHER_API gLayoutable : public gCore {
public:
	// *structers
	gLayoutable(gLayoutable* parent = nullptr);
	virtual ~gLayoutable() = default;

	// member methods
	virtual void update() = 0;
	//virtual gRect contentsRect();
	//virtual Rect contentsMargin();
	int margin() const { return _margin; }

	//virtual gSize minimumSize() const = 0;
	//virtual gSize maximumSize() const = 0;
	//virtual bool isEmpty() const = 0;

	virtual gPoint pos() const { return c_topleft; };

	virtual void move(gPoint new_p);

	void move(int x, int y) { move(gPoint(x, y)); }

	virtual void resize(gSize new_s);
	virtual void resize(int width, int height) { resize(gSize(width, height)); }
	virtual gSize size() const { return _size; }

	virtual gRect geometry() const { return gRect(pos(), size()); }

	gLayout* layout() const {
		return m_layout;
	}

protected:
	// members methods
	//virtual gRect setContentsRect(gRect r);

	// data members
	gLayout* m_layout = nullptr;

private:
	// data members
	gLayoutable *parent_layoutable = nullptr;
	gLayoutable *layoutitem_parent = nullptr;
	bool in_layout = false;
	gRect available_space;
	gPoint g_topleft;
	gPoint c_topleft;
	gSize _size = gSize(500, 300);;
	int _margin = 2;


	friend class gLayout;
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
	void setWigdet(gCoreWidget *new_parent);

	virtual void add(gLayoutable *item, Alignment align=Alignment::HCenter|Alignment::VCenter);

	//gCoreWidget& takeWidget();
	//gLayout& takeLayout();
	//void remove(gLayoutable&);

	//int spacing();
	//void setMargin();
	//void setSpacing();

	//virtual int count() const;
	//bool isEmpty() const;

	//gSize minimumSize() const override { return gSize(0, 0); };
	//gSize maximumSize() const override { return gSize(UINT32_MAX, UINT32_MAX); };
	//gSize prefferedSize() const;

	//gRect contentsRect() const;
	virtual void invalidate() = 0;

protected:
	// data members
	std::shared_ptr<gLayoutImpl> l_impl;


private:
	// member methods
	void update() override {};
	//void changeLayoutableParent(const gLayoutable*);
};

NAMESPACE_END