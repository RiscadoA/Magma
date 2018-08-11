// This shader was automatically generated from binary bytecode by the D3D11Assembler::Assemble function
// Pixel shader
// DO NOT MODIFY THIS FILE BY HAND

struct ShaderInput
{
	float4 in_8 : VOUT8VOUT;
};

struct ShaderOutput
{
	float4 out_1 : SV_Target0;
};

ShaderOutput PS(ShaderInput input)
{
	ShaderOutput output;
{
{
	output.out_1 = input.in_8;
}
}
	return output;
}
