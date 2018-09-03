#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/String/StringStream.hpp>
#include <Magma/Framework/File/FolderArchive.hpp>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	FileHandle::ArchiveHandle archive = NULL;

	// Create archive and register it
	{
		mfsUTF8CodeUnit path[256];
		String::StreamHandle ss = String::CreateStringStream(path, sizeof(path));
		ss.PutString(mffMagmaRootDirectory);
		ss.PutString(u8"/resources/FileSystem-Example");
		archive = FileHandle::CreateFolderArchive(path);
		FileHandle::RegisterArchive(archive, u8"temp");
	}

	// Create directory
	FileHandle::FileHandle dir = FileHandle::CreateDirectory(u8"/temp/dir");

	FileHandle::FileHandle file = FileHandle::CreateFile(u8"/temp/dir/test.txt");
	
	String::StreamHandle stream = FileHandle::OpenFile(file, FileHandle::FileMode::Write);
	stream.PutString(u8"test\n");
	stream.Release();
	dir.Release();
	file.Release();

	//FileHandle::DeleteDirectory(dir);

	// Unregister archive
	FileHandle::UnregisterArchive(archive);

	Magma::Framework::Terminate();
	return 0;
}
