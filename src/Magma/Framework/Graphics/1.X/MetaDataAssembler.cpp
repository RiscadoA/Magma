#include "MetaDataAssembler.hpp"
#include "ShaderData.hpp"

#include "../../Memory/Endianness.hpp"

#include <sstream>
#include <regex>
#include <stack>

std::string ToLower(const std::string& str)
{
	std::string ret = str;
	for (auto& c : ret)
		c = tolower(c);
	return ret;
}

size_t Magma::Framework::Graphics_V1X::MetaDataAssembler::Assemble(const std::string & code, char * out, size_t maxSize)
{
	struct Object
	{
		enum
		{
			Type_Input_Var,
			Type_Output_Var,
			Type_Texture_2D,
			Type_Constant_Buffer,
			Type_Constant_Buffer_Var,
		} objType;

		unsigned long bufferIndex;
		unsigned long bufferOffset;
		unsigned long index;
		std::string name;
		ShaderDataVariableType varType;
	};

	unsigned long majorVersion = 0;
	unsigned long minorVersion = 0;
	auto shaderType = ShaderType::Invalid;

	std::vector<Object> objects;

	// Parse file
	{
		static const std::regex majorRegex(R"regex(^\s*major\s+"(.*)"\s*$)regex", std::regex_constants::icase);
		static const std::regex minorRegex(R"regex(^\s*minor\s+"(.*)"\s*$)regex", std::regex_constants::icase);
		static const std::regex shaderTypeRegex(R"regex(^\s*shader type\s+"(.*)"\s*$)regex", std::regex_constants::icase);
		
		static const std::regex inputVarRegex(R"regex(^\s*input var\s*$)regex", std::regex_constants::icase);
		static const std::regex outputVarRegex(R"regex(^\s*output var\s*$)regex", std::regex_constants::icase);
		static const std::regex texture2DRegex(R"regex(^\s*texture 2d\s*$)regex", std::regex_constants::icase);
		static const std::regex constantBufferRegex(R"regex(^\s*constant buffer\s*$)regex", std::regex_constants::icase);
		static const std::regex constantBufferVarRegex(R"regex(^\s*constant buffer var\s*$)regex", std::regex_constants::icase);

		static const std::regex typeRegex(R"regex(^\s*type\s+"(.*)"\s*$)regex", std::regex_constants::icase);
		static const std::regex indexRegex(R"regex(^\s*index\s+"(.*)"\s*$)regex", std::regex_constants::icase);
		static const std::regex nameRegex(R"regex(^\s*name\s+"(.*)"\s*$)regex", std::regex_constants::icase);
		static const std::regex bufferIndexRegex(R"regex(^\s*buffer index\s+"(.*)"\s*$)regex", std::regex_constants::icase);
		static const std::regex bufferOffsetRegex(R"regex(^\s*buffer offset\s+"(.*)"\s*$)regex", std::regex_constants::icase);

		std::stringstream ss(code);
		std::string line;
		while (std::getline(ss, line))
		{
			if (line.empty() || line[0] == '#')
				continue;

			std::smatch match;
			if (std::regex_match(line, match, majorRegex))
				majorVersion = std::stoul(match.str(1));
			else if (std::regex_match(line, match, minorRegex))
				minorVersion = std::stoul(match.str(1));
			else if (std::regex_match(line, match, shaderTypeRegex))
			{
				if (ToLower(match.str(1)) == "vertex")
					shaderType = ShaderType::Vertex;
				else if (ToLower(match.str(1)) == "pixel")
					shaderType = ShaderType::Pixel;
				else
					throw ShaderError("Failed to assemble binary shader meta data:\nUnknown shader type");
			}
			else if (std::regex_match(line, match, inputVarRegex))
			{
				objects.push_back(Object());
				objects.back().objType = Object::Type_Input_Var;
			}
			else if (std::regex_match(line, match, outputVarRegex))
			{
				objects.push_back(Object());
				objects.back().objType = Object::Type_Output_Var;
			}
			else if (std::regex_match(line, match, texture2DRegex))
			{
				objects.push_back(Object());
				objects.back().objType = Object::Type_Texture_2D;
			}
			else if (std::regex_match(line, match, constantBufferRegex))
			{
				objects.push_back(Object());
				objects.back().objType = Object::Type_Constant_Buffer;
			}
			else if (std::regex_match(line, match, constantBufferVarRegex))
			{
				objects.push_back(Object());
				objects.back().objType = Object::Type_Constant_Buffer_Var;
			}
			else if (std::regex_match(line, match, indexRegex))
				objects.back().index = std::stoul(match.str(1));
			else if (std::regex_match(line, match, nameRegex))
				objects.back().name = match.str(1);
			else if (std::regex_match(line, match, bufferIndexRegex))
				objects.back().bufferIndex = std::stoul(match.str(1));
			else if (std::regex_match(line, match, bufferOffsetRegex))
				objects.back().bufferOffset = std::stoul(match.str(1));
			else if (std::regex_match(line, match, typeRegex))
			{
				auto str = ToLower(match.str(1));
				if (str == "int" || str == "int1")
					objects.back().varType = ShaderDataVariableType::Int1;
				else if (str == "int2")
					objects.back().varType = ShaderDataVariableType::Int2;
				else if (str == "int3")
					objects.back().varType = ShaderDataVariableType::Int3;
				else if (str == "int4")
					objects.back().varType = ShaderDataVariableType::Int4;
				else if (str == "int22")
					objects.back().varType = ShaderDataVariableType::Int22;
				else if (str == "int33")
					objects.back().varType = ShaderDataVariableType::Int33;
				else if (str == "int44")
					objects.back().varType = ShaderDataVariableType::Int44;
				else if (str == "float" || str == "float1")
					objects.back().varType = ShaderDataVariableType::Float1;
				else if (str == "float2")
					objects.back().varType = ShaderDataVariableType::Float2;
				else if (str == "float3")
					objects.back().varType = ShaderDataVariableType::Float3;
				else if (str == "float4")
					objects.back().varType = ShaderDataVariableType::Float4;
				else if (str == "float22")
					objects.back().varType = ShaderDataVariableType::Float22;
				else if (str == "float33")
					objects.back().varType = ShaderDataVariableType::Float33;
				else if (str == "float44")
					objects.back().varType = ShaderDataVariableType::Float44;
				else
					throw ShaderError("Failed to assemble binary shader meta data:\nUnknown variable type");
			}
		}
	}

	// Build binary file
	size_t size = 0;

	if (size + 12 > maxSize)
		throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space for meta data header");

	// Write major version
	Memory::ToBigEndian4(&majorVersion, &majorVersion);
	memcpy(out + size, &majorVersion, sizeof(unsigned long));
	size += 4;

	// Write minor version
	Memory::ToBigEndian4(&minorVersion, &minorVersion);
	memcpy(out + size, &minorVersion, sizeof(unsigned long));
	size += 4;

	// Write shader type
	Memory::ToBigEndian4(&shaderType, &shaderType);
	memcpy(out + size, &shaderType, sizeof(unsigned long));
	size += 4;

	// Write input var count
	unsigned long inputVarCount = 0;
	for (auto& o : objects)
		if (o.objType == Object::Type_Input_Var)
			inputVarCount++;
	Memory::ToBigEndian4(&inputVarCount, &inputVarCount);
	if (size + 4 > maxSize)
		throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
	memcpy(out + size, &inputVarCount, sizeof(unsigned long));
	size += 4;

	// Write input vars
	for (auto& o : objects)
		if (o.objType == Object::Type_Input_Var)
		{
			// Write index
			Memory::ToBigEndian4(&o.index, &o.index);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.index, sizeof(unsigned long));
			size += 4;

			// Write name size
			unsigned long nameSize = o.name.size();
			Memory::ToBigEndian4(&nameSize, &nameSize);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &nameSize, sizeof(unsigned long));
			size += 4;

			// Write name
			if (size + o.name.size() > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, o.name.data(), o.name.size());
			size += o.name.size();

			// Write type
			Memory::ToBigEndian4(&o.varType, &o.varType);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.varType, sizeof(unsigned long));
			size += 4;
		}

	// Write output var count
	unsigned long outputVarCount = 0;
	for (auto& o : objects)
		if (o.objType == Object::Type_Output_Var)
			outputVarCount++;
	Memory::ToBigEndian4(&outputVarCount, &outputVarCount);
	if (size + 4 > maxSize)
		throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
	memcpy(out + size, &outputVarCount, sizeof(unsigned long));
	size += 4;

	// Write output vars
	for (auto& o : objects)
		if (o.objType == Object::Type_Output_Var)
		{
			// Write index
			Memory::ToBigEndian4(&o.index, &o.index);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.index, sizeof(unsigned long));
			size += 4;

			// Write name size
			unsigned long nameSize = o.name.size();
			Memory::ToBigEndian4(&nameSize, &nameSize);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &nameSize, sizeof(unsigned long));
			size += 4;

			// Write name
			if (size + o.name.size() > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, o.name.data(), o.name.size());
			size += o.name.size();

			// Write type
			Memory::ToBigEndian4(&o.varType, &o.varType);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.varType, sizeof(unsigned long));
			size += 4;
		}

	// Write 2D texture var count
	unsigned long texture2DVarCount = 0;
	for (auto& o : objects)
		if (o.objType == Object::Type_Texture_2D)
			texture2DVarCount++;
	Memory::ToBigEndian4(&texture2DVarCount, &texture2DVarCount);
	if (size + 4 > maxSize)
		throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
	memcpy(out + size, &texture2DVarCount, sizeof(unsigned long));
	size += 4;

	// Write 2D texture vars
	for (auto& o : objects)
		if (o.objType == Object::Type_Texture_2D)
		{
			// Write index
			Memory::ToBigEndian4(&o.index, &o.index);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.index, sizeof(unsigned long));
			size += 4;

			// Write name size
			unsigned long nameSize = o.name.size();
			Memory::ToBigEndian4(&nameSize, &nameSize);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &nameSize, sizeof(unsigned long));
			size += 4;

			// Write name
			if (size + o.name.size() > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, o.name.data(), o.name.size());
			size += o.name.size();
		}

	// Write contant buffer count
	unsigned long constantBufferCount = 0;
	for (auto& o : objects)
		if (o.objType == Object::Type_Constant_Buffer)
			constantBufferCount++;
	Memory::ToBigEndian4(&constantBufferCount, &constantBufferCount);
	if (size + 4 > maxSize)
		throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
	memcpy(out + size, &constantBufferCount, sizeof(unsigned long));
	size += 4;

	// Write constant buffers
	for (auto& o : objects)
		if (o.objType == Object::Type_Constant_Buffer)
		{
			// Write index
			Memory::ToBigEndian4(&o.index, &o.index);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.index, sizeof(unsigned long));
			size += 4;

			// Write name size
			unsigned long nameSize = o.name.size();
			Memory::ToBigEndian4(&nameSize, &nameSize);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &nameSize, sizeof(unsigned long));
			size += 4;

			// Write name
			if (size + o.name.size() > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, o.name.data(), o.name.size());
			size += o.name.size();
		}

	// Write contant buffer var count
	unsigned long constantBufferVarCount = 0;
	for (auto& o : objects)
		if (o.objType == Object::Type_Constant_Buffer_Var)
			constantBufferVarCount++;
	Memory::ToBigEndian4(&constantBufferVarCount, &constantBufferVarCount);
	if (size + 4 > maxSize)
		throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
	memcpy(out + size, &constantBufferVarCount, sizeof(unsigned long));
	size += 4;

	// Write constant buffer vars
	for (auto& o : objects)
		if (o.objType == Object::Type_Constant_Buffer_Var)
		{
			// Write buffer index
			Memory::ToBigEndian4(&o.bufferIndex, &o.bufferIndex);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.bufferIndex, sizeof(unsigned long));
			size += 4;

			// Write buffer offset
			Memory::ToBigEndian4(&o.bufferOffset, &o.bufferOffset);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.bufferOffset, sizeof(unsigned long));
			size += 4;

			// Write index
			Memory::ToBigEndian4(&o.index, &o.index);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.index, sizeof(unsigned long));
			size += 4;

			// Write name size
			unsigned long nameSize = o.name.size();
			Memory::ToBigEndian4(&nameSize, &nameSize);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &nameSize, sizeof(unsigned long));
			size += 4;

			// Write name
			if (size + o.name.size() > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, o.name.data(), o.name.size());
			size += o.name.size();

			// Write type
			Memory::ToBigEndian4(&o.varType, &o.varType);
			if (size + 4 > maxSize)
				throw ShaderError("Failed to assemble binary shader meta data:\nNot enough space");
			memcpy(out + size, &o.varType, sizeof(unsigned long));
			size += 4;
		}

	return size;
}