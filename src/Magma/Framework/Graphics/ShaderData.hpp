#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Describes a shader variable type
			/// </summary>
			enum class ShaderDataVariableType : unsigned long
			{
				/// <summary>
				///		Invalid variable type
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Integer type
				/// </summary>
				Int1 = 0,

				/// <summary>
				///		Two component integer vector type
				/// </summary>
				Int2 = 1,

				/// <summary>
				///		Three component integer vector type
				/// </summary>
				Int3 = 2,

				/// <summary>
				///		Four component integer vector type
				/// </summary>
				Int4 = 3,

				/// <summary>
				///		2x2 integer matrix type
				/// </summary>
				Int22 = 4,
				
				/// <summary>
				///		3x3 integer matrix type
				/// </summary>
				Int33 = 5,

				/// <summary>
				///		4x4 integer matrix type
				/// </summary>
				Int44 = 6,

				/// <summary>
				///		Floating point type
				/// </summary>
				Float1 = 7,

				/// <summary>
				///		Two component floating point vector type
				/// </summary>
				Float2 = 8,

				/// <summary>
				///		Three component floating point vector type
				/// </summary>
				Float3 = 9,

				/// <summary>
				///		Four component floating point vector type
				/// </summary>
				Float4 = 10,

				/// <summary>
				///		2x2 floating point matrix type
				/// </summary>
				Float22 = 11,

				/// <summary>
				///		3x3 floating point matrix type
				/// </summary>
				Float33 = 12,

				/// <summary>
				///		4x4 floating point matrix type
				/// </summary>
				Float44 = 13,

				/// <summary>
				///		Variable type count
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a shader type
			/// </summary>
			enum class ShaderType : unsigned long
			{
				/// <summary>
				///		Invalid shader type
				/// </summary>
				Invalid = -1,

				/// <summary>
				///		Vertex shaders
				/// </summary>
				Vertex = 0,

				/// <summary>
				///		Pixel shaders
				/// </summary>
				Pixel = 1,

				/// <summary>
				///		Shader type count
				/// </summary>
				Count
			};

			/// <summary>
			///		Describes a shader input variable
			/// </summary>
			struct ShaderInputVariable
			{
				/// <summary>
				///		Shader input variable name
				/// </summary>
				std::string name;

				/// <summary>
				///		Shader input variable index
				/// </summary>
				unsigned long index;

				/// <summary>
				///		Shader variable type
				/// </summary>
				ShaderDataVariableType type;
			};

			/// <summary>
			///		Describes a shader output variable
			/// </summary>
			struct ShaderOutputVariable
			{
				/// <summary>
				///		Shader output variable name
				/// </summary>
				std::string name;

				/// <summary>
				///		Shader output variable index
				/// </summary>
				unsigned long index;

				/// <summary>
				///		Shader variable type
				/// </summary>
				ShaderDataVariableType type;
			};

			/// <summary>
			///		Describes a shader 2D texture variable
			/// </summary>
			struct ShaderTexture2DVariable
			{
				/// <summary>
				///		Shader 2D texture variable name
				/// </summary>
				std::string name;

				/// <summary>
				///		Shader 2D texture variable index
				/// </summary>
				unsigned long index;
			};

			/// <summary>
			///		Describes a shader constant buffer variable
			/// </summary>
			struct ShaderConstantBufferVariable
			{
				/// <summary>
				///		Shader constant buffer variable name
				/// </summary>
				std::string name;

				/// <summary>
				///		Shader constant buffer variable index
				/// </summary>
				unsigned long index;

				/// <summary>
				///		Shader constant buffer variable buffer offset
				/// </summary>
				unsigned long offset;

				/// <summary>
				///		Shader constant buffer index
				/// </summary>
				unsigned long bufferIndex;

				/// <summary>
				///		Shader variable type
				/// </summary>
				ShaderDataVariableType type;
			};

			/// <summary>
			///		Describes a shader constant buffer
			/// </summary>
			struct ShaderConstantBuffer
			{
				/// <summary>
				///		Shader constant buffer name
				/// </summary>
				std::string name;

				/// <summary>
				///		Shader constant buffer index
				/// </summary>
				unsigned long index;
			};

			/// <summary>
			///		Holds data for shader creation
			/// </summary>
			class ShaderData final
			{
			public:
				/// <summary>
				///		Creates a shader data container (makes a copy from the input data and stores it)
				/// </summary>
				/// <exception cref="Magma::Framework::Graphics::ShaderError">Thrown if there is an error loading the meta data</exception>
				ShaderData(const char* binaryObject, size_t binaryObjectSize);

				/// <summary>
				///		Creates a shader data container	(makes a copy from the input data and stores it)
				/// </summary>
				/// <exception cref="Magma::Framework::Graphics::ShaderError">Thrown if there is an error loading the meta data</exception>
				ShaderData(const char* bytecode, size_t bytecodeSize, const char* metaData, size_t metaDataSize);

				/// <summary>
				///		Destroys this shader data container
				/// </summary>
				~ShaderData();

				/// <summary>
				///		Moves a shader data container
				/// </summary>
				ShaderData(ShaderData&& rhs);

				/// <summary>
				///		Creates a copy of a shader data container
				/// </summary>
				ShaderData(const ShaderData& rhs);

				/// <summary>
				///		Gets a const pointer to the shader's bytecode
				/// </summary>
				/// <returns>Const pointer to the shader's bytecode</returns>
				inline const char* GetBytecode() const { return m_bytecode; }

				/// <summary>
				///		Gets the shader's bytecode's size in bytes
				/// </summary>
				/// <returns>Shader's bytecode's size in bytes</returns>
				inline size_t GetBytecodeSize() const { return m_bytecodeSize; }

				/// <summary>
				///		Gets a const pointer to the shader's meta data
				/// </summary>
				/// <returns>Const pointer to the shader's meta data</returns>
				inline const char* GetMetaData() const { return m_metaData; }

				/// <summary>
				///		Gets the shader's meta data's size in bytes
				/// </summary>
				/// <returns>Shader's meta data's size in bytes</returns>
				inline size_t GetMetaDataSize() const { return m_metaDataSize; }

				/// <summary>
				///		Gets the shader's major version
				/// </summary>
				/// <returns>Shader's major version</returns>
				inline unsigned long GetMajorVersion() const { return m_majorVersion; }

				/// <summary>
				///		Gets the shader's minor version
				/// </summary>
				/// <returns>Shader's minor version</returns>
				inline unsigned long GetMinorVersion() const { return m_minorVersion; }

				/// <summary>
				///		Gets the shader's type
				/// </summary>
				/// <returns>Shader's type</returns>
				inline ShaderType GetShaderType() const { return m_shaderType; }

				/// <summary>
				///		Gets the shader's input variables
				/// </summary>
				/// <returns>Shader's input variables</returns>
				inline const std::vector<ShaderInputVariable>& GetInputVariables() const { return m_inputVars; }
				
				/// <summary>
				///		Gets the shader's output variables
				/// </summary>
				/// <returns>Shader's output variables</returns>
				inline const std::vector<ShaderOutputVariable>& GetOutputVariables() const { return m_outputVars; }
				
				/// <summary>
				///		Gets the shader's 2D texture variables
				/// </summary>
				/// <returns>Shader's 2D texture variables</returns>
				inline const std::vector<ShaderTexture2DVariable>& GetTexture2DVariables() const { return m_texture2DVars; }

				/// <summary>
				///		Gets the shader's constant buffer variables
				/// </summary>
				/// <returns>Shader's constant buffer variables</returns>
				inline const std::vector<ShaderConstantBufferVariable>& GetShaderConstantVariables() const { return m_constantBufferVars; }

				/// <summary>
				///		Gets the shader's constant buffers
				/// </summary>
				/// <returns>Shader's constant buffers</returns>
				inline const std::vector<ShaderConstantBuffer>& GetShaderConstantBuffers() const { return m_constantBuffers; }

			private:
				void Load();

				char* m_bytecode;
				size_t m_bytecodeSize;

				char* m_metaData;
				size_t m_metaDataSize;

				unsigned long m_majorVersion;
				unsigned long m_minorVersion;
				ShaderType m_shaderType;
				std::vector<ShaderInputVariable> m_inputVars;
				std::vector<ShaderOutputVariable> m_outputVars;
				std::vector<ShaderTexture2DVariable> m_texture2DVars;
				std::vector<ShaderConstantBufferVariable> m_constantBufferVars;
				std::vector<ShaderConstantBuffer> m_constantBuffers;
			};
		}
	}
}