#pragma once

#include "Error.h"
#include "Window.h"
#include "../String/UTF8.h"

/*
	Implementation of the abstract window type on WindowHandle.h using the Windows API.
*/

#define MFI_D3DWINDOW_TYPE_NAME u8"d3d"

#ifdef __cplusplus
extern "C"
{
#endif
	/// <summary>
	///		Creates a new window to use with Direct3D.
	/// </summary>
	/// <param name="window">Out window pointer</param>
	/// <param name="width">WindowHandle width</param>
	/// <param name="height">WindowHandle height</param>
	/// <param name="title">Pointer to window title string (UTF-8)</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFI_ERROR_ALLOCATION_FAILED if the window data struct allocation failed.
	///		Returns MFI_ERROR_INVALID_ARGUMENTS if any of the parameters are invalid.
	///		Returns MFI_ERROR_INTERNAL if the internal library failed to open the window.
	/// </returns>
	mfError mfiCreateD3DWindow(mfiWindow** window, mfmU32 width, mfmU32 height, mfiEnum mode, const mfsUTF8CodeUnit* title);

	/// <summary>
	///		Destroys a window previously created with mfiCreateD3DWindow.
	/// </summary>
	/// <param name="window">WindowHandle pointer</param>
	void mfiDestroyD3DWindow(void* window);

	/// <summary>
	///		Gets the underlying window HWND.
	/// </summary>
	/// <param name="window">WindowHandle to get the HWND from</param>
	/// <returns>WindowHandle HWND</returns>
	void* mfiGetD3DWindowHandle(void* window);

#ifdef __cplusplus
}
#endif
