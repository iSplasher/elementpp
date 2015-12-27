#include "include/Utils/Region.h"
#include "include/Global.h"

using namespace gspl;

Region::Region(const Region& other) {

}

Region::Point Region::startPoint() const {
	return path.empty() ? Point(0, 0) : path[0];
}

Region::Point Region::endPoint() const {
	return path.empty() ? Point(0, 0) : path[path.size() - 1];
}

const Region::iterator Region::begin() {
	return path.begin();
}

const Region::iterator Region::end() {
	return path.end();
}

bool Region::addPoint(std::initializer_list<Point> p) {
	for (auto x : p) {
		path.push_back(x);
	}
	return true;
}

bool Region::addPoint(Point p) {

	return false;
}

bool Region::isEmpty() const {
	return path.empty();
}
