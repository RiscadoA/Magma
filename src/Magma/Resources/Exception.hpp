#pragma once

#include <stdexcept>

namespace Magma
{
	namespace Resources
	{
		/// <summary>
		///		Thrown when there is a resources manager related error
		/// </summary>
		class ManagerError : public std::runtime_error
		{
		public:
			using std::runtime_error::runtime_error;
		};

		/// <summary>
		///		Thrown when there is a resource related error
		/// </summary>
		class ResourceError : public std::runtime_error
		{
		public:
			using std::runtime_error::runtime_error;
		};
	}
}