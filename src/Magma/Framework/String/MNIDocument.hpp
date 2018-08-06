#pragma once

#include <string>
#include <vector>

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			/// <summary>
			///		Represents an entry in a MNI document.
			/// </summary>
			struct MNIEntry
			{
				std::string name;
				std::string value;
			};

			/// <summary>
			///		Represents a region in a MNI document
			/// </summary>
			struct MNIRegion
			{
				std::string name;
				std::vector<MNIEntry> entries;
			};

			/// <summary>
			///		Used to read from and write to MNI documents.
			/// </summary>
			class MNIDocument final
			{
			public:
				MNIDocument();
				~MNIDocument();

				/// <summary>
				///		Reads this document from a string.
				/// </summary>
				/// <param name="str">String containing the document</param>
				void Read(const std::string& str);

				/// <summary>
				///		Writes this document to a string
				/// </summary>
				void Write(std::string& str) const;

				/// <summary>
				///		Gets the regions on this document.
				/// </summary>
				/// <returns>Constant reference to the regions on this document</returns>
				inline const std::vector<MNIRegion>& GetRegions() const { return m_regions; }

				/// <summary>
				///		Gets a region from this document.
				/// </summary>
				/// <param name="name">Region name</param>
				/// <returns>Constant reference to the region</returns>
				const MNIRegion& GetRegion(const std::string& name) const;

				/// <summary>
				///		Gets an entry from this document.
				/// </summary>
				/// <param name="region">Region name</param>
				/// <param name="name">Entry name</param>
				/// <returns>Constant reference to the entry</returns>
				const MNIEntry& GetEntry(const std::string& region, const std::string& name) const;

				/// <summary>
				///		Adds a region to this document.
				/// </summary>
				/// <param name="name">Region name</param>
				void AddRegion(const std::string& name);

				/// <summary>
				///		Adds an entry to this document.
				/// </summary>
				/// <param name="region">Region name</param>
				/// <param name="name">Entry name</param>
				/// <param name="value">Entry value</param>
				void AddEntry(const std::string& region, const std::string& name, const std::string& value);

				/// <summary>
				///		Removes a region from this document.
				/// </summary>
				/// <param name="name">Region name</param>
				void RemoveRegion(const std::string& name);

				/// <summary>
				///		Removes an entry from this document.
				/// </summary>
				/// <param name="region">Region name</param>
				/// <param name="name">Entry name</param>
				void RemoveEntry(const std::string& region, const std::string& name);

				/// <summary>
				///		Clears this document.
				/// </summary>
				void Clear();

			private:
				std::vector<MNIRegion> m_regions;
			};
		}
	}
}