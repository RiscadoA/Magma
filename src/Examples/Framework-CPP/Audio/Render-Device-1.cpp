#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Audio/RenderDevice.hpp>
#include <Magma/Framework/File/FileSystem.hpp>
#include <Magma/Framework/File/FolderArchive.hpp>
#include <Magma/Framework/String/Stream.hpp>
#include <Magma/Framework/String/StringStream.hpp>

using namespace Magma::Framework;

int run()
{
	auto rd = Audio::CreateRenderDevice(NULL, Memory::StandardAllocator);

	mfmU16 bufData[65536];
	for (mfmU64 i = 0; i < 65536; ++i)
		bufData[i] = i;
	auto buf = rd.CreateBuffer(bufData, sizeof(bufData), Audio::Format::Mono16, 60000);
	auto src = rd.CreateSource();

	src.Bind(buf);
	src.SetLooping(true);
	src.Play();

	while (true);
}

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	// Register archive on file system
	{
		mfsUTF8CodeUnit archivePath[256];
		auto ss = String::CreateStringStream(archivePath, sizeof(archivePath));
		ss.PutString(mffMagmaRootDirectory);
		ss.PutString(u8"/resources");
		ss.Release();

		auto archive = File::CreateFolderArchive(archivePath);
		File::RegisterArchive(archive, u8"resources");
	}

	int ret = run();

	Magma::Framework::Terminate();
	return ret;
}
