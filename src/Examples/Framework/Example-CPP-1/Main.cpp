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

	String::OutStream.PutString(str);
	String::OutStream.Flush();

	Magma::Framework::Terminate();

	while (true);
	return 0;
}
