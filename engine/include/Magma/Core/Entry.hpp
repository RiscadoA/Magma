#pragma once

#include <Magma/Framework/String/UTF8.hpp>
#include <Magma/Framework/Memory/Allocator.hpp>
#include <Magma/Framework/File/FileSystem.hpp>

namespace Magma
{
	namespace Core
	{
		/// <summary>
		///		Manages the Engine startup and termination.
		/// </summary>
		class Engine final
		{
		public:
			Engine(mfsUTF8CodeUnit** argv, Framework::Memory::HAllocator allocator);
			~Engine();

		private:
			static mfsUTF8CodeUnit** ParseDirectoryParam(void* opaque, mfsUTF8CodeUnit** arg);

			mfsUTF8CodeUnit m_rootDirectory[512];
			Framework::Memory::HAllocator m_allocator;

			Framework::File::HArchive m_gameArchive;
			Framework::File::HArchive m_userArchive;
		};
	}
}