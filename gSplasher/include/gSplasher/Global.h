#pragma once

#include <memory>
#include <cassert>

// macros

#define GSP_VERSION_MAJOR 0
#define GSP_VERSION_MINOR 1
#define GSP_VERSION_PATCH 0
#define USING_NAMESPACE using namespace gsp;
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

#ifdef OS_WINDOWS_D
	#ifdef GSPLASHER_EXPORTS
		#define GSPLASHER_API __declspec(dllexport)
	#else
		#define GSPLASHER_API __declspec(dllimport)
	#endif
#else
	#define GSPLASHER_API
#endif

NAMESPACE_BEGIN

enum class Orientation {
	Vertical,
	Horizontal
};

NAMESPACE_END