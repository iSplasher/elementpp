#pragma once

#include "element/global.h"
#include "element/property.h"

typedef struct NVGcolor _PColor;

NAMESPACE_BEGIN
class Pen;
class Brush;


/// <summary>
/// Point. Defines x and y.
/// </summary>
template< typename T >
struct ELEMENT_API PointT {
	PointT() : PointT( 0, 0 ) {}

	PointT( T X, T Y ) : x( X ),
	                     y( Y ) {}

	template< typename O >
	explicit PointT( const PointT< O >& point ) : PointT< T >( point.x, point.y ) {}

	// operators for PointT

	PointT< T > operator -( const PointT< T >& rh ) const {
		return PointT< T >( x - rh.x, y - rh.y );
	}

	PointT< T >& operator +=( const PointT< T >& rh ) {
		x += rh.x;
		y += rh.y;
		return *this;
	}

	PointT< T >& operator -=( const PointT< T >& rh ) {
		x -= rh.x;
		y -= rh.y;
		return *this;
	}

	PointT< T > operator +( const PointT< T >& rh ) const {
		return PointT< T >( x + rh.x, y + rh.y );
	}

	PointT< T > operator *( const PointT< T >& rh ) const {
		return PointT< T >( x * rh.x, y * rh.y );
	}

	PointT< T >& operator *=( const PointT< T >& rh ) {
		x *= rh.x;
		y *= rh.y;
		return *this;
	}

	PointT< T > operator /( const PointT< T >& rh ) const {
		return PointT< T >( x / rh.x, y / rh.y );
	}

	PointT< T >& operator /=( const PointT< T >& rh ) {
		x /= rh.x;
		y /= rh.y;
		return *this;
	}

	bool operator ==( const PointT< T >& rh ) const {
		return x == rh.x && y == rh.y;
	}

	bool operator !=( const PointT< T >& rh ) const {
		return x != rh.x || y != rh.y;
	}

	bool operator >( const PointT< T >& rh ) const {
		return x > rh.x || y > rh.y;
	}

	bool operator <( const PointT< T >& rh ) const {
		return x < rh.x || y < rh.y;
	}

	// operators for T

	PointT< T > operator -( const T rh ) const {
		return PointT< T >( x - rh, y - rh );
	}

	PointT< T >& operator +=( const T rh ) {
		x += rh;
		y += rh;
		return *this;
	}

	PointT< T >& operator -=( const T rh ) {
		x -= rh;
		y -= rh;
		return *this;
	}

	PointT< T > operator +( const T rh ) const {
		return PointT< T >( x + rh, y + rh );
	}

	PointT< T > operator *( const T rh ) const {
		return PointT< T >( x * rh, y * rh );
	}

	PointT< T >& operator *=( const T rh ) {
		x *= rh;
		y *= rh;
		return *this;
	}

	PointT< T > operator /( const T rh ) const {
		return PointT< T >( x / rh, y / rh );
	}

	PointT< T >& operator /=( const T rh ) {
		x /= rh;
		y /= rh;
		return *this;
	}

	PointT< T >& operator =( const T rh ) const {
		x = rh;
		y = rh;
		return *this;
	}

	bool operator ==( const T rh ) const {
		return x == rh && y == rh;
	}

	bool operator !=( const T rh ) const {
		return x != rh || y != rh;
	}

	bool operator >( const T rh ) const {
		return x > rh || y > rh;
	}

	bool operator <( const T rh ) const {
		return x < rh || y < rh;
	}

	T x = 0;
	T y = 0;
};


template< typename T >
std::ostream& operator <<( std::ostream& out, const PointT< T >& p ) {
	out << "Point(" << p.x << ", " << p.y << ")";
	return out;
}

using Point = PointT< int >;
using PointF = PointT< float >;
using PointD = PointT< double >;
using PointU = PointT< unsigned >;


/// <summary>
/// Size. Defines width and height.
/// </summary>
template< typename T >
struct ELEMENT_API SizeT {
	SizeT() : SizeT( 0, 0 ) {}

	SizeT( T W, T H ) : width( W ),
	                    height( H ) {}

	template< typename O >
	explicit SizeT( const SizeT< O >& size ) : SizeT( size.width, size.height ) {}

	// operators for SizeT

	SizeT< T > operator -( const SizeT< T >& rh ) const {
		return SizeT< T >( width - rh.width, height - rh.height );
	}

	SizeT< T >& operator +=( const SizeT< T >& rh ) {
		width += rh.width;
		height += rh.height;
		return *this;
	}

	SizeT< T >& operator -=( const SizeT< T >& rh ) {
		width -= rh.width;
		height -= rh.height;
		return *this;
	}

	SizeT< T > operator +( const SizeT< T >& rh ) const {
		return SizeT< T >( width + rh.width, height + rh.height );
	}

	SizeT< T > operator *( const SizeT< T >& rh ) const {
		return SizeT< T >( width * rh.width, height * rh.height );
	}

	SizeT< T >& operator *=( const SizeT< T >& rh ) {
		width *= rh.width;
		height *= rh.height;
		return *this;
	}

	SizeT< T > operator /( const SizeT< T >& rh ) const {
		return SizeT< T >( width / rh.width, height / rh.height );
	}

	SizeT< T >& operator /=( const SizeT< T >& rh ) const {
		width /= rh.width;
		height /= rh.height;
		return *this;
	}

	bool operator ==( const SizeT< T >& rh ) const {
		return width == rh.width && height == rh.height;
	}

	bool operator !=( const SizeT< T >& rh ) const {
		return width != rh.width || height != rh.height;
	}

	bool operator >( const SizeT< T >& rh ) const {
		return width > rh.width || height > rh.height;
	}

	bool operator <( const SizeT< T >& rh ) const {
		return width < rh.width || height < rh.height;
	}

	// operators for T

	SizeT< T > operator -( const T rh ) const {
		return SizeT< T >( width - rh, height - rh );
	}

	SizeT< T >& operator +=( const T rh ) {
		width += rh;
		height += rh;
		return *this;
	}

	SizeT< T >& operator -=( const T rh ) {
		width -= rh;
		height -= rh;
		return *this;
	}

	SizeT< T > operator +( const T rh ) const {
		return SizeT< T >( width + rh, height + rh );
	}

	SizeT< T > operator *( const T rh ) const {
		return SizeT< T >( width * rh, height * rh );
	}

	SizeT< T >& operator *=( const T rh ) {
		width *= rh;
		height *= rh;
		return *this;
	}

	SizeT< T > operator /( const T rh ) const {
		return SizeT< T >( width / rh, height / rh );
	}

	SizeT< T >& operator /=( const T rh ) {
		width /= rh;
		height /= rh;
		return *this;
	}

	SizeT< T > operator =( const T rh ) {
		width = rh;
		height = rh;
		return *this;
	}

	bool operator ==( const T rh ) const {
		return width == rh && height == rh;
	}

	bool operator !=( const T rh ) const {
		return width != rh || height != rh;
	}

	bool operator >( const T rh ) const {
		return width > rh || height > rh;
	}

	bool operator <( const T rh ) const {
		return width < rh || height < rh;
	}

	T width = 0;
	T height = 0;
};


template< typename T >
std::ostream& operator <<( std::ostream& out, const SizeT< T >& s ) {
	out << "Size(" << s.width << ", " << s.height << ")";
	return out;
}

using Size = SizeT< int >;
using SizeF = SizeT< float >;
using SizeD = SizeT< double >;


/// <summary>
/// Rect. Defines x, y, width and height.
/// </summary>
template< typename T >
struct ELEMENT_API RectT {
	RectT() : RectT( 0, 0, 0, 0 ) {}

	RectT( T X, T Y, T W, T H ) : x( X ),
	                              y( Y ),
	                              width( W ),
	                              height( H ) {}

	RectT( PointT< T > p, SizeT< T > s ) : RectT( p.x, p.y, s.width, s.height ) {}
	RectT( PointT< T > p, T W, T H ) : RectT( p.x, p.y, W, H ) {}
	RectT( T X, T Y, SizeT< T > s ) : RectT( X, Y, s.width, s.height ) {}

	template< typename O >
	explicit RectT( const RectT< O >& rect ) : RectT( rect.x, rect.y, rect.width, rect.height ) {}

	// methods

	/// <summary>
	/// If Rect is within this Rect
	/// </summary>
	bool contains( const RectT< T >& r ) const {
		return contains( r.x, r.y, r.width, r.height );
	}

	/// <summary>
	/// If Point is within this Rect
	/// </summary>
	template< typename O >
	bool contains( const PointT< O >& p ) const {
		return p.x > x && p.x < x + width && p.y > y && p.y < y + height;
	}

	/// <summary>
	/// If Size is within this Rect
	/// </summary>
	template< typename O >
	bool contains( const SizeT< O >& s ) const {
		return s.width > 0 && s.width < width && s.height > 0 && s.height < height;
	}

	/// <summary>
	/// If x, y, width and height are within this Rect
	/// </summary>
	bool contains( const T X, const T Y, const T W, const T H ) const {
		return contains( PointT< T >( X, Y ) ) && contains( SizeT< T >( W, H ) );
	}

	/// <summary>
	/// Position of Rect
	/// </summary>
	/// <returns>PointT/<T/></returns>
	PointT< T > pos() const {
		return PointT< T >( x, y );
	}

	/// <summary>
	/// Size of Rect
	/// </summary>
	/// <returns>SizeT/<T/></returns>
	/// <remarks>If type of Rect is int, an unsigned type of Size will be returned</remarks>
	SizeT< T > size() const {
		return SizeT< T >( width, height );
	}

	//SizeT<unsigned> size<int>() const {
	//	return Size(width, height);
	//}

	// operators for RectT

	RectT< T > operator -( const RectT< T >& rh ) const {
		return RectT< T >( x - rh.x, y - rh.y, width - rh.width, height - rh.height );
	}

	template< typename O >
	RectT< T > operator -( const PointT< O >& rh ) const {
		return RectT< T >( x - rh.x, y - rh.y, width, height );
	}

	template< typename O >
	RectT< T > operator -( const SizeT< O >& rh ) const {
		return RectT< T >( x, y, width - rh.width, height - rh.height );
	}

	RectT< T >& operator +=( const RectT< T >& rh ) {
		x += rh.x;
		y += rh.y;
		width += rh.width;
		height += rh.height;
		return *this;
	}

	template< typename O >
	RectT< T >& operator +=( const PointT< O >& rh ) {
		x += rh.x;
		y += rh.y;
		return *this;
	}

	template< typename O >
	RectT< T >& operator +=( const SizeT< O >& rh ) {
		width += rh.width;
		height += rh.height;
		return *this;
	}

	RectT< T >& operator -=( const RectT< T >& rh ) {
		x -= rh.x;
		y -= rh.y;
		width -= rh.width;
		height -= rh.height;
		return *this;
	}

	template< typename O >
	RectT< T >& operator -=( const PointT< O >& rh ) {
		x -= rh.x;
		y -= rh.y;
		return *this;
	}

	template< typename O >
	RectT< T >& operator -=( const SizeT< O >& rh ) {
		width -= rh.width;
		height -= rh.height;
		return *this;
	}

	RectT< T > operator +( const RectT< T >& rh ) const {
		return RectT< T >( x + rh.x, y + rh.y, width + rh.width, height + rh.height );
	}

	template< typename O >
	RectT< T > operator +( const PointT< O >& rh ) const {
		return RectT< T >( x + rh.x, y + rh.y, width, height );
	}

	template< typename O >
	RectT< T > operator +( const SizeT< O >& rh ) const {
		return RectT< T >( x, y, width + rh.width, height + rh.height );
	}

	RectT< T > operator *( const RectT< T >& rh ) const {
		return RectT< T >( x * rh.x, y * rh.y, width * rh.width, height * rh.height );
	}

	template< typename O >
	RectT< T > operator *( const PointT< O >& rh ) const {
		return RectT< T >( x * rh.x, y * rh.y, width, height );
	}

	template< typename O >
	RectT< T > operator *( const SizeT< O >& rh ) const {
		return RectT< T >( x, y, width * rh.width, height * rh.height );
	}

	RectT< T >& operator *=( const RectT< T >& rh ) {
		x *= rh.x;
		y *= rh.y;
		width *= rh.width;
		height *= rh.height;
		return *this;
	}

	template< typename O >
	RectT< T >& operator *=( const PointT< O >& rh ) {
		x *= rh.x;
		y *= rh.y;
		return *this;
	}

	template< typename O >
	RectT< T >& operator *=( const SizeT< O >& rh ) {
		width *= rh.width;
		height *= rh.height;
		return *this;
	}

	RectT< T > operator /( const RectT< T >& rh ) const {
		return RectT< T >( x / rh.x, y / rh.y, width / rh.width, height / rh.height );
	}

	template< typename O >
	RectT< T > operator /( const PointT< O >& rh ) const {
		return RectT< T >( x / rh.x, y / rh.y, width, height );
	}

	template< typename O >
	RectT< T > operator /( const SizeT< O >& rh ) const {
		return RectT< T >( x, y, width / rh.width, height / rh.height );
	}

	RectT< T >& operator /=( const RectT< T >& rh ) {
		x /= rh.x;
		y /= rh.y;
		width /= rh.width;
		height /= rh.height;
		return *this;
	}

	template< typename O >
	RectT< T >& operator /=( const PointT< O >& rh ) {
		x /= rh.x;
		y /= rh.y;
		return *this;
	}

	template< typename O >
	RectT< T >& operator /=( const SizeT< O >& rh ) {
		width /= rh.width;
		height /= rh.height;
		return *this;
	}

	bool operator ==( const RectT< T >& rh ) const {
		return x == rh.x && y == rh.y && width == rh.width && height == rh.height;
	}

	template< typename O >
	bool operator ==( const PointT< O >& rh ) const {
		return x == rh.x && x == rh.x;
	}

	template< typename O >
	bool operator ==( const SizeT< O >& rh ) const {
		return width == rh.width && height == rh.height;
	}

	bool operator !=( const RectT< T >& rh ) const {
		return x != rh.x || y != rh.y || width != rh.width || height != rh.height;
	}

	template< typename O >
	bool operator !=( const PointT< O >& rh ) const {
		return x != rh.x || x != rh.x;
	}

	template< typename O >
	bool operator !=( const SizeT< O >& rh ) const {
		return width != rh.width || height != rh.height;
	}

	bool operator >( const RectT< T >& rh ) const {
		return x > rh.x || y > rh.y || width > rh.width || height > rh.height;
	}

	template< typename O >
	bool operator >( const PointT< O >& rh ) const {
		return x > rh.x || x > rh.x;
	}

	template< typename O >
	bool operator >( const SizeT< O >& rh ) const {
		return width > rh.width || height > rh.height;
	}

	bool operator <( const RectT< T >& rh ) const {
		return x < rh.y || y < rh.y || width < rh.width || height < rh.height;
	}

	template< typename O >
	bool operator <( const PointT< O >& rh ) const {
		return x < rh.x || x < rh.x;
	}

	template< typename O >
	bool operator <( const SizeT< O >& rh ) const {
		return width < rh.width || height < rh.height;
	}

	template< typename O >
	RectT< T > operator =( const PointT< O >& rh ) {
		x = rh.x;
		y = rh.y;
		return *this;
	}

	template< typename O >
	RectT< T > operator =( const SizeT< O >& rh ) {
		width = rh.width;
		height = rh.height;
		return *this;
	}

	// operators for T

	RectT< T > operator -( const T rh ) const {
		return RectT< T >( x - rh, y - rh, width - rh, height - rh );
	}

	RectT< T >& operator +=( const T rh ) {
		x += rh;
		y += rh;
		width += rh;
		height += rh;
		return *this;
	}

	RectT< T >& operator -=( const T rh ) {
		x -= rh;
		y -= rh;
		width -= rh;
		height -= rh;
		return *this;
	}

	RectT< T > operator +( const T rh ) const {
		return RectT< T >( x + rh, y + rh, width + rh, height + rh );
	}

	RectT< T > operator *( const T rh ) const {
		return RectT< T >( x * rh, y * rh, width * rh, height * rh );
	}

	RectT< T >& operator *=( const T rh ) {
		x *= rh;
		y *= rh;
		width *= rh;
		height *= rh;
		return *this;
	}

	RectT< T > operator /( const T rh ) const {
		return RectT< T >( x / rh, y / rh, width / rh, height / rh );
	}

	RectT< T >& operator /=( const T rh ) {
		x /= rh;
		y /= rh;
		width /= rh;
		height /= rh;
		return *this;
	}

	RectT< T > operator =( const T rh ) {
		x = rh;
		y = rh;
		width = rh;
		height = rh;
		return *this;
	}

	bool operator ==( const T rh ) const {
		return x == rh && y == rh && width == rh && height == rh;
	}

	bool operator !=( const T rh ) const {
		return x != rh || y != rh || width != rh || height != rh;
	}

	bool operator >( const T rh ) const {
		return x > rh || y > rh || width > rh || height > rh;
	}

	bool operator <( const T rh ) const {
		return x < rh || y < rh || width < rh || height < rh;
	}

	T x = 0;
	T y = 0;
	T width = 0;
	T height = 0;
};


template< typename T >
std::ostream& operator <<( std::ostream& out, const RectT< T >& r ) {
	out << "Rect(" << r.x << ", " << r.y << ", " << r.width << ", " << r.height << ")";
	return out;
}

using Rect = RectT< int >;
using RectF = RectT< float >;
using RectD = RectT< double >;
using RectU = RectT< unsigned >;


class ELEMENT_API Color {
public:
	Color() = default;

	Color( int r, int g, int b ) : red( r ),
	                                green( g ),
	                                blue( b ),
	                                type( RGB ) {}

	Color( int r, int g, int b, float a ) : red( r ),
	                                         green( g ),
	                                         blue( b ),
	                                         alpha( a ),
	                                         type( RGBA ) {}

	Color( float h, float s, float l ) : hue( h ),
	                                      sat( s ),
	                                      light( l ),
	                                      type( HSL ) {}

	Color( float h, float s, float l, float a ) : hue( h ),
	                                               sat( s ),
	                                               light( l ),
	                                               alpha( a ),
	                                               type( HSLA ) {}

	bool operator==(const Color& rhs ) const {
		return red == rhs.red
				&& green == rhs.green
				&& blue == rhs.blue
				&& hue == rhs.hue
				&& sat == rhs.sat
				&& light == rhs.light
				&& alpha == rhs.alpha
				&& type == rhs.type;
	}

	bool operator!=(const Color& rhs ) const { return !( *this == rhs ); }

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

	friend class Pen;
	friend class Brush;

	friend std::ostream& operator <<( std::ostream& out, const Color& c );
};


inline std::ostream& operator <<( std::ostream& out, const Color& c ) {
	switch( c.type ) {
		case Color::RGB:
			out << "Color(RGB" << "(" << c.red << ", " << c.green << ", " << c.blue << "))";
			break;
		case Color::RGBA:
			out << "Color(RGBA" << "(" << c.red << ", " << c.green << ", " << c.blue << ", " << c.alpha << "))";
			break;
		case Color::HSL:
			out << "Color(HSL" << "(" << c.hue << ", " << c.sat << ", " << c.light << "))";
			break;
		case Color::HSLA:
			out << "Color(HSLA" << "(" << c.hue << ", " << c.sat << ", " << c.light << ", " << c.alpha << "))";
			break;
	}
	return out;
}


class ELEMENT_API Font {
public:
	Font() = default;
	bool operator==(const Font& rhs ) const {
		return f_face == rhs.f_face
				&& f_pixel_size == rhs.f_pixel_size
				&& f_point_size == rhs.f_point_size
				&& l_height == rhs.l_height
				&& f_align == rhs.f_align
				&& f_spacing == rhs.f_spacing;
	}

	bool operator!=(const Font& rhs ) const { return !( *this == rhs ); }

	//Font(std::string face);
	//Font(const Font &other);

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
