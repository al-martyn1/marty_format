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

#include <array>
#include <algorithm>
#include <cstring>
#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <utility>
// #include <system_error>

// GCC ниже 11ой версии не поддерживает std::to_chars
// Поэтому будем использовать fcvt/ecvt/gcvt
// MSVC2019 поддерживает, включая hex, а ниже нам и не надо
// Другие компиляторы старых версий нас не интересуют
#if defined(__GNUC__) && (__GNUC__ < 11)

    #include <sstream>
    #include <stdexcept>
    #include <ios>

#else 

    #include <charconv>

#endif


//
#include "undef_min_max.h"

//----------------------------------------------------------------------------
// #include "marty_format/marty_format.h"
// marty::format::utils::
namespace marty{
namespace format{

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

// Для некоторых шаблонов требуется предварительное объявление
struct FormattingOptions;

//----------------------------------------------------------------------------
// MSVC, по крайней мере, не переваривает некоторые SFINAE-шаблоны, если
// вообще ничего не знает об именах, поэтому подсунем ему фиктивное объявление
// шаблонной функции, определение которой не будем нигде делать

#if 0
template<typename WC, typename ST, typename T>
ST martyFormatValueFormat(FormattingOptions&, const T&);

// template<typename StringType> inline StringType martyFormatSimpleConvertToString(bool b) { return StringType(b ? "true" : "false" ); }
// template<typename ST, typename T>
// ST martyFormatSimpleConvertToString(const T &t);

// template<typename ST, typename T>
// ST martyFormatSimpleConvertToString(const T& t) {
//     // Стандартная реализация
//     if constexpr (std::is_convertible_v<T, ST>) {
//         return ST{t};
//     } else {
//         static_assert(sizeof(T) == 0, "No implementation for this type");
//     }
// }

// template<typename ST, typename T>
// ST martyFormatSimpleConvertToString(const T& t);
//  
// // Явные инстанцирования
// extern template std::string martyFormatSimpleConvertToString<std::string, bool>(const bool&);

struct DummyCustomType {};
struct DummyAnotherType {};

template<typename ST>
ST martyFormatSimpleConvertToString(const DummyCustomType&) {
    return ST{"DummyCustomType"};
}

template<typename ST>
ST martyFormatSimpleConvertToString(DummyAnotherType) {
    return ST{"DummyAnotherType"};
}
#endif

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace utils{

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
using utf32_char_t = marty::utf::utf32_char_t;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::string charToStringUtf8(utf32_char_t ch)
{
    std::string res;
    auto outIt = marty::utf::UtfOutputIterator<char>(res);
    *outIt++ = utf32_char_t(ch);
    return res;
}

//----------------------------------------------------------------------------
inline
std::string charToStringUtf8(char ch)
{
    return std::string(1, ch);
}

//----------------------------------------------------------------------------
inline
unsigned getNumeralSystemBase(NumeralSystem ns)
{
    switch(ns)
    {
        case NumeralSystem::unknown : return 10u;
        case NumeralSystem::none    : return 10u;
        case NumeralSystem::currency: return 10u;
        case NumeralSystem::bin     : return  2u;
        case NumeralSystem::oct     : return  8u;
        case NumeralSystem::dec     : return 10u;
        case NumeralSystem::hex     : return 16u;
        default: return 10u;
    }
}

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
const wchar_t* simpleParseDecimal(const wchar_t* b, const wchar_t* e, unsigned *pu)
{
    unsigned res = 0;

    auto returnB = [&]()
    {
        if (pu)
           *pu = res;
        return b;
    };

    for(; b!=e; ++b)
    {
        auto d = toDigit(utf32_char_t(*b));
        if (d<0)
            return returnB();

        res *= 10u;
        res += unsigned(d);
    }

    return returnB();
}

//----------------------------------------------------------------------------
inline
const char* simpleParseDecimal(const char* b, const char* e, unsigned *pu)
{
    unsigned res = 0;

    auto returnB = [&]()
    {
        if (pu)
           *pu = res;
        return b;
    };

    for(; b!=e; ++b)
    {
        auto d = toDigit(*b);
        if (d<0)
            return returnB();

        res *= 10u;
        res += unsigned(d);
    }

    return returnB();
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace details{

inline
constexpr
char digitToCharDec(int d)
{
    return char('0'+d);
}

inline
constexpr
char digitToCharAlpha(int d, bool bUpper)
{
    return char((bUpper?'A':'a')+d);
}

constexpr
inline
std::size_t calcExpandSize(std::size_t curSize, std::size_t requiredSize)
{
    return curSize>requiredSize ? 0 : requiredSize-curSize;
}

inline
std::string makeStrRepitition(std::size_t n, const std::string &str)
{
    std::string res; res.reserve(str.size()*n);
    for(std::size_t i=0; i!=n; ++i)
        res.append(str);
    return res;
}

} // namespace details

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//! Добавляет size элементов
inline
std::string expandBefore(std::string str, std::size_t size, char chFill)
{
    return std::string(size, chFill) + str;
}

//----------------------------------------------------------------------------
//! Добавляет size элементов
inline
std::string expandAfter(std::string str, std::size_t size, char chFill)
{
    return str + std::string(size, chFill);
}

//----------------------------------------------------------------------------
//! Добавляет size элементов
// strFill - это один символ, неважно, сколько он занимает char'ов
inline
std::string expandBefore(std::string str, std::size_t size, const std::string &strFill)
{
    return utils::details::makeStrRepitition(size, strFill) + str;
}

//----------------------------------------------------------------------------
//! Добавляет size элементов
// strFill - это один символ, неважно, сколько он занимает char'ов
inline
std::string expandAfter(std::string str, std::size_t size, const std::string &strFill)
{
    return str + utils::details::makeStrRepitition(size, strFill);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//! Расширяет до размера size
inline
std::string expandBeforeUpTo(std::string str, std::size_t strSize, std::size_t sizeTo, char chFill)
{
    std::size_t expandSize = details::calcExpandSize(strSize, sizeTo);
    return expandBefore(str, expandSize, chFill);
}

//----------------------------------------------------------------------------
//! Расширяет до размера size
inline
std::string expandAfterUpTo(std::string str, std::size_t strSize, std::size_t sizeTo, char chFill)
{
    std::size_t expandSize = details::calcExpandSize(strSize, sizeTo);
    return expandAfter(str, expandSize, chFill);
}

//----------------------------------------------------------------------------
//! Расширяет до размера size
// strFill - это один символ, неважно, сколько он занимает char'ов
inline
std::string expandBeforeUpTo(std::string str, std::size_t strSize, std::size_t sizeTo, const std::string &strFill)
{
    std::size_t expandSize = details::calcExpandSize(strSize, sizeTo);
    return expandBefore(str, expandSize, strFill);
}

//----------------------------------------------------------------------------
//! Расширяет до размера size
// strFill - это один символ, неважно, сколько он занимает char'ов
inline
std::string expandAfterUpTo(std::string str, std::size_t strSize, std::size_t sizeTo, const std::string &strFill)
{
    std::size_t expandSize = details::calcExpandSize(strSize, sizeTo);
    return expandAfter(str, expandSize, strFill);
}

//----------------------------------------------------------------------------
inline
std::string expandBeforeUpTo(std::string str, std::size_t sizeTo, char chFill)
{
    return expandBeforeUpTo(str, str.size(), sizeTo, chFill);
}

inline
std::string expandAfterUpTo(std::string str, std::size_t sizeTo, char chFill)
{
    return expandAfterUpTo(str, str.size(), sizeTo, chFill);
}

inline
std::string expandBeforeUpTo(std::string str, std::size_t sizeTo, const std::string &strFill)
{
    return expandBeforeUpTo(str, str.size(), sizeTo, strFill);
}

inline
std::string expandAfterUpTo(std::string str, std::size_t sizeTo, const std::string &strFill)
{
    return expandAfterUpTo(str, str.size(), sizeTo, strFill);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
constexpr
char digitToChar(int d, bool bUpper)
{
    return d<10 ? details::digitToCharDec(d) : details::digitToCharAlpha(d-10, bUpper);
}

//----------------------------------------------------------------------------
template<typename IntType>
std::string simpleToString(IntType i, NumeralSystem ns=NumeralSystem::dec, bool bUpper=false)
{
    std::string res;
    IntType base = IntType(getNumeralSystemBase(ns));
    while(i)
    {
        res.append(1, digitToChar(int(i%base), bUpper));
        i /= base;
    }

    if (res.empty())
        res.assign(1, '0');

    std::reverse(res.begin(), res.end());

    return res;
}

//----------------------------------------------------------------------------
inline
unsigned charRangeToUnsigned(const char *b, const char *e)
{
    unsigned res = 0;

    if (b==e)
        throw std::invalid_argument("charRangeToUnsigned: can't convert empty string");

    for(; b!=e; ++b)
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
    return ch==utf32_char_t('?')
        || ch==utf32_char_t('s')
        || ch==utf32_char_t('S') 
        || ch==utf32_char_t('b')
        || ch==utf32_char_t('B')
        || ch==utf32_char_t('c')
        || ch==utf32_char_t('d')
        || ch==utf32_char_t('h')
        || ch==utf32_char_t('H')
        || ch==utf32_char_t('o')
        || ch==utf32_char_t('x')
        || ch==utf32_char_t('X')
        || ch==utf32_char_t('a')
        || ch==utf32_char_t('A')
        || ch==utf32_char_t('e')
        || ch==utf32_char_t('E')
        || ch==utf32_char_t('f')
        || ch==utf32_char_t('F')
        || ch==utf32_char_t('g')
        || ch==utf32_char_t('G')
        || ch==utf32_char_t('n')
        || ch==utf32_char_t('t')
        || ch==utf32_char_t('T')
        || ch==utf32_char_t('y')
        || ch==utf32_char_t('Y')
        || ch==utf32_char_t('p')
        || ch==utf32_char_t('P')
        || ch==utf32_char_t('r')
        || ch==utf32_char_t('R')
        || ch==utf32_char_t('%')
        || ch==utf32_char_t('$')
    ;
}

//----------------------------------------------------------------------------
inline
constexpr
bool isFormatAnySpecialChar(utf32_char_t ch)
{
    return isFormatAlignMarker(ch)
        || isFormatSignMarker(ch)
        || isFormatConvertMarker(ch)
        || isFormatAlterChar(ch)
        || isFormatFormatThousandSep(ch)
        || isFormatPeriodChar(ch)
        || isFormatLocaleChar(ch)
        || isFormatDigit(ch)
        || isFormatTypeChar(ch)
        || ch==utf32_char_t('z')
        || ch==utf32_char_t('|')
        || ch==utf32_char_t('{')
        || ch==utf32_char_t('~')
           ;
}

//----------------------------------------------------------------------------
// Подсчитывает длину строки в символах
//#! DefaultUtfWidthCalculator
struct DefaultUtfWidthCalculator
{

    std::size_t operator()(marty::utf::unicode_char_t ch) const
    {
        // suf - simpleUnicodeFeature
        if (marty::utf::sufIsZeroWidthSpace(ch) || marty::utf::sufIsCombiningDiacretic(ch))
            return 0;

        return 1;
    }

    std::size_t operator()(const char* b, const char* e) const
    {
        auto it    = marty::utf::UtfInputIterator<char>(b, e);
        auto endIt = marty::utf::UtfInputIterator<char>();

        std::size_t size = 0;

        for(; it!=endIt; ++it)
        {
            size += operator()(*it);
        }

        return size;
    }

    std::size_t operator()(const char* b, std::size_t strLen) const
    {
        return operator()(b, b+std::ptrdiff_t(strLen));
    }

}; // struct DefaultUtfWidthCalculator
//#!

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
inline
char toupper(char ch)
{
    if (ch>='a' && ch<='z')
        ch = ch - 'a' + 'A';
    return ch;
}

//----------------------------------------------------------------------------
inline
void toupper(std::string &s)
{
    for(auto &ch : s)
    {
        ch = toupper(ch);
    }
}

//----------------------------------------------------------------------------
inline
std::string toupper_copy(std::string s)
{
    for(auto &ch : s)
    {
        ch = toupper(ch);
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
template< typename C, typename = void >
struct has_ctor_require_two_pointers_ : std::false_type {};
 
template<typename T>
struct has_ctor_require_two_pointers_<T, std::void_t<typename T::ctor_require_two_pointers>> : std::true_type {};

template<typename T>
inline constexpr bool has_ctor_require_two_pointers = has_ctor_require_two_pointers_<T>::value;

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
// Для поддержки форматирования пользовательских типов

//----------------------------------------------------------------------------
template< typename T, typename = void >
struct has_free_standing_enum_is_flags : std::false_type {};
 
template<typename T>
struct has_free_standing_enum_is_flags<T, std::void_t<decltype(enum_is_flags(static_cast<T>(0)))>> : std::true_type {};

//------------------------------
template< typename T, typename = void >
struct has_free_standing_enum_serialize : std::false_type {};
 
template<typename T>
struct has_free_standing_enum_serialize<T, std::void_t<decltype(enum_serialize(static_cast<T>(0)))>> : std::true_type {};

//------------------------------
template< typename T, typename = void >
struct has_free_standing_enum_serialize_flags : std::false_type {};
 
template<typename T>
struct has_free_standing_enum_serialize_flags<T, std::void_t<decltype(enum_serialize_flags(static_cast<T>(0)))>> : std::true_type {};

//------------------------------
template<typename T>
T& as_lvalue(const T &t)
{
    return const_cast<T&>(t);
}

//------------------------------
#if 0
template< typename WidthCalculator, typename StringType, typename T, typename = void >
struct has_free_standing_martyFormatValueFormat : std::false_type {};
 
template<typename WidthCalculator, typename StringType, typename T>
struct has_free_standing_martyFormatValueFormat< T
                                               , std::void_t< decltype(martyFormatValueFormat<WidthCalculator, StringType>( as_lvalue(marty::format::FormattingOptions{})
                                                                                                                          , T{}
                                                                                                                          )
                                                                      )
                                                            >
                                               > : std::true_type {};
#endif

// (c) deepseek
// Основной шаблон
// template< typename WidthCalculator, 
//           typename StringType, 
//           typename T, 
//           typename = void >
// struct has_free_standing_martyFormatValueFormat 
//     : std::false_type 
// {};
//  
// // Специализация
// template< typename WidthCalculator, 
//           typename StringType, 
//           typename T >
// struct has_free_standing_martyFormatValueFormat< WidthCalculator
//                                                , StringType
//                                                , T
//                                                , std::void_t< decltype(martyFormatValueFormat<WidthCalculator, StringType>( std::declval<marty::format::FormattingOptions&>()
//                                                                                                                           , std::declval<T>()
//                                                                                                                           )
//                                                                       ) 
//                                               >
//     > : std::true_type 
// {};

//------------------------------
// template< typename StringType
//         , typename T
//         , typename = void
//         >
// struct has_free_standing_martyFormatSimpleConvertToString
//     : std::false_type
// {};
 
// template<typename StringType, typename T>
// struct has_free_standing_martyFormatSimpleConvertToString< StringType, T
//                                                          , std::void_t< decltype( martyFormatSimpleConvertToString<StringType>(T{})
//                                                                                 )
//                                                                       >
//                                                          > : std::true_type {};
// template<typename StringType, typename T>
// struct has_free_standing_martyFormatSimpleConvertToString< 
//     StringType, 
//     T,
//     std::void_t<decltype(
//         martyFormatSimpleConvertToString<StringType>(std::declval<T>())
//     )>
// > : std::true_type {};

// SFINAE-хелпер
namespace detail {

// template<typename ST, typename T>
// auto test_const_ref(int) -> decltype(
//     martyFormatSimpleConvertToString<ST>(std::declval<const T&>()),
//     std::true_type{}
// );
//  
// template<typename ST, typename T>
// auto test_value(int) -> decltype(
//     martyFormatSimpleConvertToString<ST>(std::declval<T>()),
//     std::true_type{}
// );
//  
// template<typename ST, typename T>
// auto test_const_ref(...) -> std::false_type;
//  
// template<typename ST, typename T>
// auto test_value(...) -> std::false_type;


} // namespace detail



// template< typename WidthCalculator, 
//           typename StringType, 
//           typename T, 
//           typename = void >
// struct has_free_standing_martyFormatValueFormat 
//     : std::false_type 
// {};
//  
// // Специализация
// template< typename WidthCalculator, 
//           typename StringType, 
//           typename T >
// struct has_free_standing_martyFormatValueFormat< WidthCalculator
//                                                , StringType
//                                                , T
// //                                                , std::void_t< decltype(martyFormatValueFormat<WidthCalculator, StringType>( std::declval<marty::format::FormattingOptions&>()
// //                                                                                                                           , std::declval<T>()
// //                                                                                                                           )
// //                                                                       ) 
// //                                               >
// //     > : std::true_type 
// // {};
//     std::void_t<
//         decltype(detail::test_const_ref<StringType, T>(0)),
//         decltype(detail::test_value<StringType, T>(0))
//     >
// > : std::bool_constant<
//         decltype(detail::test_const_ref<StringType, T>(0))::value ||
//         decltype(detail::test_value<StringType, T>(0))::value
//     > {};



// template<typename StringType, typename T, typename = void>
// struct has_free_standing_martyFormatSimpleConvertToString 
//     : std::false_type {};
//  
// template<typename StringType, typename T>
// struct has_free_standing_martyFormatSimpleConvertToString< 
//     StringType, 
//     T,
//     std::void_t<
//         decltype(detail::test_const_ref<StringType, T>(0)),
//         decltype(detail::test_value<StringType, T>(0))
//     >
// > : std::bool_constant<
//         decltype(detail::test_const_ref<StringType, T>(0))::value ||
//         decltype(detail::test_value<StringType, T>(0))::value
//     > {};
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// (c) rg45
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

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template < typename T
         , std::enable_if_t< std::is_integral_v<T>, int> = 0
         >
constexpr
std::make_unsigned_t<T> toUnsignedCast(T t)
{
   using UT = std::make_unsigned_t<T>;
   return static_cast<UT>(t);
}

//----------------------------------------------------------------------------
template < typename T
         , std::enable_if_t< std::is_integral_v<T>, int> = 0
         >
constexpr
std::make_signed_t<T> toSignedCast(T t)
{
   using UT = std::make_signed_t<T>;
   return static_cast<UT>(t);
}

//----------------------------------------------------------------------------
template < typename T
         , std::enable_if_t< std::is_integral_v<T> and not std::is_signed_v<T>, int> = 0
         >
constexpr
std::make_unsigned_t<T> toUnsignedAbs(T t)
{
   using UT = std::make_unsigned_t<T>;
   return static_cast<UT>(t);
}

template < typename T
         , std::enable_if_t< std::is_integral_v<T> and std::is_signed_v<T>, int> = 0
         >
constexpr
std::make_unsigned_t<T> toUnsignedAbs(T t)
{
   using UT = std::make_unsigned_t<T>;
   // return static_cast<UT>(t);
   return (t<0) ? static_cast<UT>(static_cast<T>(-(t + static_cast<T>(1))) + static_cast<T>(1)) : (static_cast<UT>(t));
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template <typename T>
std::string formatFloat( T value, char spec
                       , std::size_t *pSignIdx  = 0
                       , std::size_t *pPowerIdx = 0
                       , int precision = -1
                       )
{

    std::string strRes;

#if defined(__GNUC__) && (__GNUC__ < 11)

    // GCC ниже 11ой версии не поддерживает std::to_chars
    // https://www.opennet.ru/man.shtml?topic=fcvt&category=3&russian=0
    // https://learn.microsoft.com/ru-ru/cpp/c-runtime-library/reference/fcvt?view=msvc-170
    // https://learn.microsoft.com/ru-ru/cpp/c-runtime-library/reference/ecvt?view=msvc-170
    // https://learn.microsoft.com/ru-ru/cpp/c-runtime-library/reference/gcvt?view=msvc-170

    // fcvt/ecvt/gcvt временами выдают какую-то лажу
    // Но потоки работают нормально

    std::ostringstream oss;

    // Устанавливаем точность, если она задана
    if (precision >= 0)
        oss.precision(precision);

    // Форматируем всё в нижнем регистре
    switch (fmt)
    {
        case 'a':
        case 'A':
            oss << std::hexfloat;
            // oss << (fmt == 'A' ? std::uppercase : std::nouppercase);
            break;

        case 'e':
        case 'E':
            oss << std::scientific;
            // oss << (fmt == 'E' ? std::uppercase : std::nouppercase);
            break;

        case 'f':
        case 'F':
            oss << std::fixed;
            // oss << (fmt == 'F' ? std::uppercase : std::nouppercase);
            break;

        case 'g':
        case 'G':
            oss.unsetf(std::ios_base::fixed | std::ios_base::scientific);
            // oss << (fmt == 'G' ? std::uppercase : std::nouppercase);
            break;

        // default:
        //     throw std::invalid_argument("Invalid format specifier");
    }
    
    oss << value;

    strRes = oss.str();


#else

    // MSVC2019 поддерживает std::chars_format, включая hex, а ниже версия компилятора MSVC нас и не интересует

    char buffer[128];
    std::to_chars_result result;

    // Выбор формата для std::to_chars
    std::chars_format fmt = std::chars_format::general;
    switch (spec) {
        case 'a': case 'A': fmt = std::chars_format::hex; break;
        case 'e': case 'E': fmt = std::chars_format::scientific; break;
        case 'f': case 'F': fmt = std::chars_format::fixed; break;
        case 'g': case 'G': fmt = std::chars_format::general; break;
        default: throw std::invalid_argument("Invalid specifier");
    }

    // Преобразование числа в строку
    result = std::to_chars(&buffer[0], &buffer[128], value, fmt, precision);
    // if (result.ec != std::errc())
    // {
    //     //throw std::runtime_error("Formatting failed");
    // }

    if (result.ec==std::errc())
    {
        strRes = std::string(&buffer[0], result.ptr);
    }

#endif

    if (pSignIdx)
    {
       *pSignIdx  = std::size_t(-1);
       if (!strRes.empty() && (strRes.front()=='-' || strRes.front()=='+'))
           *pSignIdx = 0;
    }

    // if (pPowerIdx)
    //    *pPowerIdx = std::size_t(-1);

    if (pPowerIdx)
    {
        char eChar = (spec=='a' || spec=='A') ? 'p' : 'e';
        *pPowerIdx = strRes.find(eChar);
    }

    return strRes;

}

//----------------------------------------------------------------------------
inline
char splitExponentionalNumberString(std::string numStr, std::string &floatStr, std::string &expStr)
{
    auto pos = numStr.find_first_of("pP");
    if (pos!=numStr.npos)
    {
        floatStr.assign(numStr, 0, pos);
        expStr.assign(numStr, pos + 1, numStr.npos);
        return numStr[pos];
    }

    pos = numStr.find_first_of("eE");
    if (pos!=numStr.npos)
    {
        floatStr.assign(numStr, 0, pos);
        expStr.assign(numStr, pos + 1, numStr.npos);
        return numStr[pos];
    }

    floatStr = numStr;
    expStr.clear(); // на всякий случай
    return 0;
}

//----------------------------------------------------------------------------
inline
char splitFloatNumberString(std::string numStr, std::string &partInteger, std::string &partFractional)
{
    auto pos = numStr.find_first_of(".,");
    if (pos==numStr.npos)
    {
        partInteger = numStr;
        partFractional.clear(); // на всякий случай
        return 0;
    }

    partInteger.assign(numStr, 0, pos);
    partFractional.assign(numStr, pos + 1, numStr.npos);

    return numStr[pos];
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#if 0
inline
constexpr
std::array<const char*, 13> getRomanDigitsLatin(bool bUpper)
{
    return bUpper
         ? std::array<const char*, 13>{ "I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"}
         : std::array<const char*, 13>{ "i", "iv", "v", "ix", "x", "xl", "l", "xc", "c", "cd", "d", "cm", "m"}
         ;
}

//----------------------------------------------------------------------------
inline
constexpr
std::array<const char*, 16> getRomanDigitsUnicode(bool bUpper)
{
    return bUpper
         ? std::array<const char*, 16>{ "\xE2\x85\xA0" // U+2160	Ⅰ	Римская цифра I	    1	Заглавный
                                      , "\xE2\x85\xA1" // U+2161	Ⅱ	Римская цифра II	2	Заглавный
                                      , "\xE2\x85\xA2" // U+2162	Ⅲ	Римская цифра III	3	Заглавный
                                      , "\xE2\x85\xA3" // U+2163	Ⅳ	Римская цифра IV	4	Заглавный
                                      , "\xE2\x85\xA4" // U+2164	Ⅴ	Римская цифра V 	5	Заглавный
                                      , "\xE2\x85\xA5" // U+2165	Ⅵ	Римская цифра VI	6	Заглавный
                                      , "\xE2\x85\xA6" // U+2166	Ⅶ	Римская цифра VII	7	Заглавный
                                      , "\xE2\x85\xA7" // U+2167	Ⅷ	Римская цифра VIII	8	Заглавный
                                      , "\xE2\x85\xA8" // U+2168	Ⅸ	Римская цифра IX	9	Заглавный
                                      , "\xE2\x85\xA9" // U+2169	Ⅹ	Римская цифра X 	10	Заглавный
                                      , "\xE2\x85\xAA" // U+216A	Ⅺ	Римская цифра XI	11	Заглавный
                                      , "\xE2\x85\xAB" // U+216B	Ⅻ	Римская цифра XII	12	Заглавный
                                      , "\xE2\x85\xAC" // U+216C	Ⅼ	Римская цифра L	    50	Заглавный
                                      , "\xE2\x85\xAD" // U+216D	Ⅽ	Римская цифра C	    100	Заглавный
                                      , "\xE2\x85\xAE" // U+216E	Ⅾ	Римская цифра D	    500	Заглавный
                                      , "\xE2\x85\xAF" // U+216F	Ⅿ	Римская цифра M	    1000 Заглавный
                                      }
         : std::array<const char*, 16>{ "\xE2\x85\xB0" // U+2170	ⅰ	Римская цифра i 	1	Строчный
                                      , "\xE2\x85\xB1" // U+2171	ⅱ	Римская цифра ii	2	Строчный
                                      , "\xE2\x85\xB2" // U+2172	ⅲ	Римская цифра iii	3	Строчный
                                      , "\xE2\x85\xB3" // U+2173	ⅳ	Римская цифра iv	4	Строчный
                                      , "\xE2\x85\xB4" // U+2174	ⅴ	Римская цифра v	    5	Строчный
                                      , "\xE2\x85\xB5" // U+2175	ⅵ	Римская цифра vi	6	Строчный
                                      , "\xE2\x85\xB6" // U+2176	ⅶ	Римская цифра vii	7	Строчный
                                      , "\xE2\x85\xB7" // U+2177	ⅷ	Римская цифра viii	8	Строчный
                                      , "\xE2\x85\xB8" // U+2178	ⅸ	Римская цифра ix	9	Строчный
                                      , "\xE2\x85\xB9" // U+2179	ⅹ	Римская цифра x 	10	Строчный
                                      , "\xE2\x85\xBA" // U+217A	ⅺ	Римская цифра xi	11	Строчный
                                      , "\xE2\x85\xBB" // U+217B	ⅻ	Римская цифра xii	12	Строчный
                                      , "\xE2\x85\xBC" // U+217C	ⅼ	Римская цифра l	    50	Строчный
                                      , "\xE2\x85\xBD" // U+217D	ⅽ	Римская цифра c	    100	Строчный
                                      , "\xE2\x85\xBE" // U+217E	ⅾ	Римская цифра d	    500	Строчный
                                      , "\xE2\x85\xBF" // U+217F	ⅿ	Римская цифра m	    1000	Строчный
                                      }
         ;
}
#endif
//----------------------------------------------------------------------------

// Вытащил из старых сорцов форматирование чисел в римской нотации
// Вроде раньше работало, особо не разбирался, только чутка адаптировал
// Есть ещё другая моя же старая реализация
// Надо будет потом потестить и её

#define MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS1(d)  (d)
#define MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS5(d)  (d-1)
#define MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS10(d)  (d-2)

//----------------------------------------------------------------------------
namespace details {

//----------------------------------------------------------------------------
inline
std::string digit2romanLatin(unsigned digit, unsigned power, const char* const roman_digits)
{
    ::std::string res;
    
    if (power<=0)
        return res;
    
    if (digit)
    {     
        res += digit2romanLatin(digit/10, power-2, roman_digits);
        digit %= 10;
    } 
   
    if (!digit) return res;
   
    if (digit>=1 && digit<=3)
    {
        for (unsigned i=0; i<digit; i++)
             res += roman_digits[MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS1(power)];
        return res;
    }
   
    if (digit==4)
        res += roman_digits[MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS1(power)];
   
    if (digit<6)
    {
        res += roman_digits[MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS5(power)];
        return res;
    }
   
    if (digit>5 && digit<=8)
    {
        res += roman_digits[MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS5(power)];
        for (unsigned i=0; i<digit-5; i++)
             res += roman_digits[MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS1(power)];
        return res;
    }
   
    if (digit==9)
        res += roman_digits[MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS1(power)];

    res += roman_digits[MARTY_FORMAT_DIGIT2ROMANLATIN_AUX_ROMANOFFS10(power)];

    return res;
}

inline
void formatRomanIntegerLatinImpl(std::string &strFormatTo, unsigned i, const char *roman_digits)
{
    unsigned thousends = i/1000ull;
    i %= 1000u;
    for (unsigned j=0; j<thousends; j++)
        strFormatTo.append(1,roman_digits[0]);
    strFormatTo.append(digit2romanLatin(unsigned(i), 6, roman_digits));
}

} // namespace details

//----------------------------------------------------------------------------
inline
void formatRomanIntegerLatin(std::string &strFormatTo, unsigned i, bool upperCase=true)
{
    if (i==0)
    {
        strFormatTo = upperCase ? "NULLA" /* "Nulla" */  : "nulla";
        return;
    }
    else if (i>9999)
    {
        strFormatTo = upperCase ? "INFINITAS" /* "Infinitas" */  : "infinitas";
        return;
    }

    details::formatRomanIntegerLatinImpl(strFormatTo, i, upperCase ? "MDCLXVI" : "mdclxvi");
}

//----------------------------------------------------------------------------
inline
std::string formatRomanIntegerLatin(unsigned i, bool upperCase=true)
{
    std::string resStr;
    formatRomanIntegerLatin(resStr, i, upperCase);
    return resStr;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace details {

//----------------------------------------------------------------------------
std::array< std::pair<unsigned, const char*>, 18>
makeRomanDigitsUnicodeArrayUpper()
{
    return std::array< std::pair<unsigned, const char*>, 18>
    {
    { {1000u, "\xE2\x85\xAF"}                // Ⅿ M
    , {900u,  "\xE2\x85\xAD\xE2\x85\xAF"}    // ⅭⅯ CM
    , {500u,  "\xE2\x85\xAE"}                // Ⅾ D
    , {400u,  "\xE2\x85\xAD\xE2\x85\xAE"}    // ⅭⅮ CD
    , {100u,  "\xE2\x85\xAD"}                // Ⅽ C
    , {90u,   "\xE2\x85\xA9\xE2\x85\xAD"}    // ⅩⅭ XC
    , {50u,   "\xE2\x85\xAC"}                // Ⅼ L 
    , {40u,   "\xE2\x85\xA9\xE2\x85\xAC"}    // ⅩⅬ XL
    , {10u,   "\xE2\x85\xA9"}                // Ⅹ X
    , {9u,    "\xE2\x85\xA8"}                // Ⅸ IX
    , {8u,    "\xE2\x85\xA7"}                // Ⅷ VIII
    , {7u,    "\xE2\x85\xA6"}                // Ⅶ VII
    , {6u,    "\xE2\x85\xA5"}                // Ⅵ VI
    , {5u,    "\xE2\x85\xA4"}                // Ⅴ V
    , {4u,    "\xE2\x85\xA3"}                // Ⅳ IV
    , {3u,    "\xE2\x85\xA2"}                // Ⅲ III
    , {2u,    "\xE2\x85\xA1"}                // Ⅱ II
    , {1u,    "\xE2\x85\xA0"}                // Ⅰ I
    }
    };
}

//----------------------------------------------------------------------------
std::array< std::pair<unsigned, const char*>, 18>
makeRomanDigitsUnicodeArrayLower()
{
    return std::array< std::pair<unsigned, const char*>, 18>
    {
    { {1000u, "\xE2\x85\xBF"}                // Ⅿ M
    , {900u,  "\xE2\x85\xBD\xE2\x85\xBF"}    // ⅭⅯ CM
    , {500u,  "\xE2\x85\xBE"}                // Ⅾ D
    , {400u,  "\xE2\x85\xBD\xE2\x85\xBE"}    // ⅭⅮ CD
    , {100u,  "\xE2\x85\xBD"}                // Ⅽ C
    , {90u,   "\xE2\x85\xB9\xE2\x85\xBD"}    // ⅩⅭ XC
    , {50u,   "\xE2\x85\xBC"}                // Ⅼ L 
    , {40u,   "\xE2\x85\xB9\xE2\x85\xBC"}    // ⅩⅬ XL
    , {10u,   "\xE2\x85\xB9"}                // Ⅹ X
    , {9u,    "\xE2\x85\xB8"}                // Ⅸ IX
    , {8u,    "\xE2\x85\xB7"}                // Ⅷ VIII
    , {7u,    "\xE2\x85\xB6"}                // Ⅶ VII
    , {6u,    "\xE2\x85\xB5"}                // Ⅵ VI
    , {5u,    "\xE2\x85\xB4"}                // Ⅴ V
    , {4u,    "\xE2\x85\xB3"}                // Ⅳ IV
    , {3u,    "\xE2\x85\xB2"}                // Ⅲ III
    , {2u,    "\xE2\x85\xB1"}                // Ⅱ II
    , {1u,    "\xE2\x85\xB0"}                // Ⅰ I
    }
    };
}

//----------------------------------------------------------------------------
const std::array< std::pair<unsigned, const char*>, 18>&
getRomanDigitsUnicodeArrayUpper()
{
    static auto a = makeRomanDigitsUnicodeArrayUpper();
    return a;
}

//----------------------------------------------------------------------------
const std::array< std::pair<unsigned, const char*>, 18>&
getRomanDigitsUnicodeArrayLower()
{
    static auto a = makeRomanDigitsUnicodeArrayLower();
    return a;
}

//----------------------------------------------------------------------------
const std::array< std::pair<unsigned, const char*>, 18>&
getRomanDigitsUnicodeArray(bool upperCase=true)
{
    return upperCase ? getRomanDigitsUnicodeArrayUpper() : getRomanDigitsUnicodeArrayLower();
}

//----------------------------------------------------------------------------
inline
void formatRomanIntegerUnicodeImpl(std::string &strFormatTo, unsigned i, bool upperCase=true)
{
    const auto &romanNumerals = getRomanDigitsUnicodeArray(upperCase);

    unsigned thousends = i/1000u;
    i %= 1000u;
    for (unsigned j=0; j<thousends; j++)
        strFormatTo.append(romanNumerals[0].second);

    for (const auto& p : romanNumerals)
    {
        while (i >= p.first)
        {
            strFormatTo.append(p.second);
            i -= p.first;
        }
    }
    
}

//----------------------------------------------------------------------------

} // namespace details

//----------------------------------------------------------------------------
inline
void formatRomanIntegerUnicode(std::string &strFormatTo, unsigned i, bool upperCase=true)
{
    if (i==0)
    {
        strFormatTo = upperCase ? "NULLA" /* "Nulla" */  : "nulla";
        return;
    }
    else if (i>9999)
    {
        strFormatTo = upperCase ? "INFINITAS" /* "Infinitas" */  : "infinitas";
        return;
    }

    details::formatRomanIntegerUnicodeImpl(strFormatTo, i, upperCase);
}

//----------------------------------------------------------------------------
inline
std::string formatRomanIntegerUnicode(unsigned i, bool upperCase=true)
{
    std::string resStr;
    formatRomanIntegerUnicode(resStr, i, upperCase);
    return resStr;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
void formatRomanInteger(std::string &strFormatTo, unsigned i, bool upperCase=true, bool bUnicode=false)
{
    if (bUnicode)
        formatRomanIntegerUnicode(strFormatTo, i, upperCase);
    else
        formatRomanIntegerLatin(strFormatTo, i, upperCase);
}

//----------------------------------------------------------------------------
inline
std::string formatRomanInteger(unsigned i, bool upperCase=true, bool bUnicode=false)
{
    return bUnicode ? formatRomanIntegerUnicode(i, upperCase) : formatRomanIntegerLatin(i, upperCase);
}

//----------------------------------------------------------------------------


    // ::std::wstring formatRomanNumber(UINT64 arab, const wchar_t  **romanar)
    //    {
    //     const UINT64    arabar[]       = {  1,   4,    5,   9,    10,  40,  50,   90,  100, 400,  500, 900,  1000};
    //     //const wchar_t  *romanarUpper[] = { "I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
    //  
    //     const SIZE_T m            = sizeof(arabar)/sizeof(arabar[0])-1, arabmax = (SIZE_T)arabar[m];
    //     const wchar_t romanmax = romanar[m][0];
    //     
    //     if (!arab)
    //        {
    //         return ::std::wstring(L"-");
    //        }
    //  
    //     ::std::wstring roman;
    //     int  /* i = 0, */  n = 0;
    //     while(arab>arabmax)
    //        {
    //         //roman[i++] = romanmax;
    //         roman.append(1, romanmax);
    //         arab -= arabmax;
    //        }
    //     n=m;
    //     while(arab > 0)
    //        {
    //         if (arab >= arabar[n])
    //            {
    //             //roman[i++] = romanar[n][0];
    //             roman.append(1, romanar[n][0]);
    //             if (n&1)
    //                {
    //                 //roman[i++] = romanar[n][1];
    //                 roman.append(1, romanar[n][1]);
    //                }
    //             arab -= arabar[n];
    //            }
    //         else
    //            {
    //             n--;
    //            }
    //        }
    //     //roman[i]=0;
    //     return roman;
    //    }








        // if (!bSmall)
        //    {
        //     const wchar_t  *romanarUpper[] = { L"I", L"IV", L"V", L"IX", L"X", L"XL", L"L", L"XC", L"C", L"CD", L"D", L"CM", L"M"};
        //     romanString = formatRomanNumber( val, romanarUpper );
        //    }
        // else
        //    {
        //     const wchar_t  *romanarLower[] = { L"i", L"iv", L"v", L"ix", L"x", L"xl", L"l", L"xc", L"c", L"cd", L"d", L"cm", L"m"};
        //     romanString = formatRomanNumber( val, romanarLower );
        //    }



//----------------------------------------------------------------------------

} // namespace utils
} // namespace format
} // namespace marty
// marty::format::utils::
// #include "marty_format/marty_format.h"

