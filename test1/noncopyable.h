#pragma once

#include "CommonType.h"

namespace Base
{

	class __BASE_COMMON_API__ noncopyable
	{
	protected:
		noncopyable() {}

	private:
		noncopyable( const noncopyable& );
		const noncopyable& operator = ( const noncopyable& );
	};

}