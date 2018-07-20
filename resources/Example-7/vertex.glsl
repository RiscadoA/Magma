#version 410 core

// This shader was automatically generated from binary bytecode by the OGL410Assembler::Assemble function
// Vertex shader
// DO NOT MODIFY THIS FILE BY HAND

layout (location = 32) in vec4 in_32; // Input variable "POSITION"; index 32
layout (location = 33) in vec4 in_33; // Input variable "COLOR"; index 33

layout (location = 9) out vec4 out_9; // Output variable "OUT_0"; index 9

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
{
{
	gl_Position = in_32;
}
{
	out_9 = in_33;
}
}
}
