/*!
    \file
    \brief Типы для marty::format
 */
#pragma once

#include "enums.h"
#include "defs.h"

// 
#include "marty_utf/utf.h"
//
#include "marty_decimal/marty_decimal.h"
//
#include "utils.h"
//namespace marty_utf {

#include <string>
#include <variant>

// #include "marty_format/marty_format.h"
// marty::format::
namespace marty{
namespace format{


using utf32_char_t = marty::utf::utf32_char_t;


//! Параметры форматирования
struct FormattingOptions
{
    std::string             argId;
    std::string             fieldWidthRef;
    std::string             precisionRef;

    // unsigned                argId = 0; // Индекс значения в массиве, начинается с единицы, или 0, если автоматически вычисляется

    FormattingOptionsFlags  optionsFlags   = FormattingOptionsFlags::none;
    int                     fieldWidth     = 0; // auto
    int                     precision      = 6; // default precision

    char                    alignment      = '>'; // По левому краю ('<'), по правому краю ('>'), по ширине ('^'). По умолчанию - по правому краю
    utf32_char_t            fillChar       = utf32_char_t(' '); // Символ заполнения, по умолчанию - пробел.
    utf32_char_t            grouppingChar  = 0; // разделитель разрядов, по умолчанию - 0 (без разделения).
    char                    typeChar       = 0;   // Символ типа, 0 - auto

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



//----------------------------------------------------------------------------
using FormatArgumentVariant = 
    std::variant< bool
                , char
                , std::int8_t
                , std::uint8_t
                , std::int16_t
                , std::uint16_t
                , std::int32_t
                , std::uint32_t
                , std::int64_t
                , std::uint64_t
                , float
                , double
                , long double
                , const char*
                , const wchar_t*
                , std::string
                , std::wstring
                , marty::Decimal
                >;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace format
} // namespace marty
// marty::format::
// #include "marty_format/marty_format.h"

