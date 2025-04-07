/*!
    \file
    \brief Типы для marty::format
 */
#pragma once

#include "enums.h"

// 
#include "marty_utf/utf.h"
//namespace marty_utf {

#include <string>


//----------------------------------------------------------------------------
// #include "marty_format/marty_format.h"
// marty::format::utils::
namespace marty{
namespace format{
namespace utils{

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
using utf32_char_t = marty::utf::utf32_char_t;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
constexpr
int toDigit(utf32_char_t ch)
{
    return (ch>=utf32_char_t('0') && ch<=utf32_char_t('9'))
           ? int(ch-utf32_char_t('0'))
           : -1
           ;
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatAlignMarker(utf32_char_t ch)
{
    return ch==utf32_char_t('<') || ch==utf32_char_t('>') || ch==utf32_char_t('^') || ch==utf32_char_t('=');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatSignMarker(utf32_char_t ch)
{
    return ch==utf32_char_t('-') || ch==utf32_char_t('+');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatAlterChar(utf32_char_t ch)
{
    return ch==utf32_char_t('#');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatOpenBrace(utf32_char_t ch)
{
    return ch==utf32_char_t('{');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatFormatThousandSep(utf32_char_t ch)
{
    return ch==utf32_char_t('_') || ch==utf32_char_t('\'') || ch==utf32_char_t(',');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatCloseBrace(utf32_char_t ch)
{
    return ch==utf32_char_t('}');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatPeriodChar(utf32_char_t ch)
{
    return ch==utf32_char_t('.');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatLocaleChar(utf32_char_t ch)
{
    return ch==utf32_char_t('L');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatDigit(utf32_char_t ch, int minDigit=0, int maxDigit=9)
{
    //constexpr 
    const int d = toDigit(ch);
    return (d<0)
           ? false
           : (d>=minDigit && d<=maxDigit)
             ? true
             : false
           ;
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatDigit(int d, int minDigit=0, int maxDigit=9)
{
    return (d<0)
           ? false
           : (d>=minDigit && d<=maxDigit)
             ? true
             : false
           ;
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatTypeChar(utf32_char_t ch)
{
    return ch==utf32_char_t('?') || ch==utf32_char_t('s') || ch==utf32_char_t('b') || ch==utf32_char_t('B') || ch==utf32_char_t('c')
        || ch==utf32_char_t('d') || ch==utf32_char_t('o') || ch==utf32_char_t('x') || ch==utf32_char_t('X') || ch==utf32_char_t('a')
        || ch==utf32_char_t('A') || ch==utf32_char_t('e') || ch==utf32_char_t('E') || ch==utf32_char_t('f') || ch==utf32_char_t('F')
        || ch==utf32_char_t('g') || ch==utf32_char_t('G') || ch==utf32_char_t('n') || ch==utf32_char_t('%');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatAnySpecialChar(utf32_char_t ch)
{
    return isFormatAlignMarker(ch) || isFormatSignMarker(ch) || isFormatAlterChar(ch) 
        || isFormatFormatThousandSep(ch) || isFormatPeriodChar(ch) || isFormatLocaleChar(ch)
        || isFormatDigit(ch) || isFormatTypeChar(ch) || ch==utf32_char_t('{')
           ;
}

//----------------------------------------------------------------------------
inline
char tolower(char ch)
{
    if (ch>='A' && ch<='Z')
        ch = ch - 'A' + 'a';
    return ch;
}

//----------------------------------------------------------------------------
inline
void tolower(std::string &s)
{
    for(auto &ch : s)
    {
        ch = tolower(ch);
    }
}

//----------------------------------------------------------------------------
inline
std::string tolower_copy(std::string s)
{
    for(auto &ch : s)
    {
        ch = tolower(ch);
    }

    return s;
}

//----------------------------------------------------------------------------
inline std::string removePrefix(std::string &str, std::size_t n)
{
    // basic_string& erase( size_type index = 0, size_type count = npos );
    // https://en.cppreference.com/w/cpp/string/basic_string/erase

    if (n>str.size())
        n = str.size(); // Для строк - не задаём вопросов

    str.erase(0, n);

    return str;
}

//----------------------------------------------------------------------------
inline std::string removeSuffix(std::string &str, std::size_t n)
{
    // basic_string& erase( size_type index = 0, size_type count = npos );
    // https://en.cppreference.com/w/cpp/string/basic_string/erase

    if (n>str.size())
        n = str.size(); // Для строк - не задаём вопросов

    str.erase(str.size()-n /* , n */ ); // Удаляем до конца

    return str;
}

//----------------------------------------------------------------------------
template<typename IteratorType>
std::size_t ltrim_distance(IteratorType b, IteratorType e)
{
    std::size_t idx = 0;
    for(; b!=e; ++b, ++idx)
    {
        if (!(*b==' ' || *b=='\t' || *b=='\r' || *b=='\n'))
            break;
    }
    return idx;
}

//----------------------------------------------------------------------------
template<typename SomethingStringLike>
std::size_t ltrim_distance(const SomethingStringLike &str) { return ltrim_distance(str.begin(), str.end()); }

//----------------------------------------------------------------------------
inline void             ltrim     (std::string      &sv)   { removePrefix(sv, ltrim_distance(sv.begin(), sv.end())); }
inline std::string      ltrim_copy(std::string       sv)   { ltrim(sv); return sv; }

//----------------------------------------------------------------------------
template<typename IteratorType>
std::size_t rtrim_distance(IteratorType b, IteratorType e)
{
    // IteratorType bcp = b;
    std::size_t idx = 0;
    //for(; e!=b; --e, ++idx)
    //for(; e--!=b; ++idx)
    for(; e!=b; ++idx)
    {
        e--;
        if (!(*e==' ' || *e=='\t' || *e=='\r' || *e=='\n'))
            return idx;
    }
    //return std::distance(bcp, e);
    return idx;
}

//----------------------------------------------------------------------------
template<typename SomethingStringLike>
std::size_t rtrim_distance(const SomethingStringLike &str) { return rtrim_distance(str.begin(), str.end()); }

inline void             rtrim     (std::string      &sv)   { removeSuffix(sv, rtrim_distance(sv.begin(), sv.end())); }
inline std::string      rtrim_copy(std::string       sv)   { rtrim(sv); return sv; }

//----------------------------------------------------------------------------
inline void             trim      (std::string      &sv)   { ltrim(sv); rtrim(sv); }
inline std::string      trim_copy (std::string       sv)   { return ltrim_copy(rtrim_copy(sv)); }


//----------------------------------------------------------------------------

} // namespace utils
} // namespace format
} // namespace marty
// marty::format::utils::
// #include "marty_format/marty_format.h"

