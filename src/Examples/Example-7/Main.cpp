#include <iostream>

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/Graphics/BytecodeAssembler.hpp>
#include <Magma/Framework/Graphics/MetaDataAssembler.hpp>
#include <Magma/Framework/Graphics/D3D11Assembler.hpp>
#include <Magma/Framework/Graphics/OGL410Assembler.hpp>
#include <Magma/Framework/Graphics/ShaderCompiler.hpp>
#include <Magma/Framework/String/Conversion.hpp>

using namespace Magma::Framework;

void Main(int argc, char** argv)
{
	Files::FileSystem* fileSystem = new Files::STDFileSystem("../../../../../resources/");

	// Vertex shader
	{
		char bytecode[2048];
		unsigned long bytecodeSize;
		char metaData[2048];
		unsigned long metaDataSize;

		std::string bytecodeSrc;
		std::string metaDataSrc;

		// Load MSL shader and compile it
		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-7/vertex.msl");
			auto size = fileSystem->GetSize(file);
			auto code = new char[size + 1];
			fileSystem->Read(file, code, size);
			fileSystem->CloseFile(file);
			code[size] = '\0';
			Graphics::ShaderCompiler::Run(code, bytecodeSrc, metaDataSrc);
		}

		// Write bytecode
		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/vertex.mslbc");
			fileSystem->Write(file, bytecodeSrc.data(), bytecodeSrc.size());
			fileSystem->CloseFile(file);
		}

		// Write meta data
		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/vertex.mslmd");
			fileSystem->Write(file, metaDataSrc.data(), metaDataSrc.size());
			fileSystem->CloseFile(file);
		}

		// Assemble shader 
		bytecodeSize = Graphics::BytecodeAssembler::Assemble(bytecodeSrc, bytecode, sizeof(bytecode));
		metaDataSize = Graphics::MetaDataAssembler::Assemble(metaDataSrc, metaData, sizeof(metaData));

		// Write binary MSL shader object
		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/vertex.mslbo");
			auto bytecodeSizeBE = String::U32ToBE(bytecodeSize);
			fileSystem->Write(file, &bytecodeSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, bytecode, bytecodeSize);

			auto metaDataSizeBE = String::U32ToBE(metaDataSize);
			fileSystem->Write(file, &metaDataSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, metaData, metaDataSize);
			fileSystem->CloseFile(file);
		}

		Graphics::ShaderData data(bytecode, bytecodeSize, metaData, metaDataSize);

		// Write HLSL shader
		{
			std::string compiled;
			Graphics::D3D11Assembler::Assemble(data, compiled);

			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/vertex.hlsl");
			fileSystem->Write(file, compiled.data(), compiled.size());
			fileSystem->CloseFile(file);
		}

		// Write GLSL shader
		{
			std::string compiled;
			Graphics::OGL410Assembler::Assemble(data, compiled);

			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/vertex.glsl");
			fileSystem->Write(file, compiled.data(), compiled.size());
			fileSystem->CloseFile(file);
		}
	}

	// Pixel shader
	{
		char bytecode[2048];
		unsigned long bytecodeSize;
		char metaData[2048];
		unsigned long metaDataSize;

		std::string bytecodeSrc;
		std::string metaDataSrc;

		// Load MSL shader and compile it
		{
			auto file = fileSystem->OpenFile(Files::FileMode::Read, "/Example-7/pixel.msl");
			auto size = fileSystem->GetSize(file);
			auto code = new char[size + 1];
			fileSystem->Read(file, code, size);
			fileSystem->CloseFile(file);
			code[size] = '\0';
			Graphics::ShaderCompiler::Run(code, bytecodeSrc, metaDataSrc);
		}

		// Write bytecode
		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/pixel.mslbc");
			fileSystem->Write(file, bytecodeSrc.data(), bytecodeSrc.size());
			fileSystem->CloseFile(file);
		}

		// Write meta data
		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/pixel.mslmd");
			fileSystem->Write(file, metaDataSrc.data(), metaDataSrc.size());
			fileSystem->CloseFile(file);
		}

		// Assemble shader 
		bytecodeSize = Graphics::BytecodeAssembler::Assemble(bytecodeSrc, bytecode, sizeof(bytecode));
		metaDataSize = Graphics::MetaDataAssembler::Assemble(metaDataSrc, metaData, sizeof(metaData));

		// Write binary MSL shader object
		{
			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/pixel.mslbo");
			auto bytecodeSizeBE = String::U32ToBE(bytecodeSize);
			fileSystem->Write(file, &bytecodeSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, bytecode, bytecodeSize);

			auto metaDataSizeBE = String::U32ToBE(metaDataSize);
			fileSystem->Write(file, &metaDataSizeBE, sizeof(unsigned long));
			fileSystem->Write(file, metaData, metaDataSize);
			fileSystem->CloseFile(file);
		}

		Graphics::ShaderData data(bytecode, bytecodeSize, metaData, metaDataSize);

		// Write HLSL shader
		{
			std::string compiled;
			Graphics::D3D11Assembler::Assemble(data, compiled);

			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/pixel.hlsl");
			fileSystem->Write(file, compiled.data(), compiled.size());
			fileSystem->CloseFile(file);
		}

		// Write GLSL shader
		{
			std::string compiled;
			Graphics::OGL410Assembler::Assemble(data, compiled);

			auto file = fileSystem->OpenFile(Files::FileMode::Write, "/Example-7/pixel.glsl");
			fileSystem->Write(file, compiled.data(), compiled.size());
			fileSystem->CloseFile(file);
		}
	}

	delete fileSystem;
}