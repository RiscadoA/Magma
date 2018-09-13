#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Audio/RenderDevice.hpp>
#include <Magma/Framework/Audio/WAVLoader.h>
#include <Magma/Framework/File/FileSystem.hpp>
#include <Magma/Framework/File/FolderArchive.hpp>
#include <Magma/Framework/String/Stream.hpp>
#include <Magma/Framework/String/StringStream.hpp>

#include <errno.h>

using namespace Magma::Framework;

int run()
{
	auto rd = Audio::CreateRenderDevice(NULL, Memory::StandardAllocator);

	Audio::HBuffer buf = NULL;
	Audio::HSource src = rd.CreateSource();

	// Create audio buffer
	{
		mfaWAVData data;

		// Load PCM from WAV file
		{
			auto file = File::GetFile(u8"/resources/Audio/test-music-0.wav");
			auto stream = File::OpenFile(file, File::FileMode::Read);

			mfError err = mfaLoadWAV(&stream.Get(), &data, NULL);
			if (err != MF_ERROR_OKAY)
				abort();
		}

		buf = rd.CreateBuffer(data.dataChunk.pcmData, data.dataChunk.header.size, (Audio::Format)data.formatChunk.format, data.formatChunk.sampleRate);
	}

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
