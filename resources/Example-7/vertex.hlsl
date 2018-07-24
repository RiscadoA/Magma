// This shader was automatically generated from binary bytecode by the D3D11Assembler::Assemble function
// Vertex shader
// DO NOT MODIFY THIS FILE BY HAND

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
	float4 local_32;
{
	float4 local_33;
{
	float4 local_34;
{
	float local_35;
	local_35 = input.in_16.x;
	local_34.x = local_35;
	local_35 = input.in_16.y;
	local_34.y = local_35;
	local_35 = input.in_16.z;
	local_34.z = local_35;
	local_35 = 1;
	local_34.w = local_35;
}
	float4 local_36;
	local_36 = float4(0, 0, 0, 0);
	local_33 = local_36 - local_34;
}
	local_32 = local_33;
}
{
	output.v_pos = local_32;
}
{
	output.out_8 = input.in_17;
}
}
	return output;
}
