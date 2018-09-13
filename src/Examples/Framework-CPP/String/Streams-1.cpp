#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Memory/Handle.hpp>
#include <Magma/Framework/String/Stream.hpp>
#include <Magma/Framework/String/StringStream.hpp>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	mfmF32 x, y;

	String::OutStream << u8"Enter x and y: ";
	String::InStream >> x >> y;
	String::OutStream << u8"\n";

	String::OutStream << u8"x + y = " << x << " + " << y << " = " << (x + y) << ";\n";

	String::InStream.ReadUntil(nullptr, 0, u8"\n");

	Magma::Framework::Terminate();
	return 0;
}
