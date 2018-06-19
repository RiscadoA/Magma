#pragma once

#include "Window.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		Window class implementation for DirectX (uses Windows API)
			/// </summary>
			class D3DWindow : public Window
			{
			public:
				/// <summary>
				///		Opens a window (OpenGL implementation).
				/// </summary>
				/// <param name="width">Window width</param>
				/// <param name="height">Window height</param>
				/// <param name="title">Window title</param>
				/// <param name="mode">Window mode</param>
				D3DWindow(unsigned int width, unsigned int height, const std::string& title, Window::Mode mode = Window::Mode::Windowed);

				/// <summary>
				///		Closes a window.
				/// </summary>
				~D3DWindow();

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
				///		Swaps the window buffers.
				/// </summary>
				virtual void SwapBuffers() final;

				/// <summary>
				///		Gets the windows API handle of this window.
				/// </summary>
				/// <returns>Window windows API handle</returns>
				inline void* GetHWND() const { return m_hwnd; }

			private:
				void* m_hwnd;
			};
		}
	}
}