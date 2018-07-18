#include "STDFileSystem.hpp"

#include <sstream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <cstring>

void * Magma::Framework::Files::STDFileSystem::OpenFile(FileMode mode, const Path & path)
{
	FILE* file = nullptr;
	errno_t err;
	if (mode == FileMode::Read)
		err = fopen_s(&file, (this->GetRoot() + path.ToString()).c_str(), "rb");
	else if (mode == FileMode::Write)
		err = fopen_s(&file, (this->GetRoot() + path.ToString()).c_str(), "wb");
	else if (mode == FileMode::ReadWrite)
		err = fopen_s(&file, (this->GetRoot() + path.ToString()).c_str(), "rwb");
	else if (mode == FileMode::Invalid)
	{
		std::stringstream ss;
		ss << "Failed to open file \"" << path.ToString() << "\" on STDFileSystem:" << std::endl;
		ss << "Invalid file mode";
		throw FileError(ss.str());
	}
	else
	{
		std::stringstream ss;
		ss << "Failed to open file \"" << path.ToString() << "\" on STDFileSystem:" << std::endl;
		ss << "Unsupported file mode";
		throw FileError(ss.str());
	}

	if (err != 0 || file == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to open file \"" << path.ToString() << "\" on STDFileSystem:" << std::endl;
		ss << "STD error " << err << ":" << std::endl;
		ss << strerror(err);
		throw FileError(ss.str());
	}

	return file;
}

void Magma::Framework::Files::STDFileSystem::CloseFile(void * file)
{
	if (file == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to close file on STDFileSystem:" << std::endl;
		ss << "File handle is null";
		throw FileError(ss.str());
	}

	if (fclose((FILE*)file) == EOF)
	{
		std::stringstream ss;
		ss << "Failed to close file on STDFileSystem:" << std::endl;
		ss << "fclose() returned EOF";
		throw FileError(ss.str());
	}
}

void Magma::Framework::Files::STDFileSystem::Read(void * file, void * buffer, size_t size)
{
	if (file == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to read from file on STDFileSystem:" << std::endl;
		ss << "File handle is null";
		throw FileError(ss.str());
	}

	if (size == 0)
		return;

	auto readSize = fread(buffer, 1, size, (FILE*)file);
	if (readSize == 0)
	{
		std::stringstream ss;
		ss << "Failed to read from file on STDFileSystem:" << std::endl;
		ss << "No data could be read from file";
		throw FileError(ss.str());
	}
	else if (size != readSize)
	{
		if (feof((FILE*)file))
		{
			std::stringstream ss;
			ss << "Failed to read from file on STDFileSystem:" << std::endl;
			ss << "Couldn't read the request amount of data:" << std::endl;
			ss << "End of file reached";
			throw FileError(ss.str());
		}
		else
		{
			auto err = ferror((FILE*)file);
			std::stringstream ss;
			ss << "Failed to read from file on STDFileSystem:" << std::endl;
			ss << "Couldn't read the request amount of data:" << std::endl;
			ss << "Error " << err << ":" << std::endl;
			ss << strerror(err);
			throw FileError(ss.str());
		}
	}
}

void Magma::Framework::Files::STDFileSystem::Write(void * file, void * buffer, size_t size)
{
	if (size == 0)
		return;

	if (file == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to write to file on STDFileSystem:" << std::endl;
		ss << "File handle is null";
		throw FileError(ss.str());
	}

	auto writtenSize = fwrite(buffer, 1, size, (FILE*)file);
	if (writtenSize == 0)
	{
		std::stringstream ss;
		ss << "Failed to write to file on STDFileSystem:" << std::endl;
		ss << "No data could be read from file";
		throw FileError(ss.str());
	}
	else if (size != writtenSize)
	{
		auto err = ferror((FILE*)file);
		std::stringstream ss;
		ss << "Failed to write to file on STDFileSystem:" << std::endl;
		ss << "Couldn't write the request amount of data:" << std::endl;
		ss << "Error " << err << ":" << std::endl;
		ss << strerror(err);
		throw FileError(ss.str());
	}
}

void Magma::Framework::Files::STDFileSystem::Create(const Path & path)
{
	if (path.IsDirectory())
	{
		if (!std::experimental::filesystem::create_directories(this->GetRoot() + path.ToString()))
		{
			std::stringstream ss;
			ss << "Failed to create file/directory on STDFileSystem:" << std::endl;
			ss << "std::experimental::filesystem::create_directories returned false";
			throw FileError(ss.str());
		}
	}
	else
	{
		if (!std::experimental::filesystem::create_directories(this->GetRoot() + path.GetDirectory().ToString()))
		{
			std::stringstream ss;
			ss << "Failed to create file/directory on STDFileSystem:" << std::endl;
			ss << "std::experimental::filesystem::create_directories returned false";
			throw FileError(ss.str());
		}

		std::ofstream ofs(this->GetRoot() + path.ToString());
		if (!ofs.is_open())
		{
			std::stringstream ss;
			ss << "Failed to create file/directory on STDFileSystem:" << std::endl;
			ss << "std::ofstream didn't open";
			throw FileError(ss.str());
		}
		ofs.close();
	}
}

void Magma::Framework::Files::STDFileSystem::Delete(const Path & path)
{
	if (path.IsRoot())
	{
		std::stringstream ss;
		ss << "Failed to delete file/directory on STDFileSystem:" << std::endl;
		ss << "Can't delete file system root";
		throw FileError(ss.str());
	}

	if (std::experimental::filesystem::remove_all(this->GetRoot() + path.ToString()) == 0)
	{
		std::stringstream ss;
		ss << "Failed to delete file/directory on STDFileSystem:" << std::endl;
		ss << "std::experimental::filesystem::remove returned false";
		throw FileError(ss.str());
	}
}

void Magma::Framework::Files::STDFileSystem::GetDirectoryContents(const Path & path, std::vector<Path>& out)
{
	if (!path.IsDirectory())
	{
		std::stringstream ss;
		ss << "Failed to get directory contents on STDFileSystem:" << std::endl;
		ss << "Path doesn't point to a directory";
		throw FileError(ss.str());
	}

	auto s = this->GetRoot().size();
	for (auto & p : std::experimental::filesystem::directory_iterator(this->GetRoot() + path.ToString()))
	{
		auto p2 = p.path().u8string();
		p2 = p2.substr(s);
		for (auto& c : p2)
			if (c == '\\')
				c = '/';
		if (std::experimental::filesystem::is_directory(p))
			out.emplace_back(p2 + "/");
		else
			out.emplace_back(p2);
	}
}

bool Magma::Framework::Files::STDFileSystem::Exists(const Path & path)
{
	return std::experimental::filesystem::exists(this->GetRoot() + path.ToString());;
}

size_t Magma::Framework::Files::STDFileSystem::GetSize(void * file)
{
	errno = 0;

	if (file == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to get file size on STDFileSystem:" << std::endl;
		ss << "File handle is null";
		throw FileError(ss.str());
	}

	int err = 0;
	auto pos = ftell((FILE*)file);
	if (pos < 0)
	{
		std::stringstream ss;
		ss << "Failed to get file size on STDFileSystem:" << std::endl;
		ss << "ftell returned less than 0, error " << errno << ":" << std::endl;
		ss << strerror(errno);
		errno = 0;
		throw FileError(ss.str());
	}

	err = fseek((FILE*)file, 0, SEEK_END);
	if (err != 0)
	{
		std::stringstream ss;
		ss << "Failed to get file size on STDFileSystem:" << std::endl;
		ss << "fseek returned non zero, error " << ferror((FILE*)file);
		throw FileError(ss.str());
	}
	auto size = ftell((FILE*)file);
	err = fseek((FILE*)file, pos, SEEK_SET);
	if (err != 0)
	{
		std::stringstream ss;
		ss << "Failed to get file size on STDFileSystem:" << std::endl;
		ss << "fseek returned non zero, error " << ferror((FILE*)file);
		throw FileError(ss.str());
	}
	return size;
}