#version 410 core

// This shader was automatically generated from binary bytecode by the OGL410Assembler::Assemble function
// Vertex shader
// DO NOT MODIFY THIS FILE BY HAND

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
		vec4 local_32;
		{
			vec4 local_33;
			{
				vec4 local_34;
				{
					float local_35;
					local_35 = in_16.x;
					local_34.x = local_35;
					local_35 = in_16.y;
					local_34.y = local_35;
					local_35 = in_16.z;
					local_34.z = local_35;
					local_35 = 1;
					local_34.w = local_35;
				}
				vec4 local_36;
				local_36 = vec4(0, 0, 0, 0);
				local_33 = local_36 - local_34;
			}
			local_32 = local_33;
		}
		{
			gl_Position = local_32;
		}
		{
			out_8 = in_17;
		}
	}
}
