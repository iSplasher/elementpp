#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Primitives.h"

namespace gspl
{
class Region final {
	using iterator = std::vector<Point>::iterator;

public:
	Region() = default;
	Region(const Region &other);
	~Region() = default;

	// starting  and ending points
	Point startPoint() const;
	Point endPoint() const;
	const iterator begin();
	const iterator end();
	// will return false or true wether point was made (point won't be added if it makes an irregular shape)
	bool addPoint(std::initializer_list<Point> p);
	bool addPoint(Point p);
	// draw region from points
	//Region &operator<<(const Point &p);
	// joins other region with this one
	//Region &operator+=(const Region &other);
	// subtracts other region with this one
	//Region &operator-=(const Region &other);
	//bool insertPoint(std::initializer_list<Point> p);
	//bool insertPoint(Point p);
	// if region is inside this region
	//bool contains(Region &r);
	bool isEmpty() const;

private:
	// removes points inside.. leaves outer points
	//void cleanInside();
	// joins two region while cleaning the insides
	//Region *concatenate(Region &r1, Region&r2);

	std::vector<Point> path;
};

Region operator+(const Region &lhs, const Region &rhs);
Region operator-(const Region &lhs, const Region &rhs);
}