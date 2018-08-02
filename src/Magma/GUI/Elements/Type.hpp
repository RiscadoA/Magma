#pragma once

namespace Magma
{
	namespace GUI
	{
		namespace Elements
		{
			/// <summary>
			///		Represents an element type
			/// </summary>
			enum class Type
			{
				Invalid = -1,

				Root,
				Null,
				Box,
				Text,

				Count
			};
		}
	}
}