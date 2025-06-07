#pragma once

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType martyFormatSimpleConvertToString(const char *str)
{
    return str ? StringType(str) : StringType();
}

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType martyFormatSimpleConvertToString(const std::string &str)
{
    return martyFormatSimpleConvertToString<StringType>(str.c_str());
}


template<typename StringType> inline StringType martyFormatSimpleConvertToString(bool b)                  { return StringType(b ? "true" : "false" ); }
//
template<typename StringType> inline StringType martyFormatSimpleConvertToString(char ch)                 { return StringType(1, ch); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned char      i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned)(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(signed char        i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((int     )(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned short     i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned)(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(signed short       i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((int     )(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned int       i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned)(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(int                i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((int     )(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned long      i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned long)(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(long               i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((long         )(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(unsigned long long i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned long long)(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(long long          i)    { return martyFormatSimpleConvertToString<StringType>(std::to_string((long long         )(i)).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(float f)                 { return martyFormatSimpleConvertToString<StringType>(std::to_string(f).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(double d)                { return martyFormatSimpleConvertToString<StringType>(std::to_string(d).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(long double d)           { return martyFormatSimpleConvertToString<StringType>(std::to_string(d).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const std::wstring &str) { return martyFormatSimpleConvertToString<StringType>(marty::utf::string_from_wstring(str).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const wchar_t *str)      { return martyFormatSimpleConvertToString<StringType>(marty::utf::string_from_wstring(str?std::wstring(str):std::wstring()).c_str()); }
#if defined(USE_MARTY_DECIMAL) && USE_MARTY_DECIMAL!=0
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const marty::Decimal &d) { return martyFormatSimpleConvertToString<StringType>(to_string(d).c_str()); }
#endif
#if defined(USE_MARTY_BIGINT) && USE_MARTY_BIGINT!=0
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const marty::BigInt &b)  { return martyFormatSimpleConvertToString<StringType>(to_string(b).c_str()); }
#endif
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const void* ptr)         { return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned long long)ptr).c_str()); }


template<typename StringType> inline StringType martyFormatSimpleConvertToString(FormatValueFilter f)
{ 
    return martyFormatSimpleConvertToString<StringType>("filter: ") + martyFormatSimpleConvertToString<StringType>(typeid(f).name());
}
