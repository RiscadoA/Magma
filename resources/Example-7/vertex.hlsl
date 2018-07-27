// This shader was automatically generated from binary bytecode by the D3D11Assembler::Assemble function
// Vertex shader
// DO NOT MODIFY THIS FILE BY HAND

cbuffer buf_0 // Constant buffer "TRANSFORM"; index 0
{
	float4x4 buf_0_32; // Constant buffer variable ""; index 32; buffer index 0
};

struct ShaderInput
{
	float4 in_16 : IN15IN;
	float4 in_17 : IN16IN;
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
	float4 local_33;
	local_33 = mul(buf_0_32, input.in_16);
	output.v_pos = local_33;
}
{
	output.out_8 = input.in_17;
}
{
	float local_34;
	float local_35;
	local_35 = 1;
	local_34 = cos(local_35);
	output.out_8.x = local_34;
}
{
	float local_36;
{
	float local_37;
	local_37 = 1;
	float local_38;
	local_38 = output.out_8.z;
	local_36 = local_39 + local_37;
}
	output.out_8.y = local_36;
}
}
	return output;
}
