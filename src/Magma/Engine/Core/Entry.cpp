#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/File/FolderArchive.hpp>
#include <Magma/Framework/String/StringStream.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	// Init framework
	Framework::Init(argc, argv);

	// Setup archive
	mfsUTF8CodeUnit archivePath[256];
	auto ss = Framework::String::create
	mffMagmaRootDirectory;
	auto archive = Framework::File::CreateFolderArchive(archivePath);

	// Terminate framework
	Framework::Terminate();
}