#version 410 core

// This shader was automatically generated from binary bytecode by the OGL410Assembler::Assemble function
// Vertex shader
// DO NOT MODIFY THIS FILE BY HAND

layout (std140) uniform buf_0
{
	mat4 buf_0_11; // Constant buffer variable ""; index 11; buffer index 0
};

layout (location = 9) in vec4 in_9; // Input variable "POSITION"; index 9
layout (location = 10) in vec4 in_10; // Input variable "COLOR"; index 10

layout (location = 8) out vec4 out_8; // Output variable "OUT_0"; index 8

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
{
{
	vec4 local_12;
	local_12 = buf_0_11 * in_9;
	gl_Position = local_12;
}
{
	out_8 = in_10;
}
{
	float local_13;
	float local_14;
	local_14 = 1;
	local_13 = cos(local_14);
	out_8.x = local_13;
}
{
	float local_15;
{
	float local_16;
	local_16 = 1;
	float local_17;
	local_17 = out_8.z;
	local_15 = local_16 + local_17;
}
	out_8.y = local_15;
}
}
}
