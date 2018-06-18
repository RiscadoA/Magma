#pragma once

#include "Window.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		Window class implementation for OpenGL (uses GLFW)
			/// </summary>
			class GLWindow : public Window
			{
			public:
				/// <summary>
				///		Opens a window (OpenGL implementation).
				/// </summary>
				/// <param name="width">Window width</param>
				/// <param name="height">Window height</param>
				/// <param name="title">Window title</param>
				/// <param name="mode">Window mode</param>
				GLWindow(unsigned int width, unsigned int height, const std::string& title, Window::Mode mode = Window::Mode::Windowed);

				/// <summary>
				///		Closes a window.
				/// </summary>
				~GLWindow();

				/// <summary>
				///		Polls events from this window.
				/// </summary>
				virtual void PollEvents() final;

				/// <summary>
				///		Waits for an event and handles it.
				/// </summary>
				virtual void WaitForEvents() final;

				/// <summary>
				///		Swaps the window buffers.
				/// </summary>
				virtual void SwapBuffers() final;

			private:
				void* m_glfwWindow;
			};
		}
	}
}