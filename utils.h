/*!
    \file
    \brief Типы для marty::format
 */
#pragma once

#include "enums.h"
#include "defs.h"

// 
#include "marty_utf/utf.h"
//namespace marty_utf {

#include <string>
#include <cstring>
#include <stdexcept>


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
int toDigit(char ch)
{
    return (ch>='0' && ch<='9')
           ? int(ch-'0')
           : -1
           ;
}

//----------------------------------------------------------------------------
inline
unsigned charRangeToUnsigned(const char *b, const char *e)
{
    unsigned res = 0;

    if (b==e)
        throw std::invalid_argument("charRangeToUnsigned: can't convert empty string");

    for(; b!=e; ++e)
    {
        auto d = toDigit(*b);
        if (d<0)
            throw std::invalid_argument("charRangeToUnsigned: symbol is not a decomal digit");

        auto newRes = res*10;
        newRes += unsigned(d);
        if (newRes<res)
            throw std::out_of_range("charRangeToUnsigned: value is out of range");

        res = newRes;
    }

    return res;
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
    return ch==utf32_char_t('-') || ch==utf32_char_t('+') /*  || ch==utf32_char_t(' ') */ ;
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatConvertMarker(utf32_char_t ch)
{
    return ch==utf32_char_t('!');
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatConvertChar(utf32_char_t ch)
{
    return ch==utf32_char_t('r') || ch==utf32_char_t('s') || ch==utf32_char_t('a');
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
        || isFormatDigit(ch) || isFormatTypeChar(ch)
        || ch==utf32_char_t('{')
        || ch==utf32_char_t('z')
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
inline const char*      ltrim_copy(const char* b, const char* e) { if (!b) return b; return b + std::ptrdiff_t(ltrim_distance(b, e)); }

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
inline const char*      rtrim_copy(const char* b, const char* e) { if (!e) return e; return e - std::ptrdiff_t(rtrim_distance(b, e)); }

//----------------------------------------------------------------------------
inline void             trim      (std::string      &sv)   { ltrim(sv); rtrim(sv); }
inline std::string      trim_copy (std::string       sv)   { return ltrim_copy(rtrim_copy(sv)); }
//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
//! Базовый false-тип для детекта наличия типа first_type у объекта
template< typename C, typename = void >
struct has_first_type : std::false_type {};

//------------------------------
//! Базовый false-тип для детекта наличия типа second_type у объекта
template< typename C, typename = void >
struct has_second_type : std::false_type {};

//----------------------------------------------------------------------------
// See also: https://en.cppreference.com/w/cpp/types/void_t

//----------------------------------------------------------------------------
//! Специализация, тестирующая наличие типа first_type у объекта
template< typename C >
struct has_first_type< C, std::void_t<typename C::first_type> > : std::true_type {};

//------------------------------
//! Специализация, тестирующая наличие типа second_type у объекта
template< typename C >
struct has_second_type< C, std::void_t<typename C::second_type> > : std::true_type {};

//----------------------------------------------------------------------------
template< typename C, typename = void >
struct is_range : std::false_type {};
 
template<typename T>
struct is_range<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> : std::true_type {};

//----------------------------------------------------------------------------
template< typename C, typename = void >
struct has_size : std::false_type {};
 
template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

//----------------------------------------------------------------------------
template< typename C, typename = void >
struct has_end : std::false_type {};
 
template<typename T>
struct has_end<T, std::void_t<decltype(std::declval<T>().end())>> : std::true_type {};

//----------------------------------------------------------------------------
template< typename C, typename = void >
struct has_string_find : std::false_type {};
 
template<typename T>
struct has_string_find<T, std::void_t<decltype(std::declval<T>().find(std::string()))>> : std::true_type {};

//----------------------------------------------------------------------------
template< typename C, typename = void >
struct has_char_ptr_find : std::false_type {};
 
template<typename T>
struct has_char_ptr_find<T, std::void_t<decltype(std::declval<T>().find((const char*)0, (const char*)0))>> : std::true_type {};

//----------------------------------------------------------------------------
template< typename C, typename = void >
struct has_size_t_find : std::false_type {};
 
template<typename T>
struct has_size_t_find<T, std::void_t<decltype(std::declval<T>().find(std::size_t(0)))>> : std::true_type {};

//----------------------------------------------------------------------------
template< typename C, typename = void >
struct has_find_by_pos : std::false_type {};
 
template<typename T>
struct has_find_by_pos<T, std::void_t<decltype(std::declval<T>().find_by_pos(std::size_t(0)))>> : std::true_type {};

//----------------------------------------------------------------------------
template< typename C, typename = void >
struct has_operator_string_index : std::false_type {};
 
template<typename T>
struct has_operator_string_index<T, std::void_t<decltype(std::declval<T>().operator[](std::string()))>> : std::true_type {};

//----------------------------------------------------------------------------
template<typename T> using is_bool = std::is_same<std::decay_t<T>, bool>;
template<typename T> using is_char = std::is_same<std::decay_t<T>, char>;

//----------------------------------------------------------------------------
//! Базовый false-тип для детекта наличия типа const_iterator у объекта
template< typename C, typename = void >
struct has_const_iterator : std::false_type {};

//! Специализация, тестирующая наличие типа const_iterator у объекта
template< typename C >
struct has_const_iterator< C, std::void_t<typename C::const_iterator> > : std::true_type {};


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Подсчитывает длину строки в символах
//#! DefaultUtfWidthCalculator
struct DefaultUtfWidthCalculator
{
    std::size_t operator()(const char* b, const char* e) const
    {
        auto it    = marty::utf::UtfInputIterator<char>(b, e);
        auto endIt = marty::utf::UtfInputIterator<char>();

        std::size_t size = 0;

        for(; it!=endIt; ++it)
        {
            auto ch = *it;
            // suf - simpleUnicodeFeature
            if (marty::utf::sufIsZeroWidthSpace(ch) || marty::utf::sufIsCombiningDiacretic(ch))
                continue;

            ++size;
        }

        return size;
    }

}; // struct DefaultUtfWidthCalculator
//#!

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename, typename = void>
struct IsDereferenceableToLvalue_ : std::false_type {};

template<typename T>
struct IsDereferenceableToLvalue_<T, std::void_t<decltype(&*std::declval<T>())>> : std::true_type {};

template<typename T>
inline constexpr bool IsDereferenceableToLvalue = IsDereferenceableToLvalue_<T>::value;

template<typename T>
inline constexpr bool IsPointer = std::is_pointer_v<std::decay_t<T>>;

template<typename CharPointerType>
const char* rawConstCharPtrFromIterator(CharPointerType* pstr)
{
   return reinterpret_cast<const char*>(pstr);
}

template <typename IteratorType, std::enable_if_t<
    IsDereferenceableToLvalue<IteratorType>
    and not IsPointer<IteratorType>
, int> = 0>
const char* rawConstCharPtrFromIterator(IteratorType it)
{
   return reinterpret_cast<const char*>(&*it);
}

// template<typename CharType>
// const char* rawConstCharPtrFromIterator(marty::utf::UtfInputIterator<CharType> it)
// {
//     return (const char*)it.rawPtr();
// }
//  
// template<typename CharType>
// const char* rawConstCharPtrFromIterator(typename std::basic_string<CharType>::iterator it)
// {
//     return (const char*)(&*it);
// }
//  
// template<typename CharType>
// const char* rawConstCharPtrFromIterator(typename std::basic_string<CharType>::const_iterator it)
// {
//     return (const char*)(&*it);
// }

//----------------------------------------------------------------------------
#if 0
//std::is_pointer<IntType>::value

template<typename CharPointerType>
typename std::enable_if< std::is_pointer<CharPointerType>::value, const char* >::type
rawConstCharPtrFromIterator(CharPointerType pstr)
{
    return (const char*)pstr;
}

//----------------------------------------------------------------------------
template<typename ContainerType>
typename std::enable_if< has_const_iterator<ContainerType>::value, const char* >::type
rawConstCharPtrFromIterator(typename ContainerType::const_iterator it)
{
    return (const char*)(&*it);
}
#endif


//----------------------------------------------------------------------------
// template<typename ContainerType>
// const char* rawConstCharPtrFromIterator(typename ContainerType::iterator it)
// {
//     return (const char*)(&*it);
// }

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace utils
} // namespace format
} // namespace marty
// marty::format::utils::
// #include "marty_format/marty_format.h"

