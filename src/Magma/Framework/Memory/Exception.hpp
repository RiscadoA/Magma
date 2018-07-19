#pragma once

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Thrown when there is an error related to memory allocation
			/// </summary>
			class AllocationError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};
		}
	}
}
