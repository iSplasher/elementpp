#pragma once

#include "gSplasher/Global.h"

#include <iostream>

typedef struct NVGcolor _PColor;

NAMESPACE_BEGIN

class gPen;
class gBrush;

/// <summary>
/// Point. Defines x and y.
/// </summary>
template <typename T>
struct GSPLASHER_API PointT {
	PointT() : PointT(0, 0) {}
	PointT(T X, T Y) : x(X), y(Y) {}

	template <typename O>
	explicit PointT(const PointT<O>& point) : PointT<T>(point.x, point.y) {}

	// operators for PointT

	PointT<T> operator -(const PointT<T>& rh) {
		return PointT<T>(x - rh.x, y - rh.y);
	}

	PointT<T> &operator +=(const PointT<T>& rh) {
		x += rh.x;
		y += rh.y;
		return *this;
	}

	PointT<T> &operator -=(const PointT<T>& rh) {
		x -= rh.x;
		y -= rh.y;
		return *this;
	}

	PointT<T> operator +(const PointT<T>& rh) {
		return PointT<T>(x+rh.x, y+rh.y);
	}

	PointT<T> operator *(const PointT<T>& rh) {
		return PointT<T>(x*rh.x, y*rh.y);
	}

	PointT<T> &operator *=(const PointT<T>& rh) {
		x *= rh.x;
		y *= rh.y;
		return *this;
	}

	PointT<T> operator /(const PointT<T>& rh) {
		return PointT<T>(x/rh.x, y/rh.y);
	}

	PointT<T> &operator /=(const PointT<T>& rh) {
		x /= rh.x;
		y /= rh.y;
		return *this;
	}

	bool operator ==(const PointT<T>& rh) {
		return x == rh.x && y == rh.y;
	}

	bool operator !=(const PointT<T>& rh) {
		return x != rh.x || y != rh.y;
	}

	bool operator >(const PointT<T>& rh) {
		return x > rh.x || y > rh.y;
	}

	bool operator <(const PointT<T>& rh) {
		return x < rh.x || y < rh.y;
	}

	// operators for T

	PointT<T> operator -(T rh) {
		return PointT<T>(x - rh, y - rh);
	}

	PointT<T> &operator +=(T rh) {
		x += rh;
		y += rh;
		return *this;
	}

	PointT<T> &operator -=(T rh) {
		x -= rh;
		y -= rh;
		return *this;
	}

	PointT<T> operator +(T rh) {
		return PointT<T>(x + rh, y + rh);
	}

	PointT<T> operator *(T rh) {
		return PointT<T>(x*rh, y*rh);
	}

	PointT<T> &operator *=(T rh) {
		x *= rh;
		y *= rh;
		return *this;
	}

	PointT<T> operator /(T rh) {
		return PointT<T>(x / rh, y / rh);
	}

	PointT<T> &operator /=(T rh) {
		x /= rh;
		y /= rh;
		return *this;
	}

	void operator =(T rh) {
		x = rh;
		y = rh;
	}

	bool operator ==(T rh) {
		return x == rh && y == rh;
	}

	bool operator !=(T rh) {
		return x != rh || y != rh;
	}

	bool operator >(T rh) {
		return x > rh || y > rh;
	}

	bool operator <(T rh) {
		return x < rh || y < rh;
	}

	T x;
	T y;
};

template<typename T>
std::ostream &operator <<(std::ostream &out, const PointT<T> &p) {
	out << "Point(" << p.x << ", " << p.y << ")";
	return out;
}

using Point = PointT<int>;
using PointF = PointT<float>;
using PointD = PointT<double>;
using PointU = PointT<unsigned>;

/// <summary>
/// Size. Defines width and height.
/// </summary>
template <typename T>
struct GSPLASHER_API SizeT {
	SizeT() : SizeT(0, 0) {}
	SizeT(T W, T H) : width(W), height(H) {}

	template <typename O>
	explicit SizeT(const SizeT<O>& size) : SizeT(size.width, size.height) {}

	// operators for SizeT

	SizeT<T> operator -(const SizeT<T>& rh) {
		return SizeT<T>(width - rh.width, height - rh.height);
	}

	SizeT<T> &operator +=(const SizeT<T>& rh) {
		width += rh.width;
		height += rh.height;
		return *this;
	}

	SizeT<T> &operator -=(const SizeT<T>& rh) {
		width -= rh.width;
		height -= rh.height;
		return *this;
	}

	SizeT<T> operator +(const SizeT<T>& rh) {
		return SizeT<T>(width + rh.width, height + rh.height);
	}

	SizeT<T> operator *(const SizeT<T>& rh) {
		return SizeT<T>(width * rh.width, height * rh.height);
	}

	SizeT<T> &operator *=(const SizeT<T>& rh) {
		width *= rh.width;
		height *= rh.height;
		return *this;
	}

	SizeT<T> operator /(const SizeT<T>& rh) {
		return SizeT<T>(width / rh.width, height / rh.height);
	}

	SizeT<T> &operator /=(const SizeT<T>& rh) {
		width /= rh.width;
		height /= rh.height;
		return *this;
	}

	bool operator ==(const SizeT<T>& rh) {
		return width == rh.width && height == rh.height;
	}

	bool operator !=(const SizeT<T>& rh) {
		return width !=  rh.width || height != rh.height;
	}

	bool operator >(const SizeT<T>& rh) {
		return width > rh.width || height > rh.height;
	}

	bool operator <(const SizeT<T>& rh) {
		return width < rh.width || height < rh.height;
	}

	// operators for T

	SizeT<T> operator -(T rh) {
		return SizeT<T>(width - rh, height - rh);
	}

	SizeT<T> &operator +=(T rh) {
		width += rh;
		height += rh;
		return *this;
	}

	SizeT<T> &operator -=(T rh) {
		width -= rh;
		height -= rh;
		return *this;
	}

	SizeT<T> operator +(T rh) {
		return SizeT<T>(width + rh, height + rh);
	}

	SizeT<T> operator *(T rh) {
		return SizeT<T>(width * rh, height * rh);
	}

	SizeT<T> &operator *=(T rh) {
		width *= rh;
		height *= rh;
		return *this;
	}

	SizeT<T> operator /(T rh) {
		return SizeT<T>(width / rh, height / rh);
	}

	SizeT<T> &operator /=(T rh) {
		width /= rh;
		height /= rh;
		return *this;
	}

	void operator =(T rh) {
		width = rh;
		height = rh;
	}

	bool operator ==(T rh) {
		return width == rh && height == rh;
	}

	bool operator !=(T rh) {
		return width != rh || height != rh;
	}

	bool operator >(T rh) {
		return width > rh || height > rh;
	}

	bool operator <(T rh) {
		return width < rh || height < rh;
	}

	T width;
	T height;
};

template<typename T>
std::ostream &operator <<(std::ostream &out, const SizeT<T> &s) {
	out << "Size(" << s.width << ", " << s.height << ")";
	return out;
}

using Size = SizeT<int>;
using SizeF = SizeT<float>;
using SizeD = SizeT<double>;

/// <summary>
/// gRect. Defines x, y, width and height.
/// </summary>
template <typename T>
struct GSPLASHER_API gRectT {
	gRectT() : gRectT(0, 0, 0, 0) {}
	gRectT(T X, T Y, T W, T H) : x(X), y(Y), width(W), height(H) {}
	gRectT(PointT<T> p, SizeT<T> s) : gRectT(p.x, p.y, s.width, s.height) {}
	gRectT(PointT<T> p, T W, T H) : gRectT(p.x, p.y, W, H) {}
	gRectT(T X, T Y, SizeT<T> s) : gRectT(X, Y, s.width, s.height) {}

	template <typename O>
	explicit gRectT(const gRectT<O>& rect) : gRectT(rect.x, rect.y, rect.width, rect.height) {}

	// methods

	/// <summary>
	/// If gRect is within this gRect
	/// </summary>
	bool contains(const gRectT<T>& r) const {
		return contains(r.x, r.y, r.width, r.height);
	}

	/// <summary>
	/// If Point is within this gRect
	/// </summary>
	template <typename O>
	bool contains(const PointT<O>& p) const {
		return p.x > x && p.x < x+width && p.y > y && p.y < y+height;
	}

	/// <summary>
	/// If Size is within this gRect
	/// </summary>
	template <typename O>
	bool contains(const SizeT<O>& s) const {
		return s.width > 0 && s.width < width && s.height > 0 && s.height < height;
	}

	/// <summary>
	/// If x, y, width and height are within this gRect
	/// </summary>
	bool contains(T X, T Y, T W, T H) const {
		return contains(PointT<T>(X, Y)) && contains(SizeT<T>(W, H));
	}

	/// <summary>
	/// Position of gRect
	/// </summary>
	/// <returns>PointT/<T/></returns>
	PointT<T> pos() const {
		return PointT<T>(x, y);
	}

	/// <summary>
	/// Size of gRect
	/// </summary>
	/// <returns>SizeT/<T/></returns>
	/// <remarks>If type of gRect is int, an unsigned type of Size will be returned</remarks>
	SizeT<T> size() const {
		return SizeT<T>(width, height);
	}

	//SizeT<unsigned> size<int>() const {
	//	return Size(width, height);
	//}

	// operators for gRectT

	gRectT<T> operator -(const gRectT<T>& rh) {
		return gRectT<T>(x - rh.x, y - rh.y, width - rh.width, height - rh.height);
	}

	template <typename O>
	gRectT<T> operator -(const PointT<O>& rh) {
		return gRectT<T>(x - rh.x, y - rh.y, width, height);
	}

	template <typename O>
	gRectT<T> operator -(const SizeT<O>& rh) {
		return gRectT<T>(x, y, width - rh.width, height - rh.height);
	}

	gRectT<T> &operator +=(const gRectT<T>& rh) {
		x += rh.x;
		y += rh.y;
		width += rh.width;
		height += rh.height;
		return *this;
	}

	template <typename O>
	gRectT<T> &operator +=(const PointT<O>& rh) {
		x += rh.x;
		y += rh.y;
		return *this;
	}

	template <typename O>
	gRectT<T> &operator +=(const SizeT<O>& rh) {
		width += rh.width;
		height += rh.height;
		return *this;
	}

	gRectT<T> &operator -=(const gRectT<T>& rh) {
		x -= rh.x;
		y -= rh.y;
		width -= rh.width;
		height -= rh.height;
		return *this;
	}

	template <typename O>
	gRectT<T> &operator -=(const PointT<O>& rh) {
		x -= rh.x;
		y -= rh.y;
		return *this;
	}

	template <typename O>
	gRectT<T> &operator -=(const SizeT<O>& rh) {
		width -= rh.width;
		height -= rh.height;
		return *this;
	}

	gRectT<T> operator +(const gRectT<T>& rh) {
		return gRectT<T>(x + rh.x, y + rh.y, width + rh.width, height + rh.height);
	}

	template <typename O>
	gRectT<T> operator +(const PointT<O>& rh) {
		return gRectT<T>(x + rh.x, y + rh.y, width, height);
	}

	template <typename O>
	gRectT<T> operator +(const SizeT<O>& rh) {
		return gRectT<T>(x, y, width + rh.width, height + rh.height);
	}

	gRectT<T> operator *(const gRectT<T>& rh) {
		return gRectT<T>(x * rh.x, y * rh.y, width * rh.width, height * rh.height);
	}

	template <typename O>
	gRectT<T> operator *(const PointT<O>& rh) {
		return gRectT<T>(x * rh.x, y * rh.y, width, height);
	}

	template <typename O>
	gRectT<T> operator *(const SizeT<O>& rh) {
		return gRectT<T>(x, y, width * rh.width, height * rh.height);
	}

	gRectT<T> &operator *=(const gRectT<T>& rh) {
		x *= rh.x;
		y *= rh.y;
		width *= rh.width;
		height *= rh.height;
		return *this;
	}

	template <typename O>
	gRectT<T> &operator *=(const PointT<O>& rh) {
		x *= rh.x;
		y *= rh.y;
		return *this;
	}

	template <typename O>
	gRectT<T> &operator *=(const SizeT<O>& rh) {
		width *= rh.width;
		height *= rh.height;
		return *this;
	}

	gRectT<T> operator /(const gRectT<T>& rh) {
		return gRectT<T>(x / rh.x, y / rh.y, width / rh.width, height / rh.height);
	}

	template <typename O>
	gRectT<T> operator /(const PointT<O>& rh) {
		return gRectT<T>(x / rh.x, y / rh.y, width, height);
	}

	template <typename O>
	gRectT<T> operator /(const SizeT<O>& rh) {
		return gRectT<T>(x, y, width / rh.width, height / rh.height);
	}

	gRectT<T> &operator /=(const gRectT<T>& rh) {
		x /= rh.x;
		y /= rh.y;
		width /= rh.width;
		height /= rh.height;
		return *this;
	}

	template <typename O>
	gRectT<T> &operator /=(const PointT<O>& rh) {
		x /= rh.x;
		y /= rh.y;
		return *this;
	}

	template <typename O>
	gRectT<T> &operator /=(const SizeT<O>& rh) {
		width /= rh.width;
		height /= rh.height;
		return *this;
	}

	bool operator ==(const gRectT<T>& rh) {
		return x == rh.x && y == rh.y && width == rh.width && height == rh.height;
	}

	template <typename O>
	bool operator ==(const PointT<O>& rh) {
		return x == rh.x && x == rh.x;
	}

	template <typename O>
	bool operator ==(const SizeT<O>& rh) {
		return width == rh.width && height == rh.height;
	}

	bool operator !=(const gRectT<T>& rh) {
		return x != rh.x || y != rh.y || width != rh.width || height != rh.height;
	}

	template <typename O>
	bool operator !=(const PointT<O>& rh) {
		return x != rh.x || x != rh.x;
	}

	template <typename O>
	bool operator !=(const SizeT<O>& rh) {
		return width != rh.width || height != rh.height;
	}

	bool operator >(const gRectT<T>& rh) {
		return x > rh.x || y > rh.y || width > rh.width || height > rh.height;
	}

	template <typename O>
	bool operator >(const PointT<O>& rh) {
		return x > rh.x || x > rh.x;
	}

	template <typename O>
	bool operator >(const SizeT<O>& rh) {
		return width > rh.width || height > rh.height;
	}

	bool operator <(const gRectT<T>& rh) {
		return x < rh.y || y < rh.y || width < rh.width || height < rh.height;
	}

	template <typename O>
	bool operator <(const PointT<O>& rh) {
		return x < rh.x || x < rh.x;
	}

	template <typename O>
	bool operator <(const SizeT<O>& rh) {
		return width < rh.width || height < rh.height;
	}

	template <typename O>
	void operator =(const PointT<O>& rh) {
		x = rh.x;
		y = rh.y;
	}

	template <typename O>
	void operator =(const SizeT<O>& rh) {
		width = rh.width;
		height = rh.height;
	}

	// operators for T

	gRectT<T> operator -(T rh) {
		return gRectT<T>(x - rh, y - rh, width - rh, height - rh);
	}

	gRectT<T> &operator +=(T rh) {
		x += rh;
		y += rh;
		width += rh;
		height += rh;
		return *this;
	}

	gRectT<T> &operator -=(T rh) {
		x -= rh;
		y -= rh;
		width -= rh;
		height -= rh;
		return *this;
	}

	gRectT<T> operator +(T rh) {
		return gRectT<T>(x + rh, y + rh, width + rh, height + rh);
	}

	gRectT<T> operator *(T rh) {
		return gRectT<T>(x * rh, y * rh, width * rh, height * rh);
	}

	gRectT<T> &operator *=(T rh) {
		x *= rh;
		y *= rh;
		width *= rh;
		height *= rh;
		return *this;
	}

	gRectT<T> operator /(T rh) {
		return gRectT<T>(x / rh, y / rh, width / rh, height / rh);
	}

	gRectT<T> &operator /=(T rh) {
		x /= rh;
		y /= rh;
		width /= rh;
		height /= rh;
		return *this;
	}

	void operator =(T rh) {
		x = rh;
		y = rh;
		width = rh;
		height = rh;
	}

	bool operator ==(T rh) {
		return x == rh && y == rh && width == rh && height == rh;
	}

	bool operator !=(T rh) {
		return x != rh || y != rh || width != rh || height != rh;
	}

	bool operator >(T rh) {
		return x > rh || y > rh || width > rh || height > rh;
	}

	bool operator <(T rh) {
		return x < rh || y < rh || width < rh || height < rh;
	}

	T x;
	T y;
	T width;
	T height;
};

template<typename T>
std::ostream &operator <<(std::ostream &out, const gRectT<T> &r) {
	out << "gRect(" << r.x << ", " << r.y << ", " << r.width << ", " << r.height << ")";
	return out;
}

using gRect = gRectT<int>;
using gRectF = gRectT<float>;
using gRectD = gRectT<double>;
using gRectU = gRectT<unsigned>;

class GSPLASHER_API gColor {
public:
	gColor() = default;
	gColor(int r, int g, int b) : red(r), green(g), blue(b), type(RGB) {}
	gColor(int r, int g, int b, float a) : red(r), green(g), blue(b), alpha(a), type(RGBA) {}
	gColor(float h, float s, float l) : hue(h), sat(s), light(l), type(HSL) {}
	gColor(float h, float s, float l, float a) : hue(h), sat(s), light(l), alpha(a), type(HSLA) {}

	// TODO: make private and return int/float array?
	int red = 0;
	int green = 0;
	int blue = 0;
	float hue = 0;
	float sat = 0;
	float light = 0;
	float alpha = 0;

private:
	enum Type {
		RGB,
		RGBA,
		HSL,
		HSLA
	};

	_PColor toPColor() const;
	
	Type type = RGB;

	friend class gPen;
	friend class gBrush;
	friend std::ostream &operator <<(std::ostream &out, const gColor &c);
};

inline std::ostream &operator <<(std::ostream &out, const gColor &c) {
	switch(c.type) {
	case gColor::RGB:
		out << "gColor(RGB" << "(" << c.red << ", " << c.green << ", " << c.blue << "))";
		break;
	case gColor::RGBA:
		out << "gColor(RGBA" << "(" << c.red << ", " << c.green << ", " << c.blue << ", " << c.alpha << "))";
		break;
	case gColor::HSL:
		out << "gColor(HSL" << "(" << c.hue << ", " << c.sat << ", " << c.light << "))";
		break;
	case gColor::HSLA:
		out << "gColor(HSLA" << "(" << c.hue << ", " << c.sat << ", " << c.light << ", " << c.alpha << "))";
		break;
	}
	return out;
}

class GSPLASHER_API gFont {
public:
	gFont() = default;
	//gFont(std::string face);
	//gFont(const gFont &other);

	//void setPixelSize(float pixel_size);
	//void setPointSize(float point_size);

private:
	std::string f_face = "";
	float f_pixel_size = 12;
	float f_point_size = 12;
	float l_height = f_pixel_size * 2;
	int f_align;
	float f_spacing;
};

NAMESPACE_END
