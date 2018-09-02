#pragma once

#include "Importer.hpp"
#include "Exception.hpp"

#include <Magma/Framework/File/FileSystem.hpp>

#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>

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
			///		Loads the permament resources
			/// </summary>
			static void Load();
			
			/// <summary>
			///		Terminates the resource manager.
			/// </summary>
			static void Terminate();

			/// <summary>
			///		Cleans unused resources.
			/// </summary>
			static void Clean();

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
			template <typename T, typename ... TArgs>
			static void AddImporter(TArgs ... args);

			/// <summary>
			///		Gets the manager's file system
			/// </summary>
			inline Framework::File::FileSystem* GetFileSystem() { return m_fileSystem; }

		private:
			static Manager* s_manager;

			Manager(const ManagerSettings& settings);
			~Manager();

			void LoadMetaData(const Framework::File::Path& directory = "/");
			void CreateResources(const std::string& metaData);
			void Import(Resource* resource);
			void Destroy(Resource* resource);
			void Update();

			ManagerSettings m_settings;
			Framework::File::FileSystem* m_fileSystem;

			std::vector<Importer*> m_importers;
			std::vector<Resource*> m_resources;

			std::thread m_updateThread;
			std::atomic<bool> m_active = false;
		};

		template <typename T, typename ... TArgs>
		inline void Manager::AddImporter(TArgs ... args)
		{
			static_assert(std::is_base_of<Importer, T>::value);

			auto imp = new T(s_manager, args...);
			for (auto& i : s_manager->m_importers)
				if (i->GetName() == imp->GetName())
				{
					delete imp;
					std::stringstream ss;
					ss << "Failed to add importer to resources manager:" << std::endl;
					ss << "There is already an importer with the name '" << i->GetName() << "'";
					throw ManagerError(ss.str());
				}

			s_manager->m_importers.push_back(imp);
		}
	}
}