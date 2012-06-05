#include "stdafx.h"
#include "UGStrOp.h"

#include <stdio.h>
#include <stdarg.h>

void UGStr::tcscpy(TCHAR * dest, SIZE_T length, const TCHAR* src)
{
#if _MSC_VER >= 1400
	::_tcscpy_s(dest, length, src);
#else
	UNREFERENCED_PARAMETER(length);
	::_tcscpy(dest,src);
# endif
}

void UGStr::tcsncpy(TCHAR * dest, size_t dstSize, const TCHAR * src, size_t maxCount)
{
#if _MSC_VER >= 1400
	_tcsncpy_s(dest, dstSize, src, maxCount);
#else
	UNREFERENCED_PARAMETER(dstSize);
#ifdef _UNICODE
	::wcsncpy(dest,src, maxCount);
#else
	::strncpy(dest,src, maxCount);
#endif
#endif
}

char* UGStr::fcvt(double val, int count, int * dec, int * sign)
{
#if _MSC_VER >= 1400
	UNREFERENCED_PARAMETER(sign);
	char * dest = new char[count * 2 + 10];
	::_fcvt_s(dest, count * 2 + 9, val, count, dec, sign);
	return dest;
#else
	return ::_fcvt(val, count, dec, sign);
# endif		
}

void UGStr::strncpy(char * dest, size_t size, const char * src, size_t count)
{
#if _MSC_VER >= 1400
	::strncpy_s(dest, size, src, count);
#else
	UNREFERENCED_PARAMETER(size);
	::strncpy(dest, src, count);
# endif
}

void UGStr::sprintf(char * dest, size_t size, const char * src, ...)
{
	va_list vl;
	va_start( vl, src );

#if _MSC_VER >= 1400
    _vsprintf_s_l(dest, size, src, NULL, vl);
#else
	UNREFERENCED_PARAMETER(size);
	vsprintf(dest, src, vl);
# endif

	va_end( vl );
}


void UGStr::stprintf(TCHAR * dest, size_t size, const TCHAR * src, ...)
{
	va_list vl;
	va_start( vl, src );

#ifdef _UNICODE
#if _MSC_VER >= 1400
	_vswprintf_s_l(dest, size, src, NULL, vl);
#else
	UNREFERENCED_PARAMETER(size);
	_vstprintf(dest, src, vl);
# endif
#else
#if _MSC_VER >= 1400
    _vsprintf_s_l(dest, size, src, NULL, vl);
#else
	UNREFERENCED_PARAMETER(size);
	_vstprintf(dest, src, vl);
# endif
#endif

	va_end( vl );
}

void UGStr::tcscat(TCHAR * dest, SIZE_T length, const TCHAR* src)
{
#if _MSC_VER >= 1400
	_tcscat_s(dest, length, src);
#else
	UNREFERENCED_PARAMETER(length);
	_tcscat(dest, src);
# endif
}

void UGStr::tcsncat(TCHAR * dest, size_t size, const TCHAR * src, size_t count)
{
#if _MSC_VER >= 1400
	_tcsncat_s(dest, size, src, count);
#else
	UNREFERENCED_PARAMETER(size);
	_tcsncat(dest, src, count);
# endif
}

TCHAR* UGStr::tcstok(TCHAR* strToken, const TCHAR* strDelimit, TCHAR ** context)
{
#if _MSC_VER >= 1400
	return _tcstok_s(strToken, strDelimit, context);
#else
	UNREFERENCED_PARAMETER(context);
	return _tcstok(strToken, strDelimit);
#endif
}

size_t UGStr::mbstowcs(wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count )
{
#if _MSC_VER >= 1400
	size_t retval;

	if (!mbstowcs_s(&retval, wcstr, sizeInWords, mbstr, count))
		return retval;

	return 0;
#else
	UNREFERENCED_PARAMETER(sizeInWords);
	return ::mbstowcs(wcstr, mbstr, count);
#endif
}

void UGStr::wcstombs(size_t *pConvertedChars, char *mbstr, 
	size_t sizeInBytes, const wchar_t *wcstr, size_t count)
{
#if _MSC_VER >= 1400
	wcstombs_s(pConvertedChars, mbstr, sizeInBytes, wcstr, count);
#else
	UNREFERENCED_PARAMETER(sizeInBytes);
	*pConvertedChars = ::wcstombs(mbstr, wcstr, count);
#endif
}
