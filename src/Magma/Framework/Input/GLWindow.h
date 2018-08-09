#pragma once

#include "Window.h"

/*
	Implementation of the abstract window type on Window.h using GLFW.
*/

#ifdef __cplusplus
extern "C"
{
#endif

	void mfiCreateGLWindow(mfiWindow** window, mfmU32 width, mfmU32 height, mfiWindowMode mode, const mfmU8* title);

#ifdef __cplusplus
}
#endif
