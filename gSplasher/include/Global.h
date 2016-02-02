#pragma once

#include <memory>
#include <cassert>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

// macros


#define GSP_VERSION_MAJOR 0
#define GSP_VERSION_MINOR 1
#define GSP_VERSION_PATCH 0
#define USING_NAMESPACE using namespace gsp;
#define NAMESPACE_BEGIN namespace gsp {
#define NAMESPACE_END }


// TODO: define GSPLASHER_API to nothing when not on windows
//#ifdef GSPLASHER_EXPORTS
//	#define GSPLASHER_API __declspec(dllexport)
//#else
//	#define GSPLASHER_API __declspec(dllimport)
//#endif
#define GSPLASHER_API

/// <summary>
/// Define system
/// </summary>
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

NAMESPACE_BEGIN

// init logger
//#ifdef _DEBUG
//plog::Severity s = plog::debug;
//#else
//Severity s = plog::info;
//#endif
//static plog::RollingFileAppender<plog::TxtFormatter> file_appender("gSplasher.log");
////#ifdef _DEBUG // maybe a devel macro?
////static plog::ColorConsoleAppender<plog::TxtFormatter> console_appender;
////plog::init(s, &file_appender).addAppender(&console_appender);
////#else
////plog::init(s, &file_appender);
//
////#endif
//
//// redefine log macros
//#define LOG_D LOG(plog::debug)
//#define LOG_I LOG(plog::info)
//#define LOG_W LOG(plog::warning)
//#define LOG_E LOG(plog::error)
//#define LOG_C LOG(plog::fatal)
//// conditional
//#define LOG_D_IF(cond) LOG_IF(plog::debug, cond)
//#define LOG_I_IF(cond) LOG_IF(plog::info, cond)
//#define LOG_W_IF(cond) LOG_IF(plog::warning, cond)
//#define LOG_E_IF(cond) LOG_IF(plog::error, cond)
//#define LOG_C_IF(cond) LOG_IF(plog::fatal, cond)

NAMESPACE_END
