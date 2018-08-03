#pragma once

#include "Resource.hpp"

#include <string>

namespace Magma
{
	namespace Resources
	{
		class Manager;

		/// <summary>
		///		Abstract class for importing the multiple types of resources
		/// </summary>
		class Importer
		{
		public:
			/// <summary>
			///		Constructs a new importer of a certain type
			/// </summary>
			/// <param name="name">Importer type name</param>
			Importer(const std::string& name, Manager* manager);

			/// <summary>
			///		Deconstructs an importer
			/// </summary>
			virtual ~Importer();

			/// <summary>
			///		Updates the resources imported by this resources
			/// </summary>
			virtual void Update() = 0;

			/// <summary>
			///		Imports a resource
			/// </summary>
			virtual void Import(Resource* resource) = 0;

			/// <summary>
			///		Destroys a resource that was imported by this importer
			/// </summary>
			virtual void Destroy(Resource* resource) = 0;

			/// <summary>
			///		Gets this importer's name
			/// </summary>
			/// <returns>This importer's name</returns>
			inline const std::string& GetName() { return m_name; }

			/// <summary>
			///		Gets this importer's manager
			/// </summary>
			/// <returns>This importer's manager</returns>
			inline Manager* GetManager() { return m_manager; }

		private:
			std::string m_name;
			Manager* m_manager;
		};
	}
}