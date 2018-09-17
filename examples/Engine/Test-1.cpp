#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/String/StringStream.hpp>
#include <Magma/Core/Entry.hpp>

using namespace Magma;

// Magma Engine entry point
int run(int argc, char** argv)
{
	auto engine = Core::Engine(argv, Framework::Memory::StandardAllocator);

	return 0;
}

// Magma Framework entry point
int main(int argc, char** argv)
{
	Framework::Init(argc, argv);
	int ret = run(argc, argv);
	Framework::Terminate();
	return ret;
}
