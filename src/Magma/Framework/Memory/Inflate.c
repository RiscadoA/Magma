#include "Deflate.h"

#include <zlib.h>

typedef struct
{
	mfsStream base;
	z_stream stream;
} mfmInflateStream;


