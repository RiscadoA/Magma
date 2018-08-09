#pragma once

#include "../Memory/Type.h"
#include "../Memory/Object.h"

#include "Keyboard.h"
#include "Mouse.h"

/*
	Abstract C window type.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#define MFI_WINDOWED		0x01
#define MFI_FULLSCREEN		0x02

	typedef mfmU32 mfiWindowMode;

	typedef mfmU32(*mfiGetWindowWidthFunc)(void*);
	typedef mfmU32(*mfiGetWindowHeightFunc)(void*);
	typedef mfiWindowMode(*mfiGetWindowModeFunc)(void*);

	typedef void(*mfiMakeWindowCurrentFunc)(void*);
	typedef void(*mfiWindowPollEventsFunc)(void*);
	typedef void(*mfiWindowWaitForEventsFunc)(void*);

	typedef void(*mfiOnCloseCallback)(void*);
	typedef void(*mfiOnMouseEnterCallback)(void*);
	typedef void(*mfiOnMouseLeaveCallback)(void*);
	typedef void(*mfiOnMouseMoveCallback)(void*, mfmF32, mfmF32);
	typedef void(*mfiOnMouseScrollCallback)(void*, mfmF32);
	typedef void(*mfiOnKeyUpCallback)(void*, mfiKeyCode, mfiKeyMods);
	typedef void(*mfiOnKeyDownCallback)(void*, mfiKeyCode, mfiKeyMods);
	typedef void(*mfiOnMouseUpCallback)(void*, mfiMouseButton);
	typedef void(*mfiOnMouseDownCallback)(void*, mfiMouseButton);

	typedef struct
	{
		mfmObject object;

		// Other functions
		mfiMakeWindowCurrentFunc makeCurrent;
		mfiWindowPollEventsFunc pollEvents;
		mfiWindowWaitForEventsFunc waitForEvents;

		// Getters
		mfiGetWindowWidthFunc getWidth;
		mfiGetWindowHeightFunc getHeight;
		mfiGetWindowModeFunc getMode;
	
		// Callbacks
		mfiOnCloseCallback onClose;
		mfiOnMouseEnterCallback onMouseEnter;
		mfiOnMouseLeaveCallback onMouseLeave;
		mfiOnMouseMoveCallback onMouseMove;
		mfiOnMouseScrollCallback onMouseScroll;
		mfiOnKeyUpCallback onKeyUp;
		mfiOnKeyDownCallback onKeyDown;
		mfiOnMouseUpCallback onMouseUp;
		mfiOnMouseDownCallback onMouseDown;
	} mfiWindow;

#ifdef __cplusplus
}
#endif
