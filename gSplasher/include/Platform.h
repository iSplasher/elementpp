#pragma once

#include "Global.h"

#if defined(OS_WINDOWS)
struct HWND__;
#endif

NAMESPACE_BEGIN

#ifdef OS_WINDOWS

	typedef HWND__* gWindowHandle;

#elif defined OS_LINUX

	typedef unsigned long gWindowHandle;

#elif defined OS_MAC

	typedef void* gWindowHandle;

#endif

class GSPLASHER_API Window
{
public:

	virtual ~Window() = 0;

	virtual gWindowHandle createHandle(int x, int y, int width, int height) = 0;
	virtual void processEvents() = 0;
	//void setTransparency(gWindowHandle handle, unsigned char alpha);
	//void redraw(gWindowHandle handle, int x, int y, int width, int height);
	virtual void close() = 0;
	virtual bool isOpen() = 0;
};

NAMESPACE_END