#include <Magma/Framework/Input/Entry.h>
#include <Magma/Framework/String/UTF8.h>
#include <Magma/Framework/String/Stream.h>
#include <stdlib.h>

void Main(int argc, char** argv)
{
	mfsUTF8CodeUnit data[] =
	{
		't',
		'e',
		's',
		't',
	};

	mfsError err;

	mfsStream* file;
	err = mfsOpenFile(&file, MFS_FILE_WRITE, u8"../../../../../../resources/Example-C-3/test.txt");
	if (err != MFS_ERROR_OKAY)
		abort();

	err = mfsWrite(file, data, sizeof(data), NULL);
	if (err != MFS_ERROR_OKAY)
		abort();

	err = mfsFlush(file);
	if (err != MFS_ERROR_OKAY)
		abort();

	mfsCloseFile(file);

	err = mfsPrintFormatUTF8(mfsOut, u8"Hello World!\n%d + %d = %f", 2, 3, 2.0f + 3.0f);
	if (err != MFS_ERROR_OKAY)
		abort();

	err = mfsGetByte(mfsIn, NULL);
	if (err != MFS_ERROR_OKAY)
		abort();
}