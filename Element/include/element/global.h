#pragma once

#include <memory>
#include <cassert>
#include <cmath>

// macros

#define ELP_VERSION_MAJOR 0
#define ELP_VERSION_MINOR 1
#define ELP_VERSION_PATCH 0
#define NAMESPACE elp
#define PRIV_NAMESPACE priv
#define USING_NAMESPACE using namespace NAMESPACE;
#define USING_NAMESPACE_PRIV using namespace NAMESPACE::PRIV_NAMESPACE;
#define PRIV_NAMESPACE_BEGIN namespace PRIV_NAMESPACE {
#define NAMESPACE_BEGIN namespace NAMESPACE {
#define NAMESPACE_END }

// Define system
#if defined(_WIN32)

#define OS_WINDOWS
// needed to avoid issues when including windows specific header
#ifndef NOMINMAX
#define NOMINMAX
#endif

#elif defined(__APPLE__) && defined(__MACH__)

#define OS_MAC

#elif defined(__linux__)

#define OS_LINUX

#endif

#ifndef _DEBUG
#ifdef ELEMENT_EXPORTS
		#define ELEMENT_API __declspec(dllexport)
#else
		#define ELEMENT_API __declspec(dllimport)
#endif
#else
#define ELEMENT_API
#endif

NAMESPACE_BEGIN
// enums 

// to allow scoped enum bitwise operations 

#define DEFINE_ENUM_FLAGS(T) \
enum class T;	\
inline T	operator	+	(T x, int y)	{	return static_cast<T>	(static_cast<int>(x) + static_cast<int>(y));	}; \
inline T	operator	&	(T x, T y)		{	return static_cast<T>	(static_cast<int>(x) & static_cast<int>(y));	}; \
inline T	operator	|	(T x, T y)		{	return static_cast<T>	(static_cast<int>(x) | static_cast<int>(y));	}; \
inline T	operator	^	(T x, T y)		{	return static_cast<T>	(static_cast<int>(x) ^ static_cast<int>(y));	}; \
inline T	operator	~	(T x)			{	return static_cast<T>	(~static_cast<int>(x));							}; \
inline T&	operator	&=	(T& x, T y)		{	x = x & y;	return x;	}; \
inline T&	operator	|=	(T& x, T y)		{	x = x | y;	return x;	}; \
inline T&	operator	^=	(T& x, T y)		{	x = x ^ y;	return x;	}; \
inline bool	 flags(T x)	{	return static_cast<int>(x) != 0;};

DEFINE_ENUM_FLAGS(Direction)


enum class Direction {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Left = 1,
	Top = 1 << 1,
	Right = 1 << 2,
	Bottom = 1 << 3,
	Default = 1 << 5
};

DEFINE_ENUM_FLAGS(ReadingDirection)


enum class ReadingDirection {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	LTR = 1,
	RTL = 1 << 1,
	Default = 1 << 2
};


DEFINE_ENUM_FLAGS(Orientation)


enum class Orientation {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Vertical = 1,
	Horizontal = 1 << 1,
	Reverse = 1 << 2,
	Default = 1 << 3
};

DEFINE_ENUM_FLAGS(Position)


enum class Position {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Absolute = 1,
	Relative = 1 << 1,
	Default = 1 << 2
};

DEFINE_ENUM_FLAGS(Justify)


enum class Justify {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Start = 1,
	Center = 1 << 1,
	End = 1 << 2,
	SpaceBetween = 1 << 3,
	SpaceAround = 1 << 4,
	Default = 1 << 5,
};

DEFINE_ENUM_FLAGS(Align)


enum class Align {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Start = 1,
	Center = 1 << 1,
	End = 1 << 2,
	Stretch = 1 << 3,
	Default = 1 << 4,
};


DEFINE_ENUM_FLAGS(Overflow)


enum class Overflow {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Visible = 1,
	Hidden = 1 << 1,
	Scroll = 1 << 2,
	Default = 1 << 3
};


DEFINE_ENUM_FLAGS(Egde)


enum class Edge {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Start = 1,
	End = 1 << 1,
	Left = 1 << 2,
	Top = 1 << 3,
	Right = 1 << 4,
	Bottom = 1 << 5,
	HCenter = 1 << 6,
	VCenter = 1 << 7,
	Default = 1 << 8,
	All = 1 << 9
};


DEFINE_ENUM_FLAGS(MouseButton)


enum class MouseButton {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Left = 1,
	Right = 1 << 2,
	Middle = 1 << 3
};


DEFINE_ENUM_FLAGS(KeyModifier)


enum class KeyModifier {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Shift = 1,
	Control = 1 << 2,
	Alt = 1 << 3,
	Meta = 1 << 4
};

DEFINE_ENUM_FLAGS(Cursor)


enum class Cursor {
	Any = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Arrow = 1,
	IBeam = 1 << 2,
	Cross = 1 << 3,
	Hand = 1 << 4,
	HResize = 1 << 5,
	VResize = 1 << 6
};



class Application;

// A pointer to the application instance
#define App Application::instance()
#define REQUIRE_APP 

NAMESPACE_END
