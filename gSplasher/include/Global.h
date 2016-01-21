#pragma once

#include <memory>

// macros


#define GSP_VERSION_MAJOR 0
#define GSP_VERSION_MINOR 1
#define USING_NAMESPACE using namespace gsp;
#define NAMESPACE_BEGIN namespace gsp {
#define NAMESPACE_END }


// TODO: define GSPLASHER_API to nothing when not on windows
#ifdef GSPLASHER_EXPORTS
	#define GSPLASHER_API __declspec(dllexport)
#else
	#define GSPLASHER_API __declspec(dllimport)
#endif

