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
				///		Sets this window as the current for rendering.
				/// </summary>
				virtual void MakeCurrent() final;

				/// <summary>
				///		Polls events from this window.
				/// </summary>
				virtual void PollEvents() final;

				/// <summary>
				///		Waits for an event and handles it.
				/// </summary>
				virtual void WaitForEvents() final;

				/// <summary>
				///		Swaps the GLFW window buffers
				/// </summary>
				void SwapBuffers();

				/// <summary>
				///		Gets the width of the window.
				/// </summary>
				/// <returns>Window width</returns>
				inline virtual unsigned int GetWidth() final { return m_width; }

				/// <summary>
				///		Gets the height of the window.
				/// </summary>
				/// <returns>Window height</returns>
				inline virtual unsigned int GetHeight() final { return m_height; }

				/// <summary>
				///		Gets the window mode
				/// </summary>
				/// <returns>Window mode</returns>
				inline virtual Window::Mode GetMode() final { return m_mode; }

				/// <summary>
				///		Returns a pointer to the underlying GLFW window
				/// </summary>
				/// <returns>GLFW window pinter</returns>
				inline void* GetGLFWWindow() const { return m_glfwWindow; }

			private:
				void* m_glfwWindow;
				unsigned int m_width;
				unsigned int m_height;
				Window::Mode m_mode;
			};
		}
	}
}