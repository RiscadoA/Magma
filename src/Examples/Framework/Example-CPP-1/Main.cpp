#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Memory/Object.hpp>
#include <Magma/Framework/String/Stream.hpp>
#include <Magma/Framework/String/StringStream.hpp>

#include <cstdlib>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	mfsUTF8CodeUnit buffer[256];
	memset(buffer, 0, sizeof(buffer));

	String::Stream ss = String::CreateStringStream(buffer, sizeof(buffer));
	ss.PutString(u8"Sample text");
	ss.Release();

	String::OutStream.PutString(buffer);

	mfmU8 byte;
	String::InStream.GetByte(byte);

	Magma::Framework::Terminate();
	return 0;
}
