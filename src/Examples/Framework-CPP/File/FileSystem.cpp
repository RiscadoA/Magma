#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/String/StringStream.hpp>
#include <Magma/Framework/File/FolderArchive.hpp>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	File::Archive archive = NULL;

	// Create archive and register it
	{
		mfsUTF8CodeUnit path[256];
		String::Stream ss = String::CreateStringStream(path, sizeof(path));
		ss.PutString(mffMagmaRootDirectory);
		ss.PutString(u8"/resources/FileSystem-Example");
		archive = File::CreateFolderArchive(path);
		File::RegisterArchive(archive, u8"temp");
	}

	// Create directory
	File::File dir = File::CreateDirectory(u8"/temp/dir");

	File::File file = File::CreateFile(u8"/temp/dir/test.txt");
	
	String::Stream stream = File::OpenFile(file, File::FileMode::Write);
	stream.PutString(u8"test\n");
	stream.Release();
	dir.Release();
	file.Release();

	//File::DeleteDirectory(dir);

	// Unregister archive
	File::UnregisterArchive(archive);

	Magma::Framework::Terminate();
	return 0;
}
