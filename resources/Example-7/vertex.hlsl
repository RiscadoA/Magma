// This shader was automatically generated from binary bytecode by the D3D11Assembler::Assemble function
// Vertex shader
// DO NOT MODIFY THIS FILE BY HAND

cbuffer buf_0 // Constant buffer "TRANSFORM"; index 0
{
	float4x4 buf_0_11; // Constant buffer variable ""; index 11; buffer index 0
};

struct ShaderInput
{
	float4 in_9 : IN8IN;
	float4 in_10 : IN9IN;
};

struct ShaderOutput
{
	float4 out_8 : VOUT8VOUT;
	float4 v_pos : SV_POSITION;
};

ShaderOutput VS(ShaderInput input)
{
	ShaderOutput output;
{
{
	float4 local_12;
	local_12 = mul(buf_0_11, input.in_9);
	output.v_pos = local_12;
}
{
	output.out_8 = input.in_10;
}
{
	float local_13;
	float local_14;
	local_14 = 1;
	local_13 = cos(local_14);
	output.out_8.x = local_13;
}
{
	float local_15;
{
	float local_16;
	local_16 = 1;
	float local_17;
	local_17 = output.out_8.z;
	local_15 = local_16 + local_17;
}
	output.out_8.y = local_15;
}
}
	return output;
}
