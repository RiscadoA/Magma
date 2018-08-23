#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Memory/Object.hpp>
#include <Magma/Framework/String/Stream.hpp>

#include <cstdlib>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	mfsUTF8CodeUnit str[256];
	String::InStream.ReadUntil(str, sizeof(str), u8"\n");

	String::Stream file = String::OpenFile(u8"test.txt", String::FileMode::Write);

	file.PutString(str);
	file.Flush();

	file.Release();

	Magma::Framework::Terminate();

	return 0;
}
