#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/File/FolderArchive.hpp>
#include <Magma/Framework/String/StringStream.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	// Init framework
	Framework::Init(argc, argv);

	Framework::File::HArchive userArchive = nullptr;
	Framework::File::HArchive gameArchive = nullptr;

	// Init archives
	{
		// Get working directory
		mfsUTF8CodeUnit workingDir[256] = { '\0' };
		auto ss = Framework::String::CreateStringStream(workingDir, sizeof(workingDir));
		{
			// Default working directory
			ss << mffMagmaRootDirectory << u8"/resources/Engine";

			char** arg = argv;
			while (*arg != nullptr)
			{
				if (*arg[0] == '-')
				{
					if (strcmp(*arg, u8"-dir") == 0)
					{
						++arg;
						if (*arg == nullptr)
						{
							Framework::String::ErrStream << u8"Expected command line parameter '-dir' argument\n";
							return 2;
						}
						ss.Clear();
						ss << *arg;
					}
					else
					{
						Framework::String::ErrStream << u8"Invalid/unsupported command line parameter '" << *arg << u8"'\n";
						return 1;
					}
				}
				++arg;
			}
		}

		// Get /user/ archive
		{
			mfsUTF8CodeUnit userPath[256] = { '\0' };
			ss.SetBuffer(userPath, sizeof(userPath));
			ss << workingDir << u8"/user";
			userArchive = Framework::File::CreateFolderArchive(userPath);
		}

		// Get /game/ archive
		{
			mfsUTF8CodeUnit gamePath[256] = { '\0' };
			ss.SetBuffer(gamePath, sizeof(gamePath));
			ss << workingDir << u8"/game";
			gameArchive = Framework::File::CreateFolderArchive(gamePath);
		}
	}

	// Terminate archives
	gameArchive.Release();
	userArchive.Release();

	// Terminate framework
	Framework::Terminate();
}