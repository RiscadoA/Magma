#pragma once

#include <glm/glm.hpp>

namespace Magma
{
	namespace GUI
	{
		/// <summary>
		///		Represents a single coordinate on the GUI
		/// </summary>
		struct Coordinate
		{
			float absolute = 0.0f;
			float relative = 0.0f;
		};

		/// <summary>
		///		Represents a point on the GUI
		/// </summary>
		struct Point
		{
			Coordinate x;
			Coordinate y;
		};

		/// <summary>
		///		Represents a bounding box
		/// </summary>
		struct BoundingBox
		{
			Coordinate top;
			Coordinate bottom;
			Coordinate left;
			Coordinate right;
		};
	}
}