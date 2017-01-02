#pragma once

#include <memory>
#include <cassert>

// macros

#define GSP_VERSION_MAJOR 0
#define GSP_VERSION_MINOR 1
#define GSP_VERSION_PATCH 0
#define USING_NAMESPACE using namespace gsp;
#define USING_NAMESPACE_PRIV using namespace gsp::priv;
#define NAMESPACE_BEGIN namespace gsp {
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
	#ifdef GSPLASHER_EXPORTS
		#define GSPLASHER_API __declspec(dllexport)
	#else
		#define GSPLASHER_API __declspec(dllimport)
	#endif
#else
	#define GSPLASHER_API
#endif

NAMESPACE_BEGIN

// enums 

// to allow scoped enum bitwise operations 

#define DEFINE_ENUM_FLAGS(T) \
enum class T;	\
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
	Left,
	Top,
	Right,
	Bottom,
};

DEFINE_ENUM_FLAGS(Orientation)
enum class Orientation {
	Vertical,
	Horizontal
};

DEFINE_ENUM_FLAGS(Alignment)
enum class Alignment {
	Left,
	Top,
	Right,
	Bottom,
	HCenter,
	VCenter,
	Default
};

DEFINE_ENUM_FLAGS(MouseButton)
enum class MouseButton {
	All = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Left = 0x1,
	Right = 0x2,
	Middle = 0x4
};

DEFINE_ENUM_FLAGS(KeyModifier)
enum class KeyModifier {
	None = 0x0,
	Shift = 0x1,
	Control = 0x2,
	Alt = 0x4,
	Meta = 0x8
};


NAMESPACE_END