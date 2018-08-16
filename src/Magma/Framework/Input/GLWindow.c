#include "GLWindow.h"
#include "Config.h"

#include <stdio.h>

#ifdef MAGMA_FRAMEWORK_USE_OPENGL

#include <GLFW/glfw3.h>

typedef struct
{
	// Base window data
	mfiWindow base;

	// Properties
	mfmU32 width;
	mfmU32 height;
	mfiWindowMode mode;

	// GLFW window handle
	GLFWwindow* glfwWindow;
} mfiGLWindow;

static mfiGLWindow* currentWindow = NULL;

mfiKeyCode mfiGLFWToMFIKey(int key)
{
	switch (key)
	{
		case GLFW_KEY_Q: return MFI_KEYBOARD_Q;
		case GLFW_KEY_W: return MFI_KEYBOARD_W;
		case GLFW_KEY_E: return MFI_KEYBOARD_E;
		case GLFW_KEY_R: return MFI_KEYBOARD_R;
		case GLFW_KEY_T: return MFI_KEYBOARD_T;
		case GLFW_KEY_Y: return MFI_KEYBOARD_Y;
		case GLFW_KEY_U: return MFI_KEYBOARD_U;
		case GLFW_KEY_I: return MFI_KEYBOARD_I;
		case GLFW_KEY_O: return MFI_KEYBOARD_O;
		case GLFW_KEY_P: return MFI_KEYBOARD_P;
		case GLFW_KEY_A: return MFI_KEYBOARD_A;
		case GLFW_KEY_S: return MFI_KEYBOARD_S;
		case GLFW_KEY_D: return MFI_KEYBOARD_D;
		case GLFW_KEY_F: return MFI_KEYBOARD_F;
		case GLFW_KEY_G: return MFI_KEYBOARD_G;
		case GLFW_KEY_H: return MFI_KEYBOARD_H;
		case GLFW_KEY_J: return MFI_KEYBOARD_J;
		case GLFW_KEY_K: return MFI_KEYBOARD_K;
		case GLFW_KEY_L: return MFI_KEYBOARD_L;
		case GLFW_KEY_Z: return MFI_KEYBOARD_Z;
		case GLFW_KEY_X: return MFI_KEYBOARD_X;
		case GLFW_KEY_C: return MFI_KEYBOARD_C;
		case GLFW_KEY_V: return MFI_KEYBOARD_V;
		case GLFW_KEY_B: return MFI_KEYBOARD_B;
		case GLFW_KEY_N: return MFI_KEYBOARD_N;
		case GLFW_KEY_M: return MFI_KEYBOARD_M;

		case GLFW_KEY_1: return MFI_KEYBOARD_NUM1;
		case GLFW_KEY_2: return MFI_KEYBOARD_NUM2;
		case GLFW_KEY_3: return MFI_KEYBOARD_NUM3;
		case GLFW_KEY_4: return MFI_KEYBOARD_NUM4;
		case GLFW_KEY_5: return MFI_KEYBOARD_NUM5;
		case GLFW_KEY_6: return MFI_KEYBOARD_NUM6;
		case GLFW_KEY_7: return MFI_KEYBOARD_NUM7;
		case GLFW_KEY_8: return MFI_KEYBOARD_NUM8;
		case GLFW_KEY_9: return MFI_KEYBOARD_NUM9;
		case GLFW_KEY_0: return MFI_KEYBOARD_NUM0;

		case GLFW_KEY_F1: return MFI_KEYBOARD_F1;
		case GLFW_KEY_F2: return MFI_KEYBOARD_F2;
		case GLFW_KEY_F3: return MFI_KEYBOARD_F3;
		case GLFW_KEY_F4: return MFI_KEYBOARD_F4;
		case GLFW_KEY_F5: return MFI_KEYBOARD_F5;
		case GLFW_KEY_F6: return MFI_KEYBOARD_F6;
		case GLFW_KEY_F7: return MFI_KEYBOARD_F7;
		case GLFW_KEY_F8: return MFI_KEYBOARD_F8;
		case GLFW_KEY_F9: return MFI_KEYBOARD_F9;
		case GLFW_KEY_F10: return MFI_KEYBOARD_F10;
		case GLFW_KEY_F11: return MFI_KEYBOARD_F11;
		case GLFW_KEY_F12: return MFI_KEYBOARD_F12;

		case GLFW_KEY_ESCAPE: return MFI_KEYBOARD_ESCAPE;
		case GLFW_KEY_TAB: return MFI_KEYBOARD_TAB;
		case GLFW_KEY_CAPS_LOCK: return MFI_KEYBOARD_CAPS;
		case GLFW_KEY_LEFT_SHIFT: return MFI_KEYBOARD_LSHIFT;
		case GLFW_KEY_RIGHT_SHIFT: return MFI_KEYBOARD_RSHIFT;
		case GLFW_KEY_LEFT_CONTROL: return MFI_KEYBOARD_LCONTROL;
		case GLFW_KEY_RIGHT_CONTROL: return MFI_KEYBOARD_RCONTROL;
		case GLFW_KEY_LEFT_ALT: return MFI_KEYBOARD_ALT;
		case GLFW_KEY_RIGHT_ALT: return MFI_KEYBOARD_ALTGR;
		case GLFW_KEY_SPACE: return MFI_KEYBOARD_SPACE;
		case GLFW_KEY_ENTER: return MFI_KEYBOARD_ENTER;
		case GLFW_KEY_BACKSPACE: return MFI_KEYBOARD_BACKSPACE;
		case GLFW_KEY_INSERT: return MFI_KEYBOARD_INSERT;
		case GLFW_KEY_DELETE: return MFI_KEYBOARD_DELETE;
		case GLFW_KEY_HOME: return MFI_KEYBOARD_HOME;
		case GLFW_KEY_END: return MFI_KEYBOARD_END;
		case GLFW_KEY_PAGE_UP: return MFI_KEYBOARD_PAGEUP;
		case GLFW_KEY_PAGE_DOWN: return MFI_KEYBOARD_PAGEDOWN;

		case GLFW_KEY_UP: return MFI_KEYBOARD_UP;
		case GLFW_KEY_DOWN: return MFI_KEYBOARD_DOWN;
		case GLFW_KEY_LEFT: return MFI_KEYBOARD_LEFT;
		case GLFW_KEY_RIGHT: return MFI_KEYBOARD_RIGHT;

		default: return MFI_KEYBOARD_INVALID;
	}
}

void mfiGLWindowPollEvents(void* window)
{
	glfwPollEvents();
}

void mfiGLWindowWaitForEvents(void* window)
{
	glfwWaitEvents();
}

mfmU32 mfiGetGLWindowWidth(void* window)
{
	mfiGLWindow* glWindow = (mfiGLWindow*)window;
	return glWindow->width;
}

mfmU32 mfiGetGLWindowHeight(void* window)
{
	mfiGLWindow* glWindow = (mfiGLWindow*)window;
	return glWindow->height;
}

mfiWindowMode mfiGetGLWindowMode(void* window)
{
	mfiGLWindow* glWindow = (mfiGLWindow*)window;
	return glWindow->mode;
}

void mfiGLFWErrorCallback(int err, const char* errMsg)
{
	fprintf(stderr, "mfiGLWindow GLFW error caught:\n%s\n", errMsg);
}

void mfiGLFWWindowCloseCallback(GLFWwindow* window)
{
	if (currentWindow->base.onClose != NULL)
		currentWindow->base.onClose(currentWindow);
}

void mfiGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	mfiKeyCode keyCode = mfiGLFWToMFIKey(key);

	switch (action)
	{
		case GLFW_PRESS:
			if (currentWindow->base.onKeyDown != NULL)
				currentWindow->base.onKeyDown(currentWindow, keyCode, mods);
			break;
		case GLFW_RELEASE:
			if (currentWindow->base.onKeyUp != NULL)
				currentWindow->base.onKeyUp(currentWindow, keyCode, mods);
			break;
	}
}

void mfiGLFWMousePositionCallback(GLFWwindow* window, double x, double y)
{
	if (currentWindow->base.onMouseMove != NULL)
		currentWindow->base.onMouseMove(currentWindow, (mfmF32)x, (mfmF32)y);
}

void mfiGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	mfiMouseButton mouseButton = MFI_MOUSE_INVALID;

	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
			mouseButton = MFI_MOUSE_LEFT;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouseButton = MFI_MOUSE_RIGHT;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			mouseButton = MFI_MOUSE_MIDDLE;
			break;
	}

	switch (action)
	{
		case GLFW_PRESS:
			if (currentWindow->base.onMouseDown != NULL)
				currentWindow->base.onMouseDown(currentWindow, mouseButton);
			break;
		case GLFW_RELEASE:
			if (currentWindow->base.onMouseUp != NULL)
				currentWindow->base.onMouseUp(currentWindow, mouseButton);
			break;
	}
}

void mfiGLFWMouseScroll(GLFWwindow* window, double x, double y)
{
	if (currentWindow->base.onMouseScroll != NULL)
		currentWindow->base.onMouseScroll(currentWindow, (mfmF32)y);
}

void mfiGLFWCursorEnterCallback(GLFWwindow* window, int enter)
{
	if (enter == GLFW_TRUE)
	{
		if (currentWindow->base.onMouseEnter != NULL)
			currentWindow->base.onMouseEnter(currentWindow);
	}
	else
	{
		if (currentWindow->base.onMouseLeave != NULL)
			currentWindow->base.onMouseLeave(currentWindow);
	}
}

#endif

mfiError mfiCreateGLWindow(mfiWindow ** window, mfmU32 width, mfmU32 height, mfiWindowMode mode, const mfsUTF8CodeUnit * title)
{
#ifdef MAGMA_FRAMEWORK_USE_OPENGL
	if (currentWindow != NULL)
		return MFI_ERROR_ALREADY_INITIALIZED;

	mfiGLWindow* glWindow = malloc(sizeof(mfiGLWindow));
	if (glWindow == NULL)
		return MFI_ERROR_ALLOCATION_FAILED;

	// Init GLFW
	glfwSetErrorCallback(mfiGLFWErrorCallback);
	if (glfwInit() != GLFW_TRUE)
	{
		// Failed to init
		free(glWindow);
		return MFI_ERROR_INTERNAL;
	}

	// Set properties
	glWindow->base.type = MFI_OGLWINDOW;

	glWindow->width = width;
	glWindow->height = height;
	glWindow->mode = mode;

	// Set destructor
	glWindow->base.object.destructorFunc = &mfiDestroyGLWindow;
	glWindow->base.object.referenceCount = 0;

	// Set functions
	glWindow->base.pollEvents = &mfiGLWindowPollEvents;
	glWindow->base.waitForEvents = &mfiGLWindowWaitForEvents;

	// Set getters
	glWindow->base.getWidth = &mfiGetGLWindowWidth;
	glWindow->base.getHeight = &mfiGetGLWindowHeight;
	glWindow->base.getMode = &mfiGetGLWindowMode;

	// Set callbacks to NULL
	glWindow->base.onClose = NULL;
	glWindow->base.onMouseEnter = NULL;
	glWindow->base.onMouseLeave = NULL;
	glWindow->base.onMouseMove = NULL;
	glWindow->base.onMouseScroll = NULL;
	glWindow->base.onKeyUp = NULL;
	glWindow->base.onKeyDown = NULL;
	glWindow->base.onMouseUp = NULL;
	glWindow->base.onMouseDown = NULL;

	// Set GLFW window hints for the window creation
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Create GLFW window
	if (mode == MFI_WINDOWED)
		glWindow->glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	else if (mode == MFI_FULLSCREEN)
		glWindow->glfwWindow = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
	else
	{
		free(glWindow);
		return MFI_ERROR_INVALID_ARGUMENTS;
	}

	// Check if the window creation succeded
	if (glWindow->glfwWindow == NULL)
	{
		free(glWindow);
		return MFI_ERROR_INTERNAL;
	}

	// Make window current
	glfwMakeContextCurrent(glWindow->glfwWindow);

	// Set window callbacks
	glfwSetKeyCallback(glWindow->glfwWindow, mfiGLFWKeyCallback);
	glfwSetCursorPosCallback(glWindow->glfwWindow, mfiGLFWMousePositionCallback);
	glfwSetWindowCloseCallback(glWindow->glfwWindow, mfiGLFWWindowCloseCallback);
	glfwSetScrollCallback(glWindow->glfwWindow, mfiGLFWMouseScroll);
	glfwSetCursorEnterCallback(glWindow->glfwWindow, mfiGLFWCursorEnterCallback);
	glfwSetMouseButtonCallback(glWindow->glfwWindow, mfiGLFWMouseButtonCallback);

	// Set out window pointer
	*window = glWindow;
	currentWindow = glWindow;

	return MFI_ERROR_OKAY;
#else
	return MFI_ERROR_NOT_SUPPORTED;
#endif
}

void mfiDestroyGLWindow(void * window)
{
#ifdef MAGMA_FRAMEWORK_USE_OPENGL
	// Destroy window
	mfiGLWindow* glWindow = (mfiGLWindow*)window;
	glfwDestroyWindow(glWindow->glfwWindow);
	free(glWindow);
	currentWindow = NULL;

	// Terminate GLFW
	glfwTerminate();
#endif
}

void * mfiGetGLWindowGLFWHandle(void * window)
{
#ifdef MAGMA_FRAMEWORK_USE_OPENGL
	mfiGLWindow* glWindow = (mfiGLWindow*)window;
	return glWindow->glfwWindow;
#else
	return NULL;
#endif
}

#if !defined(MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT)
int main(int argc, char** argv)
{
	mfEntry(argc, argv);
}
#endif
