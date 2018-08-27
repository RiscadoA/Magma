#pragma once

#include "../../Error.h"
#include "../../../String/UTF8.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef mfmU32 mfgV2XEnum;

#define MFG_TOKEN_ATTRIBUTE_SIZE 32
#define MFG_MAX_ERROR_MESSAGE_SIZE 512

	typedef struct
	{
		mfgV2XEnum type;
		mfmBool isType;
		mfmBool isOperator;
		mfmBool isPunctuation;
		const mfsUTF8CodeUnit* name;
	} mfgV2XTokenInfo;

	typedef struct
	{
		mfgV2XTokenInfo* info;
		mfsUTF8CodeUnit attribute[MFG_TOKEN_ATTRIBUTE_SIZE];
	} mfgV2XToken;

	typedef struct
	{
		mfsUTF8CodeUnit errorMsg[MFG_MAX_ERROR_MESSAGE_SIZE];
		mfmU64 tokenCount;
	} mfgV2XLexerState;

	
#define MFG_V2X_TOKEN_VOID										0x0000
static const mfgV2XTokenInfo MFG_V2X_TINFO_VOID					= { MFG_V2X_TOKEN_VOID, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"void" };
#define MFG_V2X_TOKEN_INT1										0x0001
static const mfgV2XTokenInfo MFG_V2X_TINFO_INT1					= { MFG_V2X_TOKEN_INT1, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"int1" };
#define MFG_V2X_TOKEN_INT2										0x0002
static const mfgV2XTokenInfo MFG_V2X_TINFO_INT2					= { MFG_V2X_TOKEN_INT2, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"int2" };
#define MFG_V2X_TOKEN_INT3										0x0003
static const mfgV2XTokenInfo MFG_V2X_TINFO_INT3					= { MFG_V2X_TOKEN_INT3, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"int3" };
#define MFG_V2X_TOKEN_INT4										0x0004
static const mfgV2XTokenInfo MFG_V2X_TINFO_INT4					= { MFG_V2X_TOKEN_INT4, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"int4" };
#define MFG_V2X_TOKEN_INT22										0x0005
static const mfgV2XTokenInfo MFG_V2X_TINFO_INT22				= { MFG_V2X_TOKEN_INT2, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"int2x2" };
#define MFG_V2X_TOKEN_INT33										0x0006
static const mfgV2XTokenInfo MFG_V2X_TINFO_INT33				= { MFG_V2X_TOKEN_INT3, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"int3x3" };
#define MFG_V2X_TOKEN_INT44										0x0007
static const mfgV2XTokenInfo MFG_V2X_TINFO_INT44				= { MFG_V2X_TOKEN_INT4, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"int4x4" };
#define MFG_V2X_TOKEN_FLOAT1									0x0008
static const mfgV2XTokenInfo MFG_V2X_TINFO_FLOAT1				= { MFG_V2X_TOKEN_FLOAT1, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"float1" };
#define MFG_V2X_TOKEN_FLOAT2									0x0009
static const mfgV2XTokenInfo MFG_V2X_TINFO_FLOAT2				= { MFG_V2X_TOKEN_FLOAT2, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"float2" };
#define MFG_V2X_TOKEN_FLOAT3									0x000A
static const mfgV2XTokenInfo MFG_V2X_TINFO_FLOAT3				= { MFG_V2X_TOKEN_FLOAT3, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"float3" };
#define MFG_V2X_TOKEN_FLOAT4									0x000B
static const mfgV2XTokenInfo MFG_V2X_TINFO_FLOAT4				= { MFG_V2X_TOKEN_FLOAT4, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"float4" };
#define MFG_V2X_TOKEN_FLOAT22									0x000C
static const mfgV2XTokenInfo MFG_V2X_TINFO_FLOAT22				= { MFG_V2X_TOKEN_FLOAT2, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"float2x2" };
#define MFG_V2X_TOKEN_FLOAT33									0x000D
static const mfgV2XTokenInfo MFG_V2X_TINFO_FLOAT33				= { MFG_V2X_TOKEN_FLOAT3, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"float3x3" };
#define MFG_V2X_TOKEN_FLOAT44									0x000E
static const mfgV2XTokenInfo MFG_V2X_TINFO_FLOAT44				= { MFG_V2X_TOKEN_FLOAT4, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"float4x4" };
#define MFG_V2X_TOKEN_BOOL										0x000F
static const mfgV2XTokenInfo MFG_V2X_TINFO_BOOL					= { MFG_V2X_TOKEN_BOOL, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"bool" };

#define MFG_V2X_TOKEN_INT_LITERAL								0x0010
static const mfgV2XTokenInfo MFG_V2X_TINFO_INT_LITERAL			= { MFG_V2X_TOKEN_INT_LITERAL, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"int-literal" };
#define MFG_V2X_TOKEN_FLOAT_LITERAL								0x0011
static const mfgV2XTokenInfo MFG_V2X_TINFO_FLOAT_LITERAL		= { MFG_V2X_TOKEN_FLOAT_LITERAL, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"float-literal" };
#define MFG_V2X_TOKEN_IDENTIFIER								0x0012
static const mfgV2XTokenInfo MFG_V2X_TINFO_IDENTIFIER			= { MFG_V2X_TOKEN_IDENTIFIER, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"identifier" };
#define MFG_V2X_TOKEN_IF										0x0013
static const mfgV2XTokenInfo MFG_V2X_TINFO_IF					= { MFG_V2X_TOKEN_IF, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"if" };
#define MFG_V2X_TOKEN_ELSE										0x0014
static const mfgV2XTokenInfo MFG_V2X_TINFO_ELSE					= { MFG_V2X_TOKEN_ELSE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"else" };
#define MFG_V2X_TOKEN_WHILE										0x0015
static const mfgV2XTokenInfo MFG_V2X_TINFO_WHILE				= { MFG_V2X_TOKEN_WHILE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"while" };
#define MFG_V2X_TOKEN_FOR										0x0016
static const mfgV2XTokenInfo MFG_V2X_TINFO_FOR					= { MFG_V2X_TOKEN_FOR, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"for" };
#define MFG_V2X_TOKEN_RETURN									0x0017
static const mfgV2XTokenInfo MFG_V2X_TINFO_RETURN				= { MFG_V2X_TOKEN_RETURN, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"return" };
#define MFG_V2X_TOKEN_DISCARD									0x0018
static const mfgV2XTokenInfo MFG_V2X_TINFO_DISCARD				= { MFG_V2X_TOKEN_DISCARD, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"discard" };
#define MFG_V2X_TOKEN_TRUE										0x0019
static const mfgV2XTokenInfo MFG_V2X_TINFO_TRUE					= { MFG_V2X_TOKEN_TRUE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"true" };
#define MFG_V2X_TOKEN_FALSE										0x001A
static const mfgV2XTokenInfo MFG_V2X_TINFO_FALSE				= { MFG_V2X_TOKEN_FALSE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"false" };
#define MFG_V2X_TOKEN_INPUT										0x001B
static const mfgV2XTokenInfo MFG_V2X_TINFO_INPUT				= { MFG_V2X_TOKEN_INPUT, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"Input" };
#define MFG_V2X_TOKEN_OUTPUT									0x001C
static const mfgV2XTokenInfo MFG_V2X_TINFO_OUTPUT				= { MFG_V2X_TOKEN_OUTPUT, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"Output" };
#define MFG_V2X_TOKEN_CONSTANT_BUFFER							0x001D
static const mfgV2XTokenInfo MFG_V2X_TINFO_CONSTANT_BUFFER		= { MFG_V2X_TOKEN_CONSTANT_BUFFER, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"ConstantBuffer" };
#define MFG_V2X_TOKEN_TEXTURE_1D								0x001E
static const mfgV2XTokenInfo MFG_V2X_TINFO_TEXTURE_1D			= { MFG_V2X_TOKEN_TEXTURE_1D, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"Texture1D" };
#define MFG_V2X_TOKEN_TEXTURE_2D								0x001F
static const mfgV2XTokenInfo MFG_V2X_TINFO_TEXTURE_2D			= { MFG_V2X_TOKEN_TEXTURE_2D, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"Texture2D" };
#define MFG_V2X_TOKEN_TEXTURE_3D								0x0020
static const mfgV2XTokenInfo MFG_V2X_TINFO_TEXTURE_3D			= { MFG_V2X_TOKEN_TEXTURE_3D, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"Texture3D" };

#define MFG_V2X_TOKEN_ADD										0x0030
static const mfgV2XTokenInfo MFG_V2X_TINFO_ADD					= { MFG_V2X_TOKEN_ADD, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"add" };
#define MFG_V2X_TOKEN_SUBTRACT									0x0031
static const mfgV2XTokenInfo MFG_V2X_TINFO_SUBTRACT				= { MFG_V2X_TOKEN_SUBTRACT, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"subtract" };
#define MFG_V2X_TOKEN_MULTIPLY									0x0032
static const mfgV2XTokenInfo MFG_V2X_TINFO_MULTIPLY				= { MFG_V2X_TOKEN_MULTIPLY, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"multiply" };
#define MFG_V2X_TOKEN_DIVIDE									0x0033
static const mfgV2XTokenInfo MFG_V2X_TINFO_DIVIDE				= { MFG_V2X_TOKEN_DIVIDE, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"divide" };
#define MFG_V2X_TOKEN_EQUAL										0x0034
static const mfgV2XTokenInfo MFG_V2X_TINFO_EQUAL				= { MFG_V2X_TOKEN_EQUAL, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"equal" };
#define MFG_V2X_TOKEN_DIFFERENT									0x0035
static const mfgV2XTokenInfo MFG_V2X_TINFO_DIFFERENT			= { MFG_V2X_TOKEN_DIFFERENT, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"different" };
#define MFG_V2X_TOKEN_GREATER									0x0036
static const mfgV2XTokenInfo MFG_V2X_TINFO_GREATER				= { MFG_V2X_TOKEN_GREATER, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"greater" };
#define MFG_V2X_TOKEN_LESS										0x0037
static const mfgV2XTokenInfo MFG_V2X_TINFO_LESS					= { MFG_V2X_TOKEN_LESS, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"less" };
#define MFG_V2X_TOKEN_GEQUAL									0x0038
static const mfgV2XTokenInfo MFG_V2X_TINFO_GEQUAL				= { MFG_V2X_TOKEN_GEQUAL, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"gequal" };
#define MFG_V2X_TOKEN_LEQUAL									0x0039
static const mfgV2XTokenInfo MFG_V2X_TINFO_LEQUAL				= { MFG_V2X_TOKEN_LEQUAL, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"lequal" };
#define MFG_V2X_TOKEN_AND										0x003A
static const mfgV2XTokenInfo MFG_V2X_TINFO_AND					= { MFG_V2X_TOKEN_AND, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"and" };
#define MFG_V2X_TOKEN_OR										0x003B
static const mfgV2XTokenInfo MFG_V2X_TINFO_OR					= { MFG_V2X_TOKEN_OR, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"or" };
#define MFG_V2X_TOKEN_NOT										0x003C
static const mfgV2XTokenInfo MFG_V2X_TINFO_NOT					= { MFG_V2X_TOKEN_NOT, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"not" };
#define MFG_V2X_TOKEN_ASSIGN									0x003D
static const mfgV2XTokenInfo MFG_V2X_TINFO_ASSIGN				= { MFG_V2X_TOKEN_ASSIGN, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"assign" };
#define MFG_V2X_TOKEN_MEMBER									0x003E
static const mfgV2XTokenInfo MFG_V2X_TINFO_MEMBER				= { MFG_V2X_TOKEN_MEMBER, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"member" };

#define MFG_V2X_TOKEN_OPEN_BRACES								0x0050
static const mfgV2XTokenInfo MFG_V2X_TINFO_OPEN_BRACES			= { MFG_V2X_TOKEN_OPEN_BRACES, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"open-braces" };
#define MFG_V2X_TOKEN_CLOSE_BRACES								0x0051
static const mfgV2XTokenInfo MFG_V2X_TINFO_CLOSE_BRACES			= { MFG_V2X_TOKEN_CLOSE_BRACES, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"close-braces" };
#define MFG_V2X_TOKEN_OPEN_BRACKETS								0x0052
static const mfgV2XTokenInfo MFG_V2X_TINFO_OPEN_BRACKETS		= { MFG_V2X_TOKEN_OPEN_BRACKETS, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"open-brackets" };
#define MFG_V2X_TOKEN_CLOSE_BRACKETS							0x0053
static const mfgV2XTokenInfo MFG_V2X_TINFO_CLOSE_BRACKETS		= { MFG_V2X_TOKEN_CLOSE_BRACKETS, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"close-brackets" };
#define MFG_V2X_TOKEN_OPEN_PARENTHESIS							0x0054
static const mfgV2XTokenInfo MFG_V2X_TINFO_OPEN_PARENTHESIS		= { MFG_V2X_TOKEN_OPEN_PARENTHESIS, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"open-parenthesis" };
#define MFG_V2X_TOKEN_CLOSE_PARENTHESIS							0x0055
static const mfgV2XTokenInfo MFG_V2X_TINFO_CLOSE_PARENTHESIS	= { MFG_V2X_TOKEN_CLOSE_PARENTHESIS, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"close-parenthesis" };
#define MFG_V2X_TOKEN_SEMICOLON									0x0056
static const mfgV2XTokenInfo MFG_V2X_TINFO_SEMICOLON			= { MFG_V2X_TOKEN_SEMICOLON, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"semicolon" };
#define MFG_V2X_TOKEN_COLON										0x0057
static const mfgV2XTokenInfo MFG_V2X_TINFO_COLON				= { MFG_V2X_TOKEN_COLON, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"colon" };
#define MFG_V2X_TOKEN_COMMA										0x0058
static const mfgV2XTokenInfo MFG_V2X_TINFO_COMMA				= { MFG_V2X_TOKEN_COMMA, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"comma" };

	mfError mfgV2XRunMVLLexer(const mfsUTF8CodeUnit* source, mfgV2XToken* tokens, mfmU64 maxTokenCount, mfgV2XLexerState* state);

#ifdef __cplusplus
}
#endif
