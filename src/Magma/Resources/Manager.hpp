#pragma once

#include "Importer.hpp"
#include "Exception.hpp"

#include <Magma/Framework/Files/FileSystem.hpp>

#include <sstream>
#include <string>
#include <vector>

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
			/// <summary>
			/// /	Inits the resource manager.
			/// </summary>
			/// <param name="settings">Resource manager settings</param>
			static void Init(const ManagerSettings& settings);

			/// <summary>
			///		Terminates the resource manager.
			/// </summary>
			static void Terminate();

			/// <summary>
			///		Gets a resource from the resource manager.
			/// </summary>
			/// <param name="name">Resource name</param>
			/// <returns>Resource view</returns>
			static ResourceView GetResource(const std::string& name);

			/// <summary>
			///		Destroys a resource from the resource manager
			/// </summary>
			/// <param name="name"></param>
			static void DestroyResource(const std::string& name);

			/// <summary>
			///		Adds a resource importer to the resource manager
			/// </summary>
			/// <typeparam name="T">Importer type</typeparam>
			template <typename T>
			static void AddImporter();

			/// <summary>
			///		Gets the manager's file system
			/// </summary>
			inline Framework::Files::FileSystem* GetFileSystem() { return m_fileSystem; }

		private:
			static Manager* s_manager;

			Manager(const ManagerSettings& settings);
			~Manager();

			ManagerSettings m_settings;
			Framework::Files::FileSystem* m_fileSystem;

			std::vector<Importer*> m_importers;
		};

		template<typename T>
		inline void Manager::AddImporter()
		{
			static_assert(std::is_base_of<Importer, T>::value);

			auto imp = new T(this);
			for (auto& i : m_importers)
				if (i->GetName() == imp->GetName())
				{
					delete imp;
					std::stringstream ss;
					ss << "Failed to add importer to resources manager:" << std::endl;
					ss << "There is already an importer with the name '" << i->GetName() << "'";
					throw ManagerError(ss.str());
				}

			m_importers.push_back(imp);
		}
	}
}