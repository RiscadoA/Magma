#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Memory/Handle.hpp>
#include <Magma/Framework/String/Stream.hpp>

#include <cstdlib>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	String::OutStream << "Enter x: ";
	mfmF32 x = String::InStream.ParseF32(u8"\n");
	String::OutStream << "Enter y: ";
	mfmF64 y = String::InStream.ParseF64(u8"\n");

	String::OutStream << x << " + " << y << " = " << (x + y) << "\n";
	String::InStream.ReadUntil(nullptr, 0, u8"\n");

	Magma::Framework::Terminate();
	return 0;
}
