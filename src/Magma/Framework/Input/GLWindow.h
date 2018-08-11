#pragma once

#include "Error.h"
#include "Window.h"
#include "../String/UTF8.h"

/*
	Implementation of the abstract window type on Window.h using GLFW.
*/

#ifdef __cplusplus
extern "C"
{
#endif
	/// <summary>
	///		Creates a new window to use with OpenGL.
	/// </summary>
	/// <param name="window">Out window pointer</param>
	/// <param name="width">Window width</param>
	/// <param name="height">Window height</param>
	/// <param name="title">Pointer to window title string (UTF-8)</param>
	/// <returns>
	///		Returns MFI_ERROR_OKAY if there were no errors.
	///		Returns MFI_ERROR_ALLOCATION_FAILED if the window data struct allocation failed.
	///		Returns MFI_ERROR_INVALID_ARGUMENTS if any of the parameters are invalid.
	///		Returns MFI_ERROR_INTERNAL if the internal library failed to open the window.
	/// </returns>
	mfiError mfiCreateGLWindow(mfiWindow** window, mfmU32 width, mfmU32 height, mfiWindowMode mode, const mfsUTF8CodeUnit* title);

	/// <summary>
	///		Destroys a window previously created with mfiCreateGLWindow.
	/// </summary>
	/// <param name="window">Window pointer</param>
	void mfiDestroyGLWindow(void* window);

	/// <summary>
	///		Gets the underlying window GLFW handle.
	/// </summary>
	/// <param name="window">Window to get the handle from</param>
	/// <returns>GLFW window handle</returns>
	void* mfiGetGLWindowGLFWHandle(void* window);

#ifdef __cplusplus
}
#endif
