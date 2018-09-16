#include <Magma/Core/Entry.hpp>
#include <Magma/Core/Parameters.hpp>
#include <Magma/Framework/String/StringStream.hpp>
#include <Magma/Framework/File/FolderArchive.hpp>

Magma::Core::Engine::Engine(mfsUTF8CodeUnit** argv, Framework::Memory::HAllocator allocator)
	: m_allocator(allocator), m_gameArchive(nullptr), m_userArchive(nullptr)
{
	// Set default root directory
	{
		auto ss = Framework::String::StringStream(m_rootDirectory, sizeof(m_rootDirectory));
		ss.Get() << mffMagmaRootDirectory << "/resources";
	}
	
	// Parse paremeters
	{
		Parameters p;
		p.AddParser(this, u8"-dir", Framework::Memory::GetMethod(*this, &ParseDirectoryParam));
		p.Parse(argv);
	}

	// Create and register game archive
	{
		mfsUTF8CodeUnit path[512];
		auto ss = Framework::String::StringStream(path, sizeof(path));
		ss.Get() << m_rootDirectory;
		ss.Get() << u8"/game/";
		m_gameArchive = Framework::File::CreateFolderArchive(path, allocator);
		Framework::File::RegisterArchive(m_gameArchive, u8"game");
	}

	// Create and register user archive
	{
		mfsUTF8CodeUnit path[512];
		auto ss = Framework::String::StringStream(path, sizeof(path));
		ss.Get() << m_rootDirectory;
		ss.Get() << u8"/user/";
		m_userArchive = Framework::File::CreateFolderArchive(path, allocator);
		Framework::File::RegisterArchive(m_userArchive, u8"user");
	}
}

Magma::Core::Engine::~Engine()
{
	// Unregister user archive and release it
	Framework::File::UnregisterArchive(m_userArchive);
	m_userArchive.Release();
	
	// Unregister game archive and release it
	Framework::File::UnregisterArchive(m_gameArchive);
	m_gameArchive.Release();
}

mfsUTF8CodeUnit ** Magma::Core::Engine::ParseDirectoryParam(void *, mfsUTF8CodeUnit ** arg)
{
	auto ss = Framework::String::StringStream(m_rootDirectory, sizeof(m_rootDirectory));
	ss.Get() << *arg;
	++arg;
	return arg;
}
