#pragma once

/*
	Framework entry point.
*/

#ifdef __cplusplus
extern "C"
{
#endif

	// Magma framework entry point
	void mfEntry(int argc, char** argv);

	// Program entry point
	extern void Main(int argc, char** argv);

#ifdef __cplusplus
}
#endif
