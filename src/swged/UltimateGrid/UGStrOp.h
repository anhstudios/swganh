#include "ugdefine.h"


class UG_CLASS_DECL UGStr
{
public:
	static char* fcvt(double val, int count, int * dec, int * sign);

	static void strncpy(char * dest, size_t size, const char * src, size_t count);

	static void sprintf(char * dest, size_t size, const char * src, ...);

	static void stprintf(TCHAR * dest, size_t size, const TCHAR * src, ...);

	static void tcscat(TCHAR * dest, SIZE_T length, const TCHAR* src);

	static void tcsncat(TCHAR * dest, size_t size, const TCHAR * src, size_t count);

	static void tcscpy(TCHAR * dest, SIZE_T length, const TCHAR* src);

	static void tcsncpy(TCHAR * dest, size_t dstSize, const TCHAR * src, size_t maxCount);

	static TCHAR* tcstok(TCHAR* strToken, const TCHAR* strDelimit, TCHAR ** context);

	static size_t mbstowcs(wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count );

	static void wcstombs(size_t *pConvertedChars, char *mbstr, size_t sizeInBytes, const wchar_t *wcstr, size_t count);

};