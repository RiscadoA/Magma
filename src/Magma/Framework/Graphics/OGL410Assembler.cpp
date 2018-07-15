#include "OGL410Assembler.hpp"
#include "BytecodeAssembler.hpp"

#include "../String/Conversion.hpp"

#include <sstream>
#include <stack>
#include <map>

using namespace Magma::Framework::Graphics;
using namespace Magma::Framework;

void TOGL410(ShaderVariableType type, std::stringstream& out)
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
			throw ShaderError("Failed to assemble from binary bytecode on OGL410Assembler:\nInvalid shader variable type");
			break;
		default:
			throw ShaderError("Failed to assemble from binary bytecode on OGL410Assembler:\nUnsupported shader variable type");
			break;
	}
}

void Magma::Framework::Graphics::OGL410Assembler::Assemble(const ShaderData & data, std::string & out)
{
	std::stringstream ss;
	
	ss << "#version 410 core" << std::endl << std::endl;
	ss << "// This shader was automatically generated from binary bytecode by the OGL410Assembler::Assemble function" << std::endl;
	switch (data.GetShaderType())
	{
		case ShaderType::Vertex:
			ss << "// Vertex shader" << std::endl;
			break;
		case ShaderType::Pixel:
			ss << "// Pixel shader" << std::endl;
			break;
		case ShaderType::Invalid:
			throw ShaderError("Failed to assemble from binary bytecode on OGL410Assembler:\nInvalid shader type");
			break;
		default:
			throw ShaderError("Failed to assemble from binary bytecode on OGL410Assembler:\nUnsupported shader type");
			break;
	}
	ss << "// DO NOT MODIFY THIS FILE BY HAND" << std::endl;
	ss << std::endl;

	// Add texture 2D variables
	for (auto& tex : data.GetTexture2DVariables())
	{
		ss << "uniform sampler2D";
		ss << " tex_" << tex.index << "; // Texture2D variable \"" << tex.name << "\"; index " << tex.index;
		ss << std::endl;
	}
	if (data.GetTexture2DVariables().size() > 0)
		ss << std::endl;

	// Add constant buffers
	for (auto& buf : data.GetShaderConstantBuffers())
	{
		ss << "layout (std140) uniform buf_" << buf.index << std::endl;
		ss << "{" << std::endl;
		for (auto& v : data.GetShaderConstantVariables())
		{
			if (v.bufferIndex != buf.index)
				continue;
			ss << "\t";
			TOGL410(v.type, ss);
			ss << " buf_" << buf.index << "_" << v.index << "; // Constant buffer variable \"" << v.name << "\"; index " << v.index << "; buffer index " << v.bufferIndex;
			ss << std::endl;
		}
		ss << "};" << std::endl << std::endl;
	}

	// Add input variables
	for (auto& in : data.GetInputVariables())
	{
		ss << "layout (location = " << in.index << ") in ";
		TOGL410(in.type, ss);
		ss << " in_" << in.index << "; // Input variable \"" << in.name << "\"; index " << in.index;
		ss << std::endl;
	}
	if (data.GetInputVariables().size() > 0)
		ss << std::endl;

	// Add output variables
	for (auto& out : data.GetOutputVariables())
	{
		ss << "layout (location = " << out.index << ") out ";
		TOGL410(out.type, ss);
		ss << " out_" << out.index << "; // Output variable \"" << out.name << "\"; index " << out.index;
		ss << std::endl;
	}
	if (data.GetOutputVariables().size() > 0)
		ss << std::endl;
	
	if (data.GetShaderType() == ShaderType::Vertex)
	{
		ss << "out gl_PerVertex" << std::endl;
		ss << "{" << std::endl;
		ss << "\tvec4 gl_Position;" << std::endl;
		ss << "};" << std::endl;
		ss << std::endl;
	}

	/*
		TO DO:
		- Add 2D texture variables
		- Add constant buffer variables
	*/

	auto GetComponent = [&](size_t count, size_t index) -> void
	{
		if (index >= count)
			throw ShaderError("Failed to assemble from binary bytecode on OGL410Assembler:\nFailed to get vector component, out of bounds");
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
			case BytecodeOpCode::Add:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << " + ";
				PutIndex(varIn1);
				ss << ";" << std::endl;
				break;
			case BytecodeOpCode::Sub:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << " - ";
				PutIndex(varIn1);
				ss << ";" << std::endl;
				break;
			case BytecodeOpCode::Mul:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << " * ";
				PutIndex(varIn1);
				ss << ";" << std::endl;
				break;
			case BytecodeOpCode::Div:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << " / ";
				PutIndex(varIn1);
				ss << ";" << std::endl;
				break;
			case BytecodeOpCode::Mod:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << " % ";
				PutIndex(varIn1);
				ss << ";" << std::endl;
				break;
			case BytecodeOpCode::And:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << " && ";
				PutIndex(varIn1);
				ss << ";" << std::endl;
				break;
			case BytecodeOpCode::Or:
				ss << "\t";
				PutIndex(varOut);
				ss << " = ";
				PutIndex(varIn0);
				ss << " || ";
				PutIndex(varIn1);
				ss << ";" << std::endl;
				break;
			case BytecodeOpCode::Not:
				ss << "\t";
				PutIndex(varOut);
				ss << " = !(";
				PutIndex(varIn0);
				ss << ");" << std::endl;
				break;
			case BytecodeOpCode::Greater:
				ss << "\t";
				PutIndex(varOut);
				ss << " = (";
				PutIndex(varIn0);
				ss << " > ";
				PutIndex(varIn1);
				ss << ");" << std::endl;
				break;
			case BytecodeOpCode::Less:
				ss << "\t";
				PutIndex(varOut);
				ss << " = (";
				PutIndex(varIn0);
				ss << " < ";
				PutIndex(varIn1);
				ss << ");" << std::endl;
				break;
			case BytecodeOpCode::GEqual:
				ss << "\t";
				PutIndex(varOut);
				ss << " = (";
				PutIndex(varIn0);
				ss << " >= ";
				PutIndex(varIn1);
				ss << ");" << std::endl;
				break;
			case BytecodeOpCode::LEqual:
				ss << "\t";
				PutIndex(varOut);
				ss << " = (";
				PutIndex(varIn0);
				ss << " <= ";
				PutIndex(varIn1);
				ss << ");" << std::endl;
				break;
			case BytecodeOpCode::Equal:
				ss << "\t";
				PutIndex(varOut);
				ss << " = (";
				PutIndex(varIn0);
				ss << " == ";
				PutIndex(varIn1);
				ss << ");" << std::endl;
				break;
			case BytecodeOpCode::NEqual:
				ss << "\t";
				PutIndex(varOut);
				ss << " = (";
				PutIndex(varIn0);
				ss << " != ";
				PutIndex(varIn1);
				ss << ");" << std::endl;
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

			// Scope and flow control
			{
			case BytecodeOpCode::OpScope:
				ss << "{" << std::endl;
				break;
			case BytecodeOpCode::ClScope:
				ss << "}" << std::endl;
				break;
			case BytecodeOpCode::Discard:
				ss << "\tdiscard;" << std::endl;
				break;
			case BytecodeOpCode::Return:
				ss << "\treturn;" << std::endl;
				break;
			case BytecodeOpCode::While:
				ss << "\twhile (";
				PutIndex(varIn0);
				i += 1;
				ss << " != 0)" << std::endl;
				break;
			case BytecodeOpCode::If:
				ss << "\tif (";
				PutIndex(varIn0);
				i += 1;
				ss << " != 0)" << std::endl;
				break;
			case BytecodeOpCode::Else:
				ss << "\telse" << std::endl;
				break;
			}

			// Other functions
			{
				case BytecodeOpCode::MulMat:
					ss << "\t";
					PutIndex(varOut);
					ss << " = ";
					PutIndex(varIn0);
					ss << " * ";
					PutIndex(varIn1);
					ss << ";" << std::endl;
					break;

				case BytecodeOpCode::Smple2D:
					ss << "\t";
					PutIndex(varOut);
					ss << " = texture(";
					PutIndex(varIn0);
					ss << " , ";
					PutIndex(varIn1);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Cos:
					ss << "\t";
					PutIndex(varOut);
					ss << " = cos(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Sin:
					ss << "\t";
					PutIndex(varOut);
					ss << " = sin(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Tan:
					ss << "\t";
					PutIndex(varOut);
					ss << " = tan(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::ACos:
					ss << "\t";
					PutIndex(varOut);
					ss << " = acos(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::ASin:
					ss << "\t";
					PutIndex(varOut);
					ss << " = asin(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::ATan:
					ss << "\t";
					PutIndex(varOut);
					ss << " = atan(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Degrees:
					ss << "\t";
					PutIndex(varOut);
					ss << " = degrees(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Radians:
					ss << "\t";
					PutIndex(varOut);
					ss << " = radians(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Pow:
					ss << "\t";
					PutIndex(varOut);
					ss << " = pow(";
					PutIndex(varIn0);
					ss << " , ";
					PutIndex(varIn1);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Exp:
					ss << "\t";
					PutIndex(varOut);
					ss << " = exp(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Log:
					ss << "\t";
					PutIndex(varOut);
					ss << " = log(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Exp2:
					ss << "\t";
					PutIndex(varOut);
					ss << " = exp2(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Log2:
					ss << "\t";
					PutIndex(varOut);
					ss << " = log2(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Sqrt:
					ss << "\t";
					PutIndex(varOut);
					ss << " = sqrt(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::ISqrt:
					ss << "\t";
					PutIndex(varOut);
					ss << " = inversesqrt(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Abs:
					ss << "\t";
					PutIndex(varOut);
					ss << " = abs(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Sign:
					ss << "\t";
					PutIndex(varOut);
					ss << " = sign(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Floor:
					ss << "\t";
					PutIndex(varOut);
					ss << " = floor(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Ceil:
					ss << "\t";
					PutIndex(varOut);
					ss << " = ceil(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;

				case BytecodeOpCode::Fract:
					ss << "\t";
					PutIndex(varOut);
					ss << " = fract(";
					PutIndex(varIn0);
					ss << ");" << std::endl;
					break;
			}

			default:
				throw ShaderError("Failed to assemble from binary bytecode on OGL410Assembler:\nUnsupported/unknown operation code");
				break;
		}
	}
	ss << "}" << std::endl;

	out = ss.str();
}
