#pragma once

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <memory>
#include <cassert>
#include <cmath>

// macros

#define EL_VERSION_MAJOR 0
#define EL_VERSION_MINOR 1
#define EL_VERSION_PATCH 0
#define LIB_NAMESPACE elm
#define LIB_PRIVATE_NAMESPACE detail
#define USING_LIB_NAMESPACE using namespace LIB_NAMESPACE;
#define USING_PRIVATE_LIB_NAMESPACE using namespace LIB_NAMESPACE::LIB_PRIVATE_NAMESPACE;
#define PRIVATE_LIB_NAMESPACE_BEGIN namespace LIB_PRIVATE_NAMESPACE {
#define LIB_NAMESPACE_BEGIN namespace LIB_NAMESPACE {
#define LIB_NAMESPACE_END }

// Define system
#if defined(_WIN32)

#define OS_WINDOWS
// needed to avoid issues when including windows specific header
#ifndef NOMINMAX
#define NOMINMAX
#endif

// needed for FLTK
#ifndef WIN32
#define WIN32 _WIN32
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

#ifdef _WIN32
#define WIN32 _WIN32
#endif