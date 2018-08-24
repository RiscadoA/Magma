#pragma once

#include "../Memory/Type.h"
#include "../Memory/Object.h"

#include "Keyboard.h"
#include "Mouse.h"
#include "Error.h"

#include "../String/UTF8.h"

/*
	Abstract C window type.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#define MFI_WINDOWED		0x01
#define MFI_FULLSCREEN		0x02

#define MFI_MAX_WINDOW_CREATOR_REGISTER_ENTRIES 16

	typedef mfmU32 mfiEnum;

	typedef mfmU32(*mfiGetWindowWidthFunc)(void*);
	typedef mfmU32(*mfiGetWindowHeightFunc)(void*);
	typedef mfiEnum(*mfiGetWindowModeFunc)(void*);

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

		const mfsUTF8CodeUnit* type;

		// Reserved for the user to use
		mfmU64 userAttribute;

		// Other functions
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

	typedef mfError(*mfiWindowCreatorFunction)(mfiWindow** window, mfmU32 width, mfmU32 height, mfiEnum mode, const mfsUTF8CodeUnit* title);

	/// <summary>
	///		Inits the windows library.
	/// </summary>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfiInitWindows();

	/// <summary>
	///		Terminates the windows library.
	/// </summary>
	void mfiTerminateWindows();

	/// <summary>
	///		Registers a new window creator.
	/// </summary>
	/// <param name="type">Window type name (with a maximum size of 16 bytes)</param>
	/// <param name="func">Window creator function</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFI_ERROR_NO_REGISTER_ENTRIES if there are no more creator slots in the register.
	/// </returns>
	mfError mfiRegisterWindowCreator(const mfsUTF8CodeUnit* type, mfiWindowCreatorFunction func);

	/// <summary>
	///		Creates a new window.
	/// </summary>
	/// <param name="type">Window type name</param>
	/// <param name="width">Window width</param>
	/// <param name="height">Window height</param>
	/// <param name="mode">Window mode</param>
	/// <param name="title">Window title</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFI_ERROR_TYPE_NOT_REGISTERED if there isn't a creator with the type registered.
	///		Otherwise returns a window creation error code.
	/// </returns>
	mfError mfiCreateWindow(const mfsUTF8CodeUnit* type, mfiWindow** window, mfmU32 width, mfmU32 height, mfiEnum mode, const mfsUTF8CodeUnit* title);

	/// <summary>
	///		Destroys a window.
	/// </summary>
	void mfiDestroyWindow(void* window);

#ifdef __cplusplus
}
#endif
