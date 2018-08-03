#pragma once

#include <string>

#include <Magma/Framework/Files/FileSystem.hpp>

namespace Magma
{
	namespace Resources
	{
		/// <summary>
		///		Holds the resource manager settings
		/// </summary>
		struct ManagerSettings
		{
			std::string rootPath;
		};

		/// <summary>
		///		Manages resources
		/// </summary>
		class Manager final
		{
		public:
			static void Init(const ManagerSettings& settings);
			static void Terminate();

		private:
			static Manager* s_manager;

			Manager(const ManagerSettings& settings);
			~Manager();

			ManagerSettings m_settings;

			Framework::Files::FileSystem* m_fileSystem;
		};
	}
}