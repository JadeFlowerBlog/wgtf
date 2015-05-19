#include "logging.hpp"

#include <cstdarg>
#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


namespace NGT
{


int logMessage( const char* format, ... )
{
	const size_t bufferSize = 4095;
	char buffer[ bufferSize ];

	va_list args;
	va_start( args, format );

	const int result = vsnprintf( buffer,
		sizeof( buffer ) - 1,
		format,
		args );
	buffer[ sizeof( buffer ) - 1 ] = '\0';

	va_end( args );

	OutputDebugStringA( buffer );

	return result;
}


} // namespace NGT

