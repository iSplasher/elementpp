#pragma once

#include <memory>
#include <cassert>

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
	Left = 1,
	Top = 1 << 1,
	Right = 1 << 2,
	Bottom = 1 << 3,
	Reverse = 1 << 4,
	Default = 1 << 5
};


DEFINE_ENUM_FLAGS(Orientation)


enum class Orientation {
	Vertical = 1,
	Horizontal = 1 << 1,
	Reverse = 1 << 2,
	Default = 1 << 3
};


DEFINE_ENUM_FLAGS(Alignment)


enum class Alignment {
	Start = 1,
	End = 1 << 1,
	Left = 1 << 2,
	Top = 1 << 3,
	Right = 1 << 4,
	Bottom = 1 << 5,
	Center = 1 << 6,
	Default = 1 << 7
};


DEFINE_ENUM_FLAGS(MouseButton)


enum class MouseButton {
	All = 0xF, // 28bits = 0xFFFFFFFF,
	None = 0x0,
	Left = 1,
	Right = 1 << 2,
	Middle = 1 << 3
};


DEFINE_ENUM_FLAGS(KeyModifier)


enum class KeyModifier {
	None = 0x0,
	Shift = 1,
	Control = 1 << 2,
	Alt = 1 << 3,
	Meta = 1 << 4
};


NAMESPACE_END
