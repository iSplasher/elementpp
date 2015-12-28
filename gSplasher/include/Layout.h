#pragma once

#include "Global.h"
#include "Utils/Primitives.h"

#include <SFML/Graphics.hpp>
#include <memory>

NAMESPACE_BEGIN

class BaseWidget;

class Layoutable {
	friend class Layout;
public:
	Layoutable();
	virtual ~Layoutable() = default;

	virtual void update() = 0;
	virtual Rect boundingBox() const = 0;
	virtual Size prefferedSize() const = 0;
	virtual Size minimumSize() const = 0;
	virtual Size maximumSize() const = 0;
	virtual bool isEmpty() const = 0;

private:
	bool in_layout;
	Rect available_space;
protected:
	Size requested_size;
	Point requested_pos;
};

class SpaceFill : public Layoutable {
public:
	SpaceFill(Size& s) : size(s) {}
	~SpaceFill();

	void update();
	Rect boundingBox();
	Size prefferedSize();
	Size minimumSize();
	Size maximumSize();
	bool isEmpty();
private:
	Size size;
	Rect rect;
};

class Layout : public Layoutable {
	friend class BaseWidget;
public:
	Layout();
	Layout(BaseWidget *p);
	virtual ~Layout() = 0;

	BaseWidget* parent() const;

	virtual void add(BaseWidget &w);
	virtual void add(Layout &l);

	BaseWidget& takeWidget();
	Layout& takeLayout();
	void remove(Layoutable&);
	void remove(BaseWidget&);

	int margin();
	int spacing();
	void setMargin();
	void setSpacing();

	virtual int count() const;
	bool isEmpty() const;

	Size minimumSize() const override;
	Size maximumSize() const override;

	Rect contentsRect() const;

protected:
	std::shared_ptr<std::vector<Layoutable*>> layout_members;

	unsigned m_margin;
private:
	void init();
	void update() override;
	void changeLayoutableParent(Layoutable&);
};

NAMESPACE_END