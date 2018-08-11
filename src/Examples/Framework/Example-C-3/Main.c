#include <Magma/Framework/Input/Entry.h>
#include <Magma/Framework/String/UTF8.h>
#include <Magma/Framework/String/Stream.h>
#include <stdlib.h>

void Main(int argc, char** argv)
{
	mfsUTF8CodeUnit data[] =
	{
		'o',
		'l',
		'a',
		'\n',
	};

	mfsError err;

	err = mfsWrite(mfsOut, data, sizeof(data), NULL);
	if (err != MFS_ERROR_OKAY)
		abort();

	err = mfsFlush(mfsOut);
	if (err != MFS_ERROR_OKAY)
		abort();

	err = mfsRead(mfsIn, data, 3, NULL);
	if (err != MFS_ERROR_OKAY)
		abort();

	err = mfsWrite(mfsOut, data, 3, NULL);
	if (err != MFS_ERROR_OKAY)
		abort();

	err = mfsFlush(mfsOut);
	if (err != MFS_ERROR_OKAY)
		abort();

	for (;;);
}