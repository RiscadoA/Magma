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

const mfsUTF8CodeUnit* mffMagmaRootDirectory;

#ifdef __cplusplus
extern "C"
{
#endif

#define MFF_FILE_NAME_MAX_SIZE						16
#define MFF_PATH_MAX_SIZE							256

#define MFF_FILE		0x01
#define MFF_ARCHIVE		0x02

	typedef mfmU32 mffEnum;

	const mfsUTF8CodeUnit * mffGoDownPath(const mfsUTF8CodeUnit* path);

	const mfsUTF8CodeUnit * mffGetPathName(const mfsUTF8CodeUnit* path);

	const mfsUTF8CodeUnit * mffGetPathExtension(const mfsUTF8CodeUnit* path);
	
	mffEnum mffGetPathType(const mfsUTF8CodeUnit* path);

#ifdef __cplusplus
}
#endif
