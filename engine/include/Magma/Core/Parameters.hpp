#pragma once

#include <Magma/Framework/Memory/Functor.hpp>

namespace Magma
{
	namespace Core
	{
		class Parameters final
		{
		public:
			static constexpr mfmU64 MaxParserCount = 256;
			static constexpr mfmU64 MaxParserNameSize = 32;

			Parameters();
			~Parameters();

			void AddParser(void* opaque, const mfsUTF8CodeUnit* param, Framework::Memory::Functor<mfsUTF8CodeUnit**(void* opaque, mfsUTF8CodeUnit** arg)> parser);

			void Parse(mfsUTF8CodeUnit** argv);

		private:
			struct
			{
				void* opaque;
				Framework::Memory::Functor<char**(void* opaque, char** arg)> func;
				mfsUTF8CodeUnit name[MaxParserNameSize];
			} m_parsers[MaxParserCount];
		};
	}
}