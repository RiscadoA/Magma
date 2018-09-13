#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Memory/Handle.hpp>
#include <Magma/Framework/String/Stream.hpp>

#include <cstdlib>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	String::OutStream.Print(u8"test: ");
	String::OutStream.Print(2.0f);
	String::OutStream.Print(u8" ; ");
	String::OutStream.Print(2.2582);
	String::OutStream.Print(u8" ; ");
	String::OutStream.Print(-4298);
	String::OutStream.Print(u8" ; ");
	String::OutStream.Print('t');

	String::InStream.ReadUntil(nullptr, 0, u8"\n");

	Magma::Framework::Terminate();
	return 0;
}
