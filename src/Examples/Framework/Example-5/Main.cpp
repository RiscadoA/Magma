#include <iostream>

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/Graphics/1.X/BytecodeAssembler.hpp>
#include <Magma/Framework/Graphics/1.X/MetaDataAssembler.hpp>
#include <Magma/Framework/Graphics/1.X/D3D11Assembler.hpp>
#include <Magma/Framework/Memory/Endianness.hpp>
#include <Magma/Framework/Entry.h>

using namespace Magma::Framework;

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	Files::FileSystem* fileSystem = new Files::STDFileSystem("../../../../../../resources/");

	{
		char bytecode[2048];
		unsigned long bytecodeSize;
		char metaData[2048];
		unsigned long metaDataSize;

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-4-5/vertex.mslbc");
			auto size = fileSystem->GetSize(file);
			auto code = new char[size + 1];
			fileSystem->Read(file, code, size);
			fileSystem->CloseFile(file);
			code[size] = '\0';
			bytecodeSize = Graphics::V1X::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
		}

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-4-5/vertex.mslmd");
			auto size = fileSystem->GetSize(file);
			auto code = new char[size + 1];
			fileSystem->Read(file, code, size);
			fileSystem->CloseFile(file);
			code[size] = '\0';
			metaDataSize = Graphics::V1X::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
		}

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-4-5/vertex.mslbo");
			mfmU32 bytecodeSizeBE;
			Memory::ToBigEndian4(&bytecodeSize, &bytecodeSizeBE);
			fileSystem->Write(file, &bytecodeSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, bytecode, bytecodeSize);

			mfmU32 metaDataSizeBE;
			Memory::ToBigEndian4(&metaDataSize, &metaDataSizeBE);
			fileSystem->Write(file, &metaDataSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, metaData, metaDataSize);
			fileSystem->CloseFile(file);
		}

		char* binaryObject = nullptr;
		unsigned long binaryObjectSize = 0;

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-4-5/vertex.mslbo");
			binaryObjectSize = fileSystem->GetSize(file);
			binaryObject = new char[binaryObjectSize];
			fileSystem->Read(file, binaryObject, binaryObjectSize);
			fileSystem->CloseFile(file);
		}

		Graphics::V1X::ShaderData data(binaryObject, binaryObjectSize);

		std::string compiled;
		Graphics::V1X::D3D11Assembler::Assemble(data, compiled);

		std::cout << compiled << std::endl;
	}

	{
		char bytecode[2048];
		unsigned long bytecodeSize;
		char metaData[2048];
		unsigned long metaDataSize;

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-4-5/pixel.mslbc");
			auto size = fileSystem->GetSize(file);
			auto code = new char[size + 1];
			fileSystem->Read(file, code, size);
			fileSystem->CloseFile(file);
			code[size] = '\0';
			bytecodeSize = Graphics::V1X::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
		}

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-4-5/pixel.mslmd");
			auto size = fileSystem->GetSize(file);
			auto code = new char[size + 1];
			fileSystem->Read(file, code, size);
			fileSystem->CloseFile(file);
			code[size] = '\0';
			metaDataSize = Graphics::V1X::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
		}

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-4-5/pixel.mslbo");
			mfmU32 bytecodeSizeBE;
			Memory::ToBigEndian4(&bytecodeSize, &bytecodeSizeBE);
			fileSystem->Write(file, &bytecodeSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, bytecode, bytecodeSize);

			mfmU32 metaDataSizeBE;
			Memory::ToBigEndian4(&metaDataSize, &metaDataSizeBE);
			fileSystem->Write(file, &metaDataSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, metaData, metaDataSize);
			fileSystem->CloseFile(file);
		}

		char* binaryObject = nullptr;
		unsigned long binaryObjectSize = 0;

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-4-5/pixel.mslbo");
			binaryObjectSize = fileSystem->GetSize(file);
			binaryObject = new char[binaryObjectSize];
			fileSystem->Read(file, binaryObject, binaryObjectSize);
			fileSystem->CloseFile(file);
		}

		Graphics::V1X::ShaderData data(binaryObject, binaryObjectSize);

		std::string compiled;
		Graphics::V1X::D3D11Assembler::Assemble(data, compiled);

		std::cout << compiled << std::endl;
	}

	getchar();

	delete fileSystem;

	mfTerminate();
	return 0;
}