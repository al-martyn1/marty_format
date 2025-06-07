#pragma once

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType martyFormatSimpleConvertToString(const char *str);

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType martyFormatSimpleConvertToString(const std::string &str);

//----------------------------------------------------------------------------

template<typename StringType> inline StringType martyFormatSimpleConvertToString(bool b)                  ;
//
template<typename StringType> inline StringType martyFormatSimpleConvertToString(char ch)                 ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned char      i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(signed char        i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned short     i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(signed short       i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned int       i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(int                i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned long      i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(long               i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned long long i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(long long          i)    ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(float f)                 ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(double d)                ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(long double d)           ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const std::wstring &str) ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const wchar_t *str)      ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const marty::Decimal &d) ;
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const void* ptr)         ;


template<typename StringType> inline StringType martyFormatSimpleConvertToString(FormatValueFilter f)     ;
