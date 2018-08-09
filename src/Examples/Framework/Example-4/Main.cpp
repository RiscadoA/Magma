#include <iostream>

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/Graphics/BytecodeAssembler.hpp>
#include <Magma/Framework/Graphics/MetaDataAssembler.hpp>
#include <Magma/Framework/Graphics/OGL410Assembler.hpp>
#include <Magma/Framework/String/Conversion.hpp>

using namespace Magma::Framework;

void Main(int argc, char** argv)
{
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
			bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
		}

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-4-5/vertex.mslmd");
			auto size = fileSystem->GetSize(file);
			auto code = new char[size + 1];
			fileSystem->Read(file, code, size);
			fileSystem->CloseFile(file);
			code[size] = '\0';
			metaDataSize = Graphics::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
		}

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-4-5/vertex.mslbo");
			auto bytecodeSizeBE = String::U32ToBE(bytecodeSize);
			fileSystem->Write(file, &bytecodeSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, bytecode, bytecodeSize);

			auto metaDataSizeBE = String::U32ToBE(metaDataSize);
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

		Graphics::ShaderData data(binaryObject, binaryObjectSize);

		std::string compiled;
		Graphics::OGL410Assembler::Assemble(data, compiled);

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
			bytecodeSize = Graphics::BytecodeAssembler::Assemble(code, bytecode, sizeof(bytecode));
		}

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-4-5/pixel.mslmd");
			auto size = fileSystem->GetSize(file);
			auto code = new char[size + 1];
			fileSystem->Read(file, code, size);
			fileSystem->CloseFile(file);
			code[size] = '\0';
			metaDataSize = Graphics::MetaDataAssembler::Assemble(code, metaData, sizeof(metaData));
		}

		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-4-5/pixel.mslbo");
			auto bytecodeSizeBE = String::U32ToBE(bytecodeSize);
			fileSystem->Write(file, &bytecodeSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, bytecode, bytecodeSize);

			auto metaDataSizeBE = String::U32ToBE(metaDataSize);
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

		Graphics::ShaderData data(binaryObject, binaryObjectSize);

		std::string compiled;
		Graphics::OGL410Assembler::Assemble(data, compiled);

		std::cout << compiled << std::endl;
	}

	getchar();

	delete fileSystem;
}