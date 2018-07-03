#include "OGL400Assembler.hpp"
#include "BytecodeAssembler.hpp"

#include "../String/Conversion.hpp"

#include <sstream>
#include <stack>
#include <map>

using namespace Magma::Framework::Graphics;
using namespace Magma::Framework;

void ToGL400(ShaderVariableType type, std::stringstream& out)
{
	switch (type)
	{
		case ShaderVariableType::Int1: out << "int"; break;
		case ShaderVariableType::Int2: out << "ivec2"; break;
		case ShaderVariableType::Int3: out << "ivec3"; break;
		case ShaderVariableType::Int4: out << "ivec4"; break;
		case ShaderVariableType::Int22: out << "imat2"; break;
		case ShaderVariableType::Int33: out << "imat3"; break;
		case ShaderVariableType::Int44: out << "imat4"; break;

		case ShaderVariableType::Float1: out << "float"; break;
		case ShaderVariableType::Float2: out << "vec2"; break;
		case ShaderVariableType::Float3: out << "vec3"; break;
		case ShaderVariableType::Float4: out << "vec4"; break;
		case ShaderVariableType::Float22: out << "mat2"; break;
		case ShaderVariableType::Float33: out << "mat3"; break;
		case ShaderVariableType::Float44: out << "mat4"; break;

		case ShaderVariableType::Invalid:
			throw ShaderError("Failed to assemble from binary bytecode on OGL400Assembler:\nInvalid shader variable type");
			break;
		default:
			throw ShaderError("Failed to assemble from binary bytecode on OGL400Assembler:\nUnsupported shader variable type");
			break;
	}
}

void Magma::Framework::Graphics::OGL400Assembler::Assemble(const ShaderData & data, std::string & out)
{
	std::stringstream ss;
	
	ss << "#version 400 core" << std::endl << std::endl;
	ss << "// This shader was automatically generated from binary bytecode by the OGL400Assembler::Assemble function" << std::endl;
	switch (data.GetShaderType())
	{
		case ShaderType::Vertex:
			ss << "// Vertex shader" << std::endl;
			break;
		case ShaderType::Pixel:
			ss << "// Pixel shader" << std::endl;
			break;
		case ShaderType::Invalid:
			throw ShaderError("Failed to assemble from binary bytecode on OGL400Assembler:\nInvalid shader type");
			break;
		default:
			throw ShaderError("Failed to assemble from binary bytecode on OGL400Assembler:\nUnsupported shader type");
			break;
	}
	ss << "// DO NOT MODIFY THIS FILE BY HAND" << std::endl;
	ss << std::endl;

	// Add input variables
	for (auto& in : data.GetInputVariables())
	{
		ss << "layout (location = " << in.index << ") in ";
		ToGL400(in.type, ss);
		ss << " in_" << in.index << "; // Input variable \"" << in.name << "\"; index " << in.index;
		ss << std::endl;
	}
	if (data.GetInputVariables().size() > 0)
		ss << std::endl;

	// Add output variables
	for (auto& out : data.GetOutputVariables())
	{
		ss << "layout (location = " << out.index << ") out ";
		ToGL400(out.type, ss);
		ss << " out_" << out.index << "; // Output variable \"" << out.name << "\"; index " << out.index;
		ss << std::endl;
	}
	if (data.GetOutputVariables().size() > 0)
		ss << std::endl;
	

	/*
		TO DO:
		- Add 2D texture variables
		- Add constant buffer variables
	*/

	auto GetComponent = [&](size_t count, size_t index) -> void
	{
		if (index >= count)
			throw ShaderError("Failed to assemble from binary bytecode on OGL400Assembler:\nFailed to get vector component, out of bounds");
		switch (index)
		{
			case 0: ss << "x"; break;
			case 1: ss << "y"; break;
			case 2: ss << "z"; break;
			case 3: ss << "w"; break;
		}
	};

	auto PutIndex = [&](size_t index) -> void
	{
		for (auto& v : data.GetInputVariables())
			if (v.index == index)
			{
				ss << "in_" << v.index;
				return;
			}

		for (auto& v : data.GetOutputVariables())
			if (v.index == index)
			{
				ss << "out_" << v.index;
				return;
			}

		for (auto& v : data.GetTexture2DVariables())
			if (v.index == index)
			{
				ss << "tex_" << v.index;
				return;
			}

		for (auto& v : data.GetShaderConstantVariables())
			if (v.index == index)
			{
				ss << "buf_" << v.bufferIndex << "_" << v.index;
				return;
			}

		if (index == 0 && data.GetShaderType() == ShaderType::Vertex)
		{
			ss << "gl_Position";
			return;
		}

		ss << "local_" << index;
	};

	auto FloatFromBE = [&](float flt) -> float
	{
		union
		{
			String::U32Char chr;
			float flt;
		} convert;
		convert.flt = flt;
		convert.chr = String::U32FromBE(convert.chr);
		return convert.flt;
	};

	auto GetFloat = [&](const char* it) -> void
	{
		union
		{
			char chr[4];
			float flt;
		} conversion;
		conversion.chr[0] = it[0];
		conversion.chr[1] = it[1];
		conversion.chr[2] = it[2];
		conversion.chr[3] = it[3];
		ss << FloatFromBE(conversion.flt);
	};

	auto GetInt = [&](const char* it) -> void
	{
		union
		{
			char chr[4];
			int integer;
		} conversion;
		conversion.chr[0] = it[0];
		conversion.chr[1] = it[1];
		conversion.chr[2] = it[2];
		conversion.chr[3] = it[3];
		ss << (int)String::U32FromBE(conversion.integer);
	};

	// Add main function (shader bytecode)
	size_t varIn0 = 0;
	size_t varIn1 = 0;
	size_t varOut = 0;

	ss << "void main()" << std::endl;
	ss << "{" << std::endl;
	for (size_t i = 0; i < data.GetBytecodeSize(); ++i)
	{
		auto it = &data.GetBytecode()[i];
		switch ((BytecodeOpCode)it[0])
		{
			// Declarations
			{
			case BytecodeOpCode::DeclI1:
				ss << "\tint local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclI2:
				ss << "\tivec2 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclI3:
				ss << "\tivec3 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclI4:
				ss << "\tivec4 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclI22:
				ss << "\timat2 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclI33:
				ss << "\timat3 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclI44:
				ss << "\timat4 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclF1:
				ss << "\tfloat local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclF2:
				ss << "\tvec2 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclF3:
				ss << "\tvec3 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclF4:
				ss << "\tvec4 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclF22:
				ss << "\tmat2 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclF33:
				ss << "\tmat3 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::DeclF44:
				ss << "\tmat4 local_" << (unsigned long)it[1] << ";" << std::endl;
				i += 1;
				break;
			}

			// Registers
			{
			case BytecodeOpCode::VarIn0:
				varIn0 = it[1];
				i += 1;
				break;
			case BytecodeOpCode::VarIn1:
				varIn1 = it[1];
				i += 1;
				break;
			case BytecodeOpCode::VarOut:
				varOut = it[1];
				i += 1;
				break;
			}

			// Operations
			{
			case BytecodeOpCode::Assign:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << ";" << std::endl;
				break;
			}

			// Literals
			{
			case BytecodeOpCode::SetI1:
				ss << "\t";
				PutIndex(it[1]);
				ss << " = ";
				GetInt(it + 2);
				ss << ";" << std::endl;
				i += 5;
				break;
			case BytecodeOpCode::SetI2:
				ss << "\t";
				PutIndex(it[1]);
				ss << " = vec2(";
				GetInt(it + 2);
				ss << ", ";
				GetInt(it + 6);
				ss << ");" << std::endl;
				i += 9;
				break;
			case BytecodeOpCode::SetI3:
				ss << "\t";
				PutIndex(it[1]);
				ss << " = vec3(";
				GetInt(it + 2);
				ss << ", ";
				GetInt(it + 6);
				ss << ", ";
				GetInt(it + 10);
				ss << ");" << std::endl;
				i += 13;
				break;
			case BytecodeOpCode::SetI4:
				ss << "\t";
				PutIndex(it[1]);
				ss << " = vec4(";
				GetInt(it + 2);
				ss << ", ";
				GetInt(it + 6);
				ss << ", ";
				GetInt(it + 10);
				ss << ", ";
				GetInt(it + 14);
				ss << ");" << std::endl;
				i += 17;
				break;

			case BytecodeOpCode::SetF1:
				ss << "\t";
				PutIndex(it[1]);
				ss << " = ";
				GetFloat(it + 2);
				ss << ";" << std::endl;
				i += 5;
				break;
			case BytecodeOpCode::SetF2:
				ss << "\t";
				PutIndex(it[1]);
				ss << " = vec2(";
				GetFloat(it + 2);
				ss << ", ";
				GetFloat(it + 6);
				ss << ");" << std::endl;
				i += 9;
				break;
			case BytecodeOpCode::SetF3:
				ss << "\t";
				PutIndex(it[1]);
				ss << " = vec3(";
				GetFloat(it + 2);
				ss << ", ";
				GetFloat(it + 6);
				ss << ", ";
				GetFloat(it + 10);
				ss << ");" << std::endl;
				i += 13;
				break;
			case BytecodeOpCode::SetF4:
				ss << "\t";
				PutIndex(it[1]);
				ss << " = vec4(";
				GetFloat(it + 2);
				ss << ", ";
				GetFloat(it + 6);
				ss << ", ";
				GetFloat(it + 10);
				ss << ", ";
				GetFloat(it + 14);
				ss << ");" << std::endl;
				i += 17;
				break;
			}

			// Vector component operations
			{
			case BytecodeOpCode::AsI2Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << ".";
				GetComponent(2, it[1]);
				ss << " = ";
				PutIndex(varIn0);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::AsI3Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << ".";
				GetComponent(3, it[1]);
				ss << " = ";
				PutIndex(varIn0);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::AsI4Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << ".";
				GetComponent(4, it[1]);
				ss << " = ";
				PutIndex(varIn0);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::AsF2Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << ".";
				GetComponent(2, it[1]);
				ss << " = ";
				PutIndex(varIn0);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::AsF3Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << ".";
				GetComponent(3, it[1]);
				ss << " = ";
				PutIndex(varIn0);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::AsF4Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << ".";
				GetComponent(4, it[1]);
				ss << " = ";
				PutIndex(varIn0);
				ss << ";" << std::endl;
				i += 1;
				break;

			case BytecodeOpCode::GeI2Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << ".";
				GetComponent(2, it[1]);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::GeI3Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << ".";
				GetComponent(3, it[1]);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::GeI4Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << ".";
				GetComponent(4, it[1]);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::GeF2Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << ".";
				GetComponent(2, it[1]);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::GeF3Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << ".";
				GetComponent(3, it[1]);
				ss << ";" << std::endl;
				i += 1;
				break;
			case BytecodeOpCode::GeF4Cmp:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << ".";
				GetComponent(4, it[1]);
				ss << ";" << std::endl;
				i += 1;
				break;
			}

			default:
				ss << std::endl << "// UNSUPPORTED OPERATION";
				// TO DO: THROW ERROR
				break;
		}
	}
	ss << "}" << std::endl;

	out = ss.str();
}
