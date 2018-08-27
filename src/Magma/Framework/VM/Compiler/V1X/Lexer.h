#pragma once

#include "../../Error.h"
#include "../../../String/UTF8.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef mfmU32 mfvV1XEnum;

#define MFV_TOKEN_ATTRIBUTE_SIZE 32
#define MFV_MAX_ERROR_MESSAGE_SIZE 512

	typedef struct
	{
		mfvV1XEnum type;
		mfmBool isType;
		mfmBool isOperator;
		mfmBool isPunctuation;
		const mfsUTF8CodeUnit* name;
	} mfvV1XTokenInfo;

	typedef struct
	{
		mfvV1XTokenInfo* info;
		mfsUTF8CodeUnit attribute[MFV_TOKEN_ATTRIBUTE_SIZE];
	} mfvV1XToken;

	typedef struct
	{
		mfsUTF8CodeUnit errorMsg[MFV_MAX_ERROR_MESSAGE_SIZE];
		mfmU64 tokenCount;
	} mfvV1XLexerState;

	
#define MFV_V1X_TOKEN_VOID										0x0000
static const mfvV1XTokenInfo MFV_V1X_TINFO_VOID					= { MFV_V1X_TOKEN_VOID, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"void" };
#define MFV_V1X_TOKEN_I8										0x0001
static const mfvV1XTokenInfo MFV_V1X_TINFO_I8					= { MFV_V1X_TOKEN_I8, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"i8" };
#define MFV_V1X_TOKEN_I16										0x0002
static const mfvV1XTokenInfo MFV_V1X_TINFO_I16					= { MFV_V1X_TOKEN_I16, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"i16" };
#define MFV_V1X_TOKEN_I32										0x0003
static const mfvV1XTokenInfo MFV_V1X_TINFO_I32					= { MFV_V1X_TOKEN_I32, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"i32" };
#define MFV_V1X_TOKEN_U8										0x0004
static const mfvV1XTokenInfo MFV_V1X_TINFO_U8					= { MFV_V1X_TOKEN_U8, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"u8" };
#define MFV_V1X_TOKEN_U16										0x0005
static const mfvV1XTokenInfo MFV_V1X_TINFO_U16					= { MFV_V1X_TOKEN_U16, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"u16" };
#define MFV_V1X_TOKEN_U32										0x0006
static const mfvV1XTokenInfo MFV_V1X_TINFO_U32					= { MFV_V1X_TOKEN_U32, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"u32" };
#define MFV_V1X_TOKEN_F32										0x0007
static const mfvV1XTokenInfo MFV_V1X_TINFO_F32					= { MFV_V1X_TOKEN_F32, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"f32" };
#define MFV_V1X_TOKEN_BOOL										0x0008
static const mfvV1XTokenInfo MFV_V1X_TINFO_BOOL					= { MFV_V1X_TOKEN_BOOL, MFM_TRUE, MFM_FALSE, MFM_FALSE, u8"bool" };

#define MFV_V1X_TOKEN_INT_LITERAL								0x0010
static const mfvV1XTokenInfo MFV_V1X_TINFO_INT_LITERAL			= { MFV_V1X_TOKEN_INT_LITERAL, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"int-literal" };
#define MFV_V1X_TOKEN_FLOAT_LITERAL								0x0011
static const mfvV1XTokenInfo MFV_V1X_TINFO_FLOAT_LITERAL		= { MFV_V1X_TOKEN_FLOAT_LITERAL, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"float-literal" };
#define MFV_V1X_TOKEN_IDENTIFIER								0x0012
static const mfvV1XTokenInfo MFV_V1X_TINFO_IDENTIFIER			= { MFV_V1X_TOKEN_IDENTIFIER, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"identifier" };
#define MFV_V1X_TOKEN_IF										0x0013
static const mfvV1XTokenInfo MFV_V1X_TINFO_IF					= { MFV_V1X_TOKEN_IF, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"if" };
#define MFV_V1X_TOKEN_ELSE										0x0014
static const mfvV1XTokenInfo MFV_V1X_TINFO_ELSE					= { MFV_V1X_TOKEN_ELSE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"else" };
#define MFV_V1X_TOKEN_WHILE										0x0015
static const mfvV1XTokenInfo MFV_V1X_TINFO_WHILE				= { MFV_V1X_TOKEN_WHILE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"while" };
#define MFV_V1X_TOKEN_FOR										0x0016
static const mfvV1XTokenInfo MFV_V1X_TINFO_FOR					= { MFV_V1X_TOKEN_FOR, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"for" };
#define MFV_V1X_TOKEN_RETURN									0x0017
static const mfvV1XTokenInfo MFV_V1X_TINFO_RETURN				= { MFV_V1X_TOKEN_RETURN, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"return" };
#define MFV_V1X_TOKEN_END										0x0018
static const mfvV1XTokenInfo MFV_V1X_TINFO_END					= { MFV_V1X_TOKEN_END, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"end" };
#define MFV_V1X_TOKEN_THROW										0x0019
static const mfvV1XTokenInfo MFV_V1X_TINFO_THROW				= { MFV_V1X_TOKEN_THROW, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"throw" };
#define MFV_V1X_TOKEN_TRUE										0x001A
static const mfvV1XTokenInfo MFV_V1X_TINFO_TRUE					= { MFV_V1X_TOKEN_TRUE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"true" };
#define MFV_V1X_TOKEN_FALSE										0x001B
static const mfvV1XTokenInfo MFV_V1X_TINFO_FALSE				= { MFV_V1X_TOKEN_FALSE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"false" };
#define MFV_V1X_TOKEN_STRING_LITERAL							0x001C
static const mfvV1XTokenInfo MFV_V1X_TINFO_STRING_LITERAL		= { MFV_V1X_TOKEN_STRING_LITERAL, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"string-literal" };
#define MFV_V1X_TOKEN_WARNING									0x001D
static const mfvV1XTokenInfo MFV_V1X_TINFO_WARNING				= { MFV_V1X_TOKEN_WARNING, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"warning" };
#define MFV_V1X_TOKEN_ERROR										0x001E
static const mfvV1XTokenInfo MFV_V1X_TINFO_ERROR				= { MFV_V1X_TOKEN_ERROR, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"error" };
#define MFV_V1X_TOKEN_YIELD										0x001F
static const mfvV1XTokenInfo MFV_V1X_TINFO_YIELD				= { MFV_V1X_TOKEN_YIELD, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"yield" };

#define MFV_V1X_TOKEN_ADD										0x0030
static const mfvV1XTokenInfo MFV_V1X_TINFO_ADD					= { MFV_V1X_TOKEN_ADD, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"add" };
#define MFV_V1X_TOKEN_SUBTRACT									0x0031
static const mfvV1XTokenInfo MFV_V1X_TINFO_SUBTRACT				= { MFV_V1X_TOKEN_SUBTRACT, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"subtract" };
#define MFV_V1X_TOKEN_MULTIPLY									0x0032
static const mfvV1XTokenInfo MFV_V1X_TINFO_MULTIPLY				= { MFV_V1X_TOKEN_MULTIPLY, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"multiply" };
#define MFV_V1X_TOKEN_DIVIDE									0x0033
static const mfvV1XTokenInfo MFV_V1X_TINFO_DIVIDE				= { MFV_V1X_TOKEN_DIVIDE, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"divide" };
#define MFV_V1X_TOKEN_EQUAL										0x0034
static const mfvV1XTokenInfo MFV_V1X_TINFO_EQUAL				= { MFV_V1X_TOKEN_EQUAL, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"equal" };
#define MFV_V1X_TOKEN_DIFFERENT									0x0035
static const mfvV1XTokenInfo MFV_V1X_TINFO_DIFFERENT			= { MFV_V1X_TOKEN_DIFFERENT, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"different" };
#define MFV_V1X_TOKEN_GREATER									0x0036
static const mfvV1XTokenInfo MFV_V1X_TINFO_GREATER				= { MFV_V1X_TOKEN_GREATER, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"greater" };
#define MFV_V1X_TOKEN_LESS										0x0037
static const mfvV1XTokenInfo MFV_V1X_TINFO_LESS					= { MFV_V1X_TOKEN_LESS, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"less" };
#define MFV_V1X_TOKEN_GEQUAL									0x0038
static const mfvV1XTokenInfo MFV_V1X_TINFO_GEQUAL				= { MFV_V1X_TOKEN_GEQUAL, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"gequal" };
#define MFV_V1X_TOKEN_LEQUAL									0x0039
static const mfvV1XTokenInfo MFV_V1X_TINFO_LEQUAL				= { MFV_V1X_TOKEN_LEQUAL, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"lequal" };
#define MFV_V1X_TOKEN_AND										0x003A
static const mfvV1XTokenInfo MFV_V1X_TINFO_AND					= { MFV_V1X_TOKEN_AND, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"and" };
#define MFV_V1X_TOKEN_OR										0x003B
static const mfvV1XTokenInfo MFV_V1X_TINFO_OR					= { MFV_V1X_TOKEN_OR, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"or" };
#define MFV_V1X_TOKEN_NOT										0x003C
static const mfvV1XTokenInfo MFV_V1X_TINFO_NOT					= { MFV_V1X_TOKEN_NOT, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"not" };
#define MFV_V1X_TOKEN_ASSIGN									0x003D
static const mfvV1XTokenInfo MFV_V1X_TINFO_ASSIGN				= { MFV_V1X_TOKEN_ASSIGN, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"assign" };
#define MFV_V1X_TOKEN_MEMBER									0x003E
static const mfvV1XTokenInfo MFV_V1X_TINFO_MEMBER				= { MFV_V1X_TOKEN_MEMBER, MFM_FALSE, MFM_TRUE, MFM_FALSE, u8"member" };

#define MFV_V1X_TOKEN_OPEN_BRACES								0x0050
static const mfvV1XTokenInfo MFV_V1X_TINFO_OPEN_BRACES			= { MFV_V1X_TOKEN_OPEN_BRACES, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"open-braces" };
#define MFV_V1X_TOKEN_CLOSE_BRACES								0x0051
static const mfvV1XTokenInfo MFV_V1X_TINFO_CLOSE_BRACES			= { MFV_V1X_TOKEN_CLOSE_BRACES, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"close-braces" };
#define MFV_V1X_TOKEN_OPEN_BRACKETS								0x0052
static const mfvV1XTokenInfo MFV_V1X_TINFO_OPEN_BRACKETS		= { MFV_V1X_TOKEN_OPEN_BRACKETS, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"open-brackets" };
#define MFV_V1X_TOKEN_CLOSE_BRACKETS							0x0053
static const mfvV1XTokenInfo MFV_V1X_TINFO_CLOSE_BRACKETS		= { MFV_V1X_TOKEN_CLOSE_BRACKETS, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"close-brackets" };
#define MFV_V1X_TOKEN_OPEN_PARENTHESIS							0x0054
static const mfvV1XTokenInfo MFV_V1X_TINFO_OPEN_PARENTHESIS		= { MFV_V1X_TOKEN_OPEN_PARENTHESIS, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"open-parenthesis" };
#define MFV_V1X_TOKEN_CLOSE_PARENTHESIS							0x0055
static const mfvV1XTokenInfo MFV_V1X_TINFO_CLOSE_PARENTHESIS	= { MFV_V1X_TOKEN_CLOSE_PARENTHESIS, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"close-parenthesis" };
#define MFV_V1X_TOKEN_SEMICOLON									0x0056
static const mfvV1XTokenInfo MFV_V1X_TINFO_SEMICOLON			= { MFV_V1X_TOKEN_SEMICOLON, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"semicolon" };
#define MFV_V1X_TOKEN_COLON										0x0057
static const mfvV1XTokenInfo MFV_V1X_TINFO_COLON				= { MFV_V1X_TOKEN_COLON, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"colon" };
#define MFV_V1X_TOKEN_COMMA										0x0058
static const mfvV1XTokenInfo MFV_V1X_TINFO_COMMA				= { MFV_V1X_TOKEN_COMMA, MFM_FALSE, MFM_FALSE, MFM_TRUE, u8"comma" };

	mfError mfvV1XRunMVLLexer(const mfsUTF8CodeUnit* source, mfvV1XToken* tokens, mfmU64 maxTokenCount, mfvV1XLexerState* state);

#ifdef __cplusplus
}
#endif
