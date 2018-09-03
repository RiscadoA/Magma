#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Memory/Handle.hpp>
#include <Magma/Framework/Memory/StackAllocator.hpp>
#include <Magma/Framework/String/Stream.hpp>
#include <Magma/Framework/String/StringStream.hpp>

#include <cstdlib>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	mfsUTF8CodeUnit buffer[256];
	memset(buffer, 0, sizeof(buffer));

	Memory::StackAllocatorHandle alloc = Memory::CreateStackAllocator(2048);
	Memory::Handle obj = String::CreateStringStream(buffer, sizeof(buffer), alloc);

	String::StringStream ss = obj;
	ss.PutString(u8"Sample text");
	ss.Release();

	alloc.Release();
	obj.Release();

	String::OutStream.PutString(buffer);

	mfmU8 byte;
	String::InStream.GetByte(byte);

	Magma::Framework::Terminate();
	return 0;
}
