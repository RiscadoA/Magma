#include <Magma/Core/Parameters.hpp>
#include <Magma/Core/Exception.hpp>
#include <Magma/Framework/String/StringStream.hpp>

Magma::Core::Parameters::Parameters()
{
	for (mfmU64 i = 0; i < MaxParserCount; ++i)
		m_parsers[i].func = nullptr;
}

Magma::Core::Parameters::~Parameters()
{
	for (mfmU64 i = 0; i < MaxParserCount; ++i)
		m_parsers[i].func = nullptr;
}

void Magma::Core::Parameters::AddParser(void * opaque, const mfsUTF8CodeUnit * param, Framework::Memory::Functor<mfsUTF8CodeUnit**(void* opaque, mfsUTF8CodeUnit** arg)> parser)
{
	for (mfmU64 i = 0; i < MaxParserCount; ++i)
		if (m_parsers[i].func.IsNull())
		{
			Framework::String::StringStream ss(m_parsers[i].name, sizeof(m_parsers[i].name));
			ss.Get() << param;
			m_parsers[i].opaque = opaque;
			m_parsers[i].func = parser;
			return;
		}

	throw Core::Exception(u8"Failed to add parameter parser to Magma::Core::Parameters: all parser slots are filled");
}

void Magma::Core::Parameters::Parse(mfsUTF8CodeUnit** argv)
{
	while (*argv != nullptr)
	{
		mfmU64 i = 0;
		for (; i < MaxParserCount; ++i)
			if (!m_parsers[i].func.IsNull() && strcmp(m_parsers[i].name, *argv) == 0)
			{
				++argv;
				argv = m_parsers[i].func(m_parsers[i].opaque, argv);
				break;
			}

		if (i == MaxParserCount)
		{
			mfsUTF8CodeUnit msg[512];
			Framework::String::StringStream ss(msg, sizeof(msg));
			ss.Get() << u8"Failed to parse parameters on Magma::Core::Parameters: no parser found for parameter '" << *argv << "'";
			throw Core::Exception(msg);
		}
	}
}
