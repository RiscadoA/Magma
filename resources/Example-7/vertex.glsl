#version 410 core

// This shader was automatically generated from binary bytecode by the OGL410Assembler::Assemble function
// Vertex shader
// DO NOT MODIFY THIS FILE BY HAND

layout (std140) uniform buf_0
{
	mat4 buf_0_32; // Constant buffer variable ""; index 32; buffer index 0
};

layout (location = 16) in vec4 in_16; // Input variable "POSITION"; index 16
layout (location = 17) in vec4 in_17; // Input variable "COLOR"; index 17

layout (location = 8) out vec4 out_8; // Output variable "OUT_0"; index 8

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
{
{
	vec4 local_33;
	local_33 = buf_0_32 * in_16;
	gl_Position = local_33;
}
{
	out_8 = in_17;
}
{
	float local_34;
	float local_35;
	local_35 = 1;
	local_34 = cos(local_35);
	out_8.x = local_34;
}
{
	float local_36;
{
	float local_37;
	local_37 = 1;
	float local_38;
	local_38 = out_8.z;
	local_36 = local_39 + local_37;
}
	out_8.y = local_36;
}
}
}
