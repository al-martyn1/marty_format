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

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StringType> inline StringType martyFormatSimpleConvertToString(bool b)                  { return StringType(b ? "true" : "false" ); }
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
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const marty::Decimal &d) { return martyFormatSimpleConvertToString<StringType>(to_string(d).c_str()); }
template<typename StringType> inline StringType martyFormatSimpleConvertToString(const void* ptr)         { return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned long long)ptr).c_str()); }


template<typename StringType> inline StringType martyFormatSimpleConvertToString(FormatValueFilter f)
{ 
    return martyFormatSimpleConvertToString<StringType>("filter: ") + martyFormatSimpleConvertToString<StringType>(typeid(f).name());
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormatString(const FormattingOptions &formattingOptions, const std::string &str)
{
    // MARTY_ARG_USED(formattingOptions);

    auto optionsFlags = formattingOptions.optionsFlags;

    auto typeChar = formattingOptions.typeChar;
    if (!typeChar)
        typeChar = 's';

    if (typeChar!='s' && typeChar!='?')
    {
        if ((formattingOptions.formattingFlags&FormattingFlags::ignoreTypeMismatchErrors)!=0)
            throw mismatch_format_type("invalid format type for string argument");
    }

    auto alignmentChar = formattingOptions.alignment;
    if (alignmentChar==0 || alignmentChar=='=')
        alignmentChar = '<';

    // Для строк formattingOptions.convertChar игнорируется

    // std::string str = str;
    // TODO: !!! Если задано сделать escape для строки
    // то надо сделать escape, но не длиннее, чем макс ширина


    std::string strLimited;

    if ((optionsFlags&FormattingOptionsFlags::precisionTaken)!=0)
    {
        // Если задана "точность", а для строк - это максимальная длина строки, то
        // из исходной строки копируем посимвольно с проверкой размера символа до тех
        // пор, пока не достигли лимита

        // TODO: !!! Тут используем UTF итераторы явно, если нам не нужен UTF, надо 
        // будет обрубить эту возможность

        strLimited.reserve(std::size_t(formattingOptions.precision));

        std::size_t strLimitedLen = 0;

        auto b = marty::utf::UtfInputIterator<char>(str.data(), str.data()+std::ptrdiff_t(str.size()));
        auto e = marty::utf::UtfInputIterator<char>(str.data()+std::ptrdiff_t(str.size()));
        auto outIt = marty::utf::UtfOutputIterator<char>(strLimited);

        for(; b!=e && strLimitedLen<std::size_t(formattingOptions.precision); ++b)
        {
            auto ch = *b;
            strLimitedLen += WidthCalculator()(ch);
            *outIt++ = ch;
        }

    }
    else
    {
        strLimited = str;
    }

    if ((optionsFlags&FormattingOptionsFlags::fieldWidthTaken)==0)
    {
        // Ширина не задана, возвращаем то, что есть
        return strLimited;
    }

    auto fillChar = formattingOptions.fillChar;
    if (fillChar<32 || WidthCalculator()(fillChar)<1)
        fillChar = utf32_char_t(' ');


    auto strWidth = WidthCalculator()(strLimited.data(), strLimited.data()+std::ptrdiff_t(strLimited.size()));
    auto fmtWidth = std::size_t(formattingOptions.width);
    std::size_t widthRest = (strWidth < fmtWidth) ? (fmtWidth-strWidth) : std::size_t(0u);

    std::size_t widthLeft = 0;

    if (alignmentChar=='^')
    {
        widthLeft = widthRest/2u;
    }
    else if (alignmentChar=='>')
    {
        widthLeft = widthRest; // strWidth - widthRest;
    }

    auto strRes = strLimited; strRes.clear(); // вместо явного типа и reserve
    auto outIt = marty::utf::UtfOutputIterator<char>(strRes);
    std::size_t resWidth = 0;

    for(; resWidth<widthLeft; ++resWidth)
        *outIt++ = fillChar; // заполнение слева, fillChar у нас не нулевой ширины, ранее было вычислено, поэтому resWidth инкрементим безусловно

    strRes.append(strLimited); resWidth += strWidth;
    
    for(; resWidth<fmtWidth; ++resWidth)
        *outIt++ = fillChar; // заполнение справа, fillChar у нас не нулевой ширины, ранее было вычислено, поэтому resWidth инкрементим безусловно

    return strRes;
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatInt(const FormattingOptions &formattingOptions, IntType v, size_t valSize)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(valSize);
    return martyFormatSimpleConvertToString<StringType>(v);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatUnsigned(const FormattingOptions &formattingOptions, IntType v, size_t valSize)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(valSize);
    return martyFormatSimpleConvertToString<StringType>(v);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename FloatType >
StringType martyFormatValueFormatFloat(const FormattingOptions &formattingOptions, FloatType v)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(v);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, bool b)
{

// The available bool presentation types are:
//  
// none, s: Copies textual representation (true or false, or the locale-specific form) to the output.
// b, B, d, o, x, X: Uses integer presentation types with the value static_cast<unsigned char>(value).
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(b);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, char ch)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(ch);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const void* ptr)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(ptr);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const marty::Decimal &d)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(d);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, signed char  v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, signed short v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, int          v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, long         v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, long long    v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }

template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned char      v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned short     v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned int       v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned long      v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned long long v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, float v)       { return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, v); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, double v)      { return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, v); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, long double v) { return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, v); }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const std::string &str)  { return martyFormatValueFormatString<WidthCalculator, StringType>(formattingOptions, str); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const char* str)         { return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, str ? std::string(str) : std::string()); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const std::wstring &str) { return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, marty::utf::string_from_wstring(str)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const wchar_t* str)      { return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, str ? std::wstring(str) : std::wstring()); }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, FormatValueFilter filter)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(filter);
    throw filter_as_value_error("invalid filter usage: filter used as value");
}

//----------------------------------------------------------------------------

