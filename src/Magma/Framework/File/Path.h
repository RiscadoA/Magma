#pragma once

#include "Error.h"

#include "../String/UTF8.h"
#include "../String/Stream.h"

/*
	Paths are always relative to the an archive.
	'..' is not allowed.
	Valid path examples:

	/perm/assets.zip/test.txt
	/model.obj
	/audio/audio.wav
*/

#ifdef __cplusplus
extern "C"
{
#endif

	extern const mfsUTF8CodeUnit* mffMagmaRootDirectory;

#define MFF_FILE_NAME_MAX_SIZE						16
#define MFF_PATH_MAX_SIZE							256

	typedef mfmU32 mffEnum;

	const mfsUTF8CodeUnit * mffGoDownPath(const mfsUTF8CodeUnit* path);

	const mfsUTF8CodeUnit * mffGetPathName(const mfsUTF8CodeUnit* path);

	const mfsUTF8CodeUnit * mffGetPathExtension(const mfsUTF8CodeUnit* path);

#ifdef __cplusplus
}
#endif
