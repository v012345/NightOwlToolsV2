#ifndef _PATCHER_EXCEPTION_H_
#define _PATCHER_EXCEPTION_H_

#include <stdexcept>
#include "def.h"

namespace AonePatcher
{
	enum class PatchErrorCode : uint32
	{
		NoError = 0,
		Common,
		Network,
		MD5,
		ZLIB,
		Storage,
		Cancel,
		InvalidArg
	};

	class PatcherException : public std::runtime_error
	{
	public:
		PatcherException(PatchErrorCode code, uint32 innerCode, const std::string msg) : std::runtime_error(msg.c_str())
		{ 
			m_code = code;
			m_innerCode = innerCode;
		}
		PatchErrorCode m_code;
		uint32 m_innerCode;

	};
}

#endif // !_PATCHER_EXCEPTION_H_
