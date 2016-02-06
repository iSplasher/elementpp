//#pragma once
//
//#include "Global.h"
//#include "Core.h"
//#include "Utils/Primitives.h"
//
//#include <SFML/Graphics.hpp>
//#include <memory>
//
//NAMESPACE_BEGIN
//
//class gCoreWidget;
//
///// <summary>
///// Derived class can be managed by a layout
///// </summary>
//class GSPLASHER_API gLayoutable : public gCore {
//	friend class gLayout;
//public:
//	// *structers
//	gLayoutable();
//	virtual ~gLayoutable() = default;
//
//	// member methods
//	virtual void update() = 0;
//	virtual Rect boundingBox() const = 0;
//	virtual Rect contentsRect();
//	//virtual Rect contentsMargin();
//	//virtual Size prefferedSize() const = 0;
//	//virtual Size minimumSize() const = 0;
//	//virtual Size maximumSize() const = 0;
//	//virtual bool isEmpty() const = 0;
//	void setSize(const Size size) {
//		m_size = size;
//		update();
//	}
//
//	gCoreWidget* widget() const {
//		return m_widget;
//	}
//
//	gLayout* layout() const {
//		return m_layout;
//	}
//
//private:
//	// data members
//	bool in_layout;
//	Rect available_space;
//
//protected:
//	// members methods
//	virtual Rect setContentsRect(Rect r);
//
//	// data members
//	gCoreWidget* m_widget;
//	gLayout* m_layout;
//
//	Size m_size;
//	Size requested_size;
//	Point requested_pos;
//
//};
//
////class SpaceFill : public gLayoutable {
////public:
////	SpaceFill(Size& s) : size(s) {}
////	~SpaceFill();
////
////	void update();
////	Rect boundingBox();
////	Size prefferedSize();
////	Size minimumSize();
////	Size maximumSize();
////	bool isEmpty();
////private:
////	Size size;
////	Rect rect;
////};
//
///// <summary>
///// Abstract layout class. Derive this class to make a custom layout. 
///// </summary>
//class GSPLASHER_API gLayout : public gLayoutable {
//	friend class gCoreWidget;
//public:
//	// * structers
//	gLayout();
//	gLayout(gCoreWidget *p);
//	virtual ~gLayout() = 0;
//
//	// member methods
//	virtual gLayoutable* parent();
//
//	virtual void add(gLayoutable &item) = 0;
//
//	//gCoreWidget& takeWidget();
//	//gLayout& takeLayout();
//	//void remove(gLayoutable&);
//
//	//int margin();
//	//int spacing();
//	//void setMargin();
//	//void setSpacing();
//
//	//virtual int count() const;
//	//bool isEmpty() const;
//
//	//Size minimumSize() const override;
//	//Size maximumSize() const override;
//
//	Rect contentsRect() const;
//
//protected:
//	// data members
//	std::shared_ptr<std::vector<gLayoutable*>> layout_members;
//
//	unsigned m_margin;
//	const gLayoutable *m_parent;
//private:
//	// member methods
//	void update() override;
//	void changeLayoutableParent(const gLayoutable*);
//};
//
//NAMESPACE_END