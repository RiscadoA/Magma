// This shader was automatically generated from binary bytecode by the D3D11Assembler::Assemble function
// Pixel shader
// DO NOT MODIFY THIS FILE BY HAND

struct ShaderInput
{
	float4 in_8 : VOUT8VOUT;
};

struct ShaderOutput
{
	float4 out_0 : SV_Depth;
};

ShaderOutput PS(ShaderInput input)
{
	ShaderOutput output;
{
{
	output.out_0 = input.in_8;
}
}
	return output;
}
