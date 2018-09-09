#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "RenderDevice.h"
#include "Error.h"
#include "../Memory/Object.h"

	typedef struct
	{
		mfsUTF8CodeUnit type[4];
		mfmI32 size;
	} mfaWAVFileHeader;

	typedef struct
	{
		mfsUTF8CodeUnit name[4];
		mfmI32 size;
	} mfaWAVChunkHeader;

	typedef struct
	{
		mfaWAVChunkHeader header;
		mfaEnum format;
		mfmI32 sampleRate;
		mfmI16 channelCount;
		mfmI16 bitsPerSample;
	} mfaWAVFormatChunk;

	typedef struct
	{
		mfaWAVChunkHeader header;
		void* pcmData;
	} mfaWAVDataChunk;

	typedef struct
	{
		mfaWAVFileHeader fileHeader;
		mfaWAVFormatChunk formatChunk;
		mfaWAVDataChunk dataChunk;
	} mfaWAVData;

	/// <summary>
	///		Loads a whole WAV file.
	///		The PCM data must be freed later with mfmDeallocate.
	/// </summary>
	/// <param name="stream">Input data stream handle</param>
	/// <param name="data">Out WAV data</param>
	/// <param name="allocator">Allocator where the PCM data will be allocated on</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfaLoadWAV(void* stream, mfaWAVData* data, void* allocator);

	/// <summary>
	///		Loads a WAV file header.
	/// </summary>
	/// <param name="stream">Input data stream handle</param>
	/// <param name="out">Out WAV file header</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfaLoadWAVFileHeader(void* stream, mfaWAVFileHeader* out);
	
	/// <summary>
	///		Loads a WAV chunk header.
	/// </summary>
	/// <param name="stream">Input data stream handle</param>
	/// <param name="out">Out WAV chunk header</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfaLoadWAVChunkHeader(void* stream, mfaWAVChunkHeader* out);

	/// <summary>
	///		Loads a WAV format chunk.
	/// </summary>
	/// <param name="stream">Input data stream handle</param>
	/// <param name="out">Out WAV format chunk</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfaLoadWAVFormatChunk(void* stream, mfaWAVFormatChunk* out);

	/// <summary>
	///		Loads a WAV format chunk without loading the header (header already loaded).
	/// </summary>
	/// <param name="stream">Input data stream handle</param>
	/// <param name="out">Out WAV format chunk</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfaLoadWAVFormatChunkNoHeader(void* stream, mfaWAVFormatChunk* out);

	/// <summary>
	///		Loads a WAV data chunk.
	/// </summary>
	/// <param name="stream">Input data stream handle</param>
	/// <param name="out">Out WAV data chunk</param>
	/// <param name="format">WAV format chunk</param>
	/// <param name="allocator">Allocator where the PCM data will be allocated on</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfaLoadWAVDataChunk(void* stream, mfaWAVDataChunk* out, const mfaWAVFormatChunk* format, void* allocator);

	/// <summary>
	///		Loads a WAV data chunk without loading the header (header already loaded).
	/// </summary>
	/// <param name="stream">Input data stream handle</param>
	/// <param name="out">Out WAV data chunk</param>
	/// <param name="format">WAV format chunk</param>
	/// <param name="allocator">Allocator where the PCM data will be allocated on</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfaLoadWAVDataChunkNoHeader(void* stream, mfaWAVDataChunk* out, const mfaWAVFormatChunk* format, void* allocator);

#ifdef __cplusplus
}
#endif
