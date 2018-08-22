#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Memory/Object.hpp>
#include <Magma/Framework/String/Stream.hpp>

#include <cstdlib>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	String::Stream obj = mfsOutStream;
	obj.PutString(u8"test\ntest2");
	obj.Flush();

	Magma::Framework::Terminate();

	while (true);
	return 0;
}
