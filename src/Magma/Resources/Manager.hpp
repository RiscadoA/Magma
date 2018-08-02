#pragma once

namespace Magma
{
	namespace Resources
	{
		/// <summary>
		///		Manages resources
		/// </summary>
		class Manager final
		{
		public:
			static void Init();
			static void Terminate();

		private:
			static Manager* s_manager;

			Manager();
			~Manager();
		};
	}
}