#pragma once

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

RESOURCE MODES:
	- STREAM;
	- TEMPORARY;
	- PERMANENT;
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
			///		Loaded chunk by chunk on a stream.
			/// </summary>
			Stream,

			/// <summary>
			///		Resource storage mode count.
			/// </summary>
			Count
		};

		/// <summary>
		///		Abstract class for storing resource data
		/// </summary>
		class Resource
		{
		public:
			Resource();
			virtual ~Resource();

			void Import();

		private:
			virtual void VirtualImport() = 0;

			bool m_onMemory;
		};
	}
}