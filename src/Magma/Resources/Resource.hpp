#pragma once

#include <atomic>

/*
RESOURCE TYPES:
	- SOUND EFFECT;
	- SOUND STREAM;
	- MESH;
	- MATERIAL;
	- SHADER;
	- TEXTURE;
	- SKELETON;
	- ANIMATION;
*/

namespace Magma
{
	namespace Resources
	{
		/// <summary>
		///		Resource storage modes.
		/// </summary>
		enum class ResourceMode
		{
			/// <summary>
			///		Invalid resource storage mode.
			/// </summary>
			Invalid = -1,

			/// <summary>
			///		Destroyed when there are no more references to it or when told to.
			/// </summary>
			CPUTemporary,

			/// <summary>
			///		Loaded chunk by chunk on a stream.
			/// </summary>
			CPUStream,

			/// <summary>
			///		Only destroyed on program termination or when told to.
			/// </summary>
			CPUPermanent,

			/// <summary>
			///		Destroyed when there are no more references to it or when told to.
			///		The resource moves from the CPU to the GPU on importation.
			/// </summary>
			GPUTemporary,

			/// <summary>
			///		Only destroyed on program termination or when told to.
			///		The resource moves from the CPU to the GPU on importation.
			/// </summary>
			GPUPermanent,

			/// <summary>
			///		Resource storage mode count.
			/// </summary>
			Count
		};

		class Resource;

		/// <summary>
		///		Holds resource data (abstract class)
		/// </summary>
		class ResourceData
		{
		public:
			ResourceData(Resource* resource);
			virtual ~ResourceData() = default;

			/// <summary>
			///		Gets the resource which this data belongs to
			/// </summary>
			/// <returns>Pointer to resource<</returns>
			inline Resource* GetResource() { return m_resource; }

			/// <summary>
			///		Gets the resource which this data belongs to
			/// </summary>
			/// <returns>Const pointer to resource<</returns>
			inline const Resource* GetResource() const { return m_resource; }

		private:
			Resource* m_resource;
		};

		/// <summary>
		///		Class used to represent a resource.
		/// </summary>
		class Resource
		{
		public:
			Resource();
			~Resource();

			/// <summary>
			///		Sets this resource's data pointer.
			/// </summary>
			/// <param name="data">Resource's data pointer</param>
			inline void SetData(ResourceData* data) { m_data = data; }

			/// <summary>
			///		Checks if this resource has data (if it has been imported)
			/// </summary>
			/// <returns>If it has, returns true, otherwise returns false</returns>
			inline bool HasData() const { return m_data != nullptr; }

			/// <summary>
			///		Get this resource's data
			/// </summary>
			/// <typeparam name="T">Resource data type</typeparam>
			/// <returns>Pointer to resource's data. Nullptr if type T doesn't match the resource data type.</returns>
			template <typename T>
			inline T* GetData() { return dynamic_cast<T*>(m_data); };

			/// <summary>
			///		Get this resource's data
			/// </summary>
			/// <typeparam name="T">Resource data type</typeparam>
			/// <returns>Const pointer to resource's data. Nullptr if type T doesn't match the resource data type.</returns>
			template <typename T>
			inline const T* GetData() const { return dynamic_cast<const T*>(m_data); };

			/// <summary>
			///		Gets the current number of references this resource has.
			/// </summary>
			inline size_t GetReferenceCount() const { return m_referenceCount; }

		private:
			friend class ResourceView;

			ResourceData* m_data;
			std::atomic<size_t> m_referenceCount;
		};

		/// <summary>
		///		Used to reference a resource.
		/// </summary>
		class ResourceView final
		{
		public:
			ResourceView(const ResourceView& rhs);
			ResourceView(ResourceView&& rhs);
			ResourceView(Resource& resource);
			~ResourceView();

			/// <summary>
			///		Gets a pointer to the resource this view points to.
			/// </summary>
			///  <returns>Resource pointer (guaranteed to never return nullptr)<returns>
			Resource& Get();

			/// <summary>
			///		Gets a const pointer to the resource this view points to.
			/// </summary>
			///  <returns>Const resource pointer (guaranteed to never return nullptr)<returns>
			const Resource& Get() const;

		private:
			Resource* m_resource;
		};
	}
}