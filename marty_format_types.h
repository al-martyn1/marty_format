/*!
    \file
    \brief Типы для marty::format
 */
#pragma once

#include "defs.h"
#include "enums.h"
#include "exceptions.h"

// 
#include "marty_utf/utf.h"
//
#include "marty_decimal/marty_decimal.h"
//
#include "utils.h"
//namespace marty_utf {

#include <functional>
#include <string>
#include <variant>


// #include "marty_format/marty_format.h"
// marty::format::
namespace marty{
namespace format{


using utf32_char_t = marty::utf::utf32_char_t;

// Немножко оптимизируем, и делаем индексы поменьше размером
// чтобы структура FormattingOptions была не такая жирная.
// Даже для больших компов может быть польза,
// а уж во встройке - однозначно

using arg_idx_t = std::uint16_t;
constexpr static const arg_idx_t arg_idx_npos = arg_idx_t(-1);

// А не может ли поле быть шириной больше 65535?
// Усекаем молча или генерируем исключение?
using width_t   = std::uint16_t ; // For fieldWidth and precision


//! Параметры форматирования
struct FormattingOptions
{
    std::string             argId;
    std::string             fillRef;
    std::string             fieldWidthRef;
    std::string             precisionRef;

    // unsigned                argId = 0; // Индекс значения в массиве, начинается с единицы, или 0, если автоматически вычисляется

    FormattingOptionsFlags  optionsFlags   = FormattingOptionsFlags::none;
    int                     fieldWidth     = 0; // auto
    int                     precision      = 6; // default precision

    utf32_char_t            convertChar    = 0;
    char                    alignment      = '>'; // По левому краю ('<'), по правому краю ('>'), по ширине ('^'). По умолчанию - по правому краю
    utf32_char_t            fillChar       = utf32_char_t(' '); // Символ заполнения, по умолчанию - пробел.
    utf32_char_t            grouppingChar  = 0; // разделитель разрядов, по умолчанию - 0 (без разделения).
    utf32_char_t            fractionalGrouppingChar = 0; // разделитель разрядов после десятичной точки, по умолчанию - 0 (без разделения).
    char                    typeChar       = 0;   // Символ типа, 0 - auto

    FormattingFlags         formattingFlags = FormattingFlags::none; // Копируем сюда аргумент функции форматирования, чтобы не таскать отдельно

}; // struct FormattingOptions


template<typename StramType>
StramType& operator<<(StramType& oss, const FormattingOptions opts)
{
    oss << "Options  : " << enum_serialize_flags(opts.optionsFlags) << "\n";


    if ((opts.optionsFlags&FormattingOptionsFlags::argIdTaken)==0)
        oss << "ArgId    : " << "auto" << "\n";
    else
        oss << "ArgId    : " << opts.argId << "\n";


    if ((opts.optionsFlags&FormattingOptionsFlags::fieldWidthTaken)==0)
        oss << "Width    : " << "auto" << "\n";
    else
    {
        if ((opts.optionsFlags&FormattingOptionsFlags::fieldWidthIndirect)==0)
            oss << "Width    : " << opts.fieldWidth << "\n";
        else 
            oss << "WidthId  : " << opts.fieldWidthRef << "\n";
    }


    if ((opts.optionsFlags&FormattingOptionsFlags::fieldWidthTaken)==0)
        oss << "Precision: " << "auto" << "\n";
    else
    {
        if ((opts.optionsFlags&FormattingOptionsFlags::precisionIndirect)==0)
            oss << "Precision: " << opts.precision << "\n";
        else
            oss << "PrecisionId: " << opts.precisionRef << "\n";
    }


    // if ((opts.optionsFlags&FormattingOptionsFlags::alignTaken)==0)
    //     oss << "Align    : " << "auto" << "\n";
    // else
    {
        oss << "Align    : ";
        switch(opts.alignment)
        {
            case '<': oss << "left"; break;
            case '>': oss << "right"; break;
            case '^': oss << "width"; break;
            default : oss << "unknown"; break;
        }
        oss << "\n";
    }

    oss << "Fill Char: " << opts.fillChar << "\n";

    oss << "Grp Char : " << opts.grouppingChar << "\n";

    oss << "Type Char: " << std::string(1, opts.typeChar) << "\n";


    return oss;
}

//----------------------------------------------------------------------------


// https://ru.wikipedia.org/wiki/Functional_(C%2B%2B)

//#! BasicFormatValueFilter
template<typename InputIteratorType, typename OutputIteratorType>
using BasicFormatValueFilter = std::function< void( InputIteratorType  // begin
                                                  , InputIteratorType  // end
                                                  , OutputIteratorType
                                                  )
                                            >;
//#!

//#! FormatValueFilter
using FormatValueFilter = BasicFormatValueFilter< marty::utf::UtfInputIterator<char>
                                                , marty::utf::UtfOutputIterator<char>
                                                >;
//#!

//----------------------------------------------------------------------------
//#! makeStandardFormatValueFilter
template<typename StringType>
FormatValueFilter makeStandardFormatValueFilter(const StringType &filterName)
//#!
{
    throw unknown_value_filter("unknown value filter");
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//#! FormatArgumentVariant
using FormatArgumentVariant = 
    std::variant< bool
                , char
                , unsigned char        // std::uint8_t
                , signed char          // std::int8_t
                , unsigned short       // std::uint16_t
                , signed short         // std::int16_t
                , unsigned int         // std::uint32_t
                , int                  // std::int32_t
                , unsigned long        // 32/64, it's depends
                , long                 // 32/64, it's depends
                , unsigned long long   // std::uint64_t
                , long long            // std::int64_t
                , float
                , double
                , long double
                , const char*
                , const wchar_t*
                , std::string
                , std::wstring
                , marty::Decimal
                , FormatValueFilter
                >;
//#!
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace format
} // namespace marty
// marty::format::
// #include "marty_format/marty_format.h"

