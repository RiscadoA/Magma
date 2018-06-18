#pragma once

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		Keyboard key codes
			/// </summary>
			enum class Keyboard
			{
				Invalid = -1,

				Q,
				W,
				E,
				R,
				T,
				Y,
				U,
				I,
				O,
				P,
				A,
				S,
				D,
				F,
				G,
				H,
				J,
				K,
				L,
				Z,
				X,
				C,
				V,
				B,
				N,
				M,

				Num1,
				Num2,
				Num3,
				Num4,
				Num5,
				Num6,
				Num7,
				Num8,
				Num9,
				Num0,

				F1,
				F2,
				F3,
				F4,
				F5,
				F6,
				F7,
				F8,
				F9,
				F10,
				F11,
				F12,

				Escape,
				Tab,
				Caps,
				LShift,
				RShift,
				LControl,
				RControl,
				Alt,
				AltGr,
				Space,
				Enter,
				Backspace,
				Insert,
				Delete,
				Home,
				End,
				PageUp,
				PageDown,

				Count
			};

			/// <summary>
			///		Key modifiers
			/// </summary>
			enum class KeyModifiers
			{
				None = 0x00,
				Shift = 0x01,
				Control = 0x02,
				Alt = 0x04,
				System = 0x08,
			};

			inline KeyModifiers operator|(KeyModifiers lhs, KeyModifiers rhs) { return (KeyModifiers)((size_t)lhs | (size_t)rhs); }
			inline KeyModifiers operator&(KeyModifiers lhs, KeyModifiers rhs) { return (KeyModifiers)((size_t)lhs & (size_t)rhs); }
			inline KeyModifiers& operator|=(KeyModifiers& lhs, KeyModifiers rhs) { return lhs = (lhs | rhs); }
			inline KeyModifiers& operator&=(KeyModifiers& lhs, KeyModifiers rhs) { return lhs = (lhs & rhs); }
		}
	}
}