#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Memory/Handle.hpp>
#include <Magma/Framework/String/StringStream.hpp>

#include <cstdlib>

using namespace Magma::Framework;

int main(int argc, char** argv)
{
	Magma::Framework::Init(argc, argv);

	{
		mfsUTF8CodeUnit buffer[256];

		{
			auto ss = String::StringStream(buffer, sizeof(buffer));
			auto h = ss.Get();
			h.PutString(u8"test");
		}

		String::OutStream.PutString(buffer);
		String::InStream.ReadUntil(nullptr, 0, u8"abc");
	}

	Magma::Framework::Terminate();
	return 0;
}
