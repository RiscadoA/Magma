#include <Magma/Framework/Input/Entry.h>
#include <Magma/Framework/Graphics/D3D11Assembler.h>
#include <stdlib.h>
#include <stdio.h>

void Main(int argc, char** argv)
{
	mfmU8 bytecode[] =
	{
		't',
		'e',
		's',
		't',
	};

	mfgD3D11Assemble(bytecode, sizeof(bytecode));
}