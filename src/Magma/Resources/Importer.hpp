#pragma once

#include "Resource.hpp"

#include <string>

namespace Magma
{
	namespace Resources
	{
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
			Importer(const std::string& name);

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
			///		Gets the importer name
			/// </summary>
			inline const std::string& GetName() { return m_name; }

		private:
			std::string m_name;
		};
	}
}