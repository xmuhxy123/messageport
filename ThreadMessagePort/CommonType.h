#pragma once

#if !(defined(WIN32)||defined(WIN64))
#include <errno.h>
#endif

#include <string.h>
#include <limits.h>

#if defined(WIN32)||defined(WIN64)
#pragma warning(disable:4996)
#endif

#pragma warning(error:4244) ///< 把4244号警告信息作为一个错误
#pragma warning(error:4715) ///< 把4715号警告信息作为一个错误

namespace Base
{

#if defined(WIN32)||defined(WIN64)
	typedef unsigned __int8		uint8;
	typedef          __int8		int8;
	typedef unsigned __int16	uint16;
	typedef          __int16	int16;
	typedef unsigned __int32	uint32;
	typedef          __int32	int32;
	typedef unsigned __int64	uint64;
	typedef          __int64	int64;
	typedef unsigned long		_ulong;
#else
	typedef unsigned char		uint8;
	typedef          char		int8;
	typedef unsigned short		uint16;
	typedef          short		int16;
	typedef unsigned int		uint32;
	typedef          int		int32;
	typedef unsigned long long	uint64;
	typedef          long long	int64;
	typedef unsigned long		_ulong;
#endif

#	define INVALID_64BIT 0xffffffffffffffff
#	define INVALID_32BIT 0xffffffff
#	define INVALID_16BIT 0xffff
#	define INVALID_8BIT  0xff

#	define __INVALID_ID 0

//#define INT8_MIN	-128
//#define INT8_MAX	127
//#define UINT8_MAX	0xff
//#define INT16_MIN	-32768
//#define INT16_MAX	32767
//#define UINT16_MAX	0xffff
//#define INT32_MIN	-2147483648
//#define INT32_MAX	2147483647
//#define UINT32_MAX	0xffffffff
//#define INT64_MIN   -9223372036854775808
//#define INT64_MAX	9223372036854775807
//#define UINT64_MAX	0xffffffffffffffff


#define __USE_64_ID__

#ifdef __USE_64_ID__
#	define OBJID uint64
#else
#	define OBJID uint32
#endif

#ifndef NULL
#	define NULL 0
#endif

#ifndef INFINITE
#	define INFINITE 0xFFFFFFFF
#endif

#if defined(WIN32)||defined(WIN64)

#	define INT64FMT		"%I64d"
#	define UINT64FMT	"%I64u"
#	define XINT64FMT	"%I64x"

#ifdef __USE_64_ID__
#	define OBJIDFMT		"%I64u"
#else
#	define OBJIDFMT		"%u"
#endif

#else

#	define INT64FMT		"%lld"
#	define UINT64FMT	"%llu"
#	define XINT64FMT	"%llx"

#ifdef __USE_64_ID__
#	define OBJIDFMT		"%llu"
#else
#	define OBJIDFMT		"%u"
#endif

	inline int32 GetLastError()
	{
		return errno;
	}

#define MAX_PATH PATH_MAX
#endif

#if defined(WIN32)||defined(WIN64)
#	ifdef __BUILD_BASE_COMMON_DLL__
#		define __BASE_COMMON_API__ __declspec(dllexport)
#	else
#		define __BASE_COMMON_API__ __declspec(dllimport)
#	endif
#else
#	define __BASE_COMMON_API__
#endif


}