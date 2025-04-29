/*!
    \file
    \brief Типы для marty::format
 */
#pragma once

#include "enums.h"
#include "defs.h"
#include "utils.h"

// 
#include "marty_utf/utf.h"

#if defined(WIN32) || defined(_WIN32)

    #include <windows.h>

#endif

#include <algorithm>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>
#include <utility>

#include "undef_min_max.h"


// #include "marty_format/locale_info.h"
// marty::format::
namespace marty{
namespace format{


/*
     Numeral system - Система счисления

     Что мы хотим от локали?
     Группировка разрядов. Для 2/8/10/16 СС
     Строка-разделитель разрядов. Для 2/8/10/16 СС

     Десятичный разделитель
     Символ плюса
     Символ минуса
     Символ процента
     Формат отрицательных процентов
     Формат положительных процентов (если надо символ знака добавлять, и если не надо)
     Формат валюты
     Символ валюты

*/

// https://learn.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-getlocaleinfoa
// https://learn.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-getlocaleinfow
// https://learn.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-getlocaleinfoex
// https://learn.microsoft.com/en-us/windows/win32/intl/locale-information-constants

/*

LOCALE_ILZERO - Specifier for leading zeros in decimal fields - https://learn.microsoft.com/en-us/windows/win32/intl/locale-ilzero
LOCALE_INEG* Constants - https://learn.microsoft.com/en-us/windows/win32/intl/locale-ineg-constants
LOCALE_INEGATIVEPERCENT - Negative percentage formatting pattern for the locale - https://learn.microsoft.com/en-us/windows/win32/intl/locale-inegativepercent
LOCALE_IPOSITIVEPERCENT - Positive percentage formatting pattern for the locale - https://learn.microsoft.com/en-us/windows/win32/intl/locale-ipositivepercent
LOCALE_IDIGITS - Number of fractional digits placed after the decimal separator - https://learn.microsoft.com/en-us/windows/win32/intl/locale-idigits
LOCALE_IDIGITSUBSTITUTION - https://learn.microsoft.com/en-us/windows/win32/intl/locale-idigitsubstitution
  LOCALE_SNATIVEDIGITS - https://learn.microsoft.com/en-us/windows/win32/intl/locale-snative-constants
LOCALE_STHOUSAND - Characters that are used to separate groups of digits to the left of the decimal.
                   The maximum number of characters allowed for this string is four, including a terminating null character.
                   https://learn.microsoft.com/en-us/windows/win32/intl/locale-sthousand
LOCALE_SPERCENT -  Symbol used to indicate percentage, for example, "%". The value is a single string of 0 to 3 characters.
LOCALE_SNEGATIVESIGN - https://learn.microsoft.com/en-us/windows/win32/intl/locale-snegativesign
LOCALE_SPOSITIVESIGN - Localized string value for the positive sign for the locale
LOCALE_SGROUPING - Sizes for each group of digits to the left of the decimal - https://learn.microsoft.com/en-us/windows/win32/intl/locale-sgrouping
LOCALE_SDECIMAL - Character(s) used for the decimal separator, for example, "." in "3.14" or "," in "3,14" - https://learn.microsoft.com/en-us/windows/win32/intl/locale-sdecimal
LOCALE_SCURRENCY - String used as the local monetary symbol - https://learn.microsoft.com/en-us/windows/win32/intl/locale-scurrency
LOCALE_ICURRDIGITS - Number of fractional digits for the local monetary format - https://learn.microsoft.com/en-us/windows/win32/intl/locale-icurrdigits
LOCALE_ICURRENCY - Position of the monetary symbol in the positive currency mode - https://learn.microsoft.com/en-us/windows/win32/intl/locale-icurrency

*/


//----------------------------------------------------------------------------
//! Интерфейс, он же базовая реализация для "C" локали
/*! В форматных строках используются знаки для подстановки
    '#' - сюда подставляется отформатированное значение числа
    '-' - сюда подставляется символ минуса (строка)
    '+' - сюда подставляется символ плюса или пробельный символ (строка)
    '%' - сюда подставляется символ процента
    '$' - сюда подставляется символ валюты

*/
struct LocaleInfo
{
    virtual ~LocaleInfo() {}

#if defined(DEBUG) || defined(_DEBUG)

    using group_info_t = std::vector<std::uint8_t>;

    static void groupInfoAppend(group_info_t &gi, std::size_t n, std::uint8_t u)
    {
        gi.resize(gi.size()+n, u);
    }

#else

    using group_info_t = std::string<std::uint8_t>;

    static void groupInfoAppend(group_info_t &gi, std::size_t n, std::uint8_t u)
    {
        groupInfoAppend(gi, n, u);
    }

#endif

    // bool needAppendGroupSeparator(std::size_t curDigitIndex, LocaleInfoValueType vt, NumeralSystem ns )

    //! Обрабатывает только LocaleInfoValueType::thousandSeparator и LocaleInfoValueType::fractionalSeparator
    virtual std::string getGroupSeparator( LocaleInfoValueType vt //! 
                                         , NumeralSystem ns       //!
                                         ) const
    {
        if (vt!=LocaleInfoValueType::thousandSeparator && vt!=LocaleInfoValueType::fractionalSeparator)
        {
            throw std::invalid_argument("LocaleInfo::getGroupSeparator: vt is not thousandSeparator nor fractionalSeparator");
        }

        // Игнорируем vt

        switch(ns)
        {
            case NumeralSystem::currency : return " ";
            case NumeralSystem::bin      : [[fallthrough]];
            case NumeralSystem::hex      : return "_";
            case NumeralSystem::oct      : [[fallthrough]];
            case NumeralSystem::dec      : return "'";
            case NumeralSystem::unknown  : [[fallthrough]];
            case NumeralSystem::none     : [[fallthrough]];
            default: throw std::invalid_argument("LocaleInfo::getGroupSeparator: invalid numeral system taken");
        }
    }

    //! Обрабатывает LocaleInfoValueType::sign* и LocaleInfoValueType::format*
    virtual std::string getLocaleInfoValue(LocaleInfoValueType vt) const
    {
        switch(vt)
        {
            case LocaleInfoValueType::signDecimal           : return ".";
            case LocaleInfoValueType::signPlus              : return "+";
            case LocaleInfoValueType::signMinus             : return "-";
            case LocaleInfoValueType::signPercent           : return "%";
            case LocaleInfoValueType::signCurrency          : return "$";
            case LocaleInfoValueType::formatPercentNegative : return "-#%";
            case LocaleInfoValueType::formatPercentPositive : return "+#%";
            case LocaleInfoValueType::formatCurrencyNegative: return "-$#";
            case LocaleInfoValueType::formatCurrencyPositive: return "+$#";
            case LocaleInfoValueType::thousandSeparator     : [[fallthrough]];
            case LocaleInfoValueType::fractionalSeparator   : [[fallthrough]];
            case LocaleInfoValueType::unknown               : [[fallthrough]];
            case LocaleInfoValueType::none                  : [[fallthrough]];
            // case LocaleInfoValueType::: return "";
            default: throw std::invalid_argument("LocaleInfo::getLocaleInfoValue: value type taken");
        }
    }

    //! Подстановка локализованных символов в шаблон форматной строки
    static
    std::string substFormatString( const std::string &fmt
                                 , const std::string &numStr
                                 , const std::string &symbolMinus
                                 , const std::string &symbolPlus
                                 , const std::string &symbolPercent
                                 , const std::string &symbolCurrency
                                 )
    {
        std::string res; res.reserve( numStr        .size()
                                    + symbolMinus   .size()
                                    + symbolPlus    .size()
                                    + symbolPercent .size()
                                    + symbolCurrency.size()
                                    );
        for(auto ch : fmt)
        {
            switch(ch)
            {
                case '#': res.append(numStr        ); break;
                case '-': res.append(symbolMinus   ); break;
                case '+': res.append(symbolPlus    ); break;
                case '%': res.append(symbolPercent ); break;
                case '$': res.append(symbolCurrency); break;
                default : res.append(1, ch);
            }
        }

        return res;
    }

    //! Нулем может быть только последний элемент.
    /*! Данная функция фиксит разобранное из строки оисание групп
        При дальнейшей работе с группами элемент с нулевым размером 
        будет считаться последним
     */
    static
    group_info_t fixGroupInfo(group_info_t &&grpInfo)
    {
        for(std::size_t i=0u; i!=grpInfo.size(); ++i)
        {
            auto &g = grpInfo[i];
            if (g==0u) // нулевой размер группы?
            {
                if (i!=(grpInfo.size()-1)) // Не последний элемент, надо фиксить
                {
                    if (i) // не первый элемент, фиксим предыдущим значением
                    {
                        g = grpInfo[i-1];
                    }
                    else // Фиксим одним из последующих ненулевых значений
                    {
                        for(std::size_t j=i+1u; j!=grpInfo.size(); ++j)
                        {
                            if (grpInfo[j]!=0u)
                            {
                                g = grpInfo[j];
                                break;
                            }
                        }
                    }
                }
            }
        }

        return grpInfo;
    }

    static
    std::string composeGroupingString(const group_info_t &grpInfo)
    {
        std::string res; res.reserve(grpInfo.size()*2);
        for(auto g : grpInfo)
        {
            if (!res.empty())
                res.append(1,';');

            std::string strTmp;
            while(g)
            {
                strTmp.append(1, utils::digitToChar(g%10, false));
                g /= group_info_t::value_type(10u);
            }

            if (strTmp.empty())
                strTmp.append(1, '0');

            std::reverse(strTmp.begin(), strTmp.end());

            res.append(strTmp);
        }

        return res;
    }

    static
    group_info_t parseGroupingString(const std::string &str)
    {
        group_info_t resGrp;

        unsigned curValue = 0;
        bool valueGot = false;

        for(auto ch: str)
        {
            int d = utils::toDigit(ch);
            if (d<0)
            {
                if (valueGot)
                {
                    if (curValue>255)
                        curValue = 255;
                    groupInfoAppend(resGrp, 1, group_info_t::value_type(curValue));
                }

                valueGot = false;
                curValue = 0;

                continue;
            }

            valueGot  = true;
            curValue *= 10u;
            curValue += unsigned(d);
        }

        if (valueGot)
        {
            if (curValue>255)
                curValue = 255;
            groupInfoAppend(resGrp, 1, group_info_t::value_type(curValue));
        }

        return fixGroupInfo(std::move(resGrp));
    }

    // 3;0     3,000,000,000,000
    // 3;2;0   30,00,00,00,00,000
    // 3       3000000000,000
    // 3;2     30000000,00,000
    static
    bool testAppendGroupSeparator(std::size_t curDigitIndex, const group_info_t &grpInfo)
    {
        if (curDigitIndex==0) // группа не может начатся с самой первой позиции
            return false;

        if (grpInfo.empty() || grpInfo.front()==0u) // нет информации по группам, возвращаем признак, что не надо вставлять разделитель групп
            return false;

        std::uint8_t rptGrpSize = 0;
        if (grpInfo.size()>1) // У нас больше одного элемента
        {
            // просканируем до первого нуля, значение перед ним будет повторяющимся размером группы
            for(std::size_t i=0u; i!=grpInfo.size(); ++i)
            {
                if (grpInfo[i]==0)
                {
                    if (i)
                        rptGrpSize = grpInfo[i-1u];
                    break;
                }
            }
        }

        std::size_t  grpIdx = 0;

        while(true)
        {
            std::uint8_t g = 0;
            if (grpIdx>=grpInfo.size())
            {
                // у нас описания групп закончились, пробуем повторение
                if (!rptGrpSize)
                    return false; // если нет повторяющейся групп, то далее цифры идут сплошняком
                g = rptGrpSize; // размер группы - повторение последней
            }
            else // иначе получаем размер текущей группы
            {
                g = grpInfo[grpIdx];
                ++grpIdx;
            }

            if (curDigitIndex<g)
                return false;
            
            if (curDigitIndex==g)
                return true;

            curDigitIndex -= g;
        }

        return false; // поймет или нет, что мы сюда никогда не попадаем?
    }

    
}; // struct LocaleInfo

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
const LocaleInfo* getLocaleInfoInvariant()
{
    static LocaleInfo li;
    return &li;
}


 /* LOCALE_INVARIANT
LOCALE_SYSTEM_DEFAULT
LOCALE_USER_DEFAULT */ 

// enum class NumeralSystem : std::uint32_t
// {
//     invalid    = (std::uint32_t)(-1) /*!<  */,
//     unknown    = (std::uint32_t)(-1) /*!<  */,
//     none       = 0x00 /*!<  */,
//     currency   = 0x01 /*!<  */,
//     bin        = 0x02 /*!<  */,
//     oct        = 0x08 /*!<  */,
//     dec        = 0x0A /*!<  */,
//     hex        = 0x10 /*!<  */
//  
// }; // enum 


// enum class LocaleValueType : std::uint32_t
// {
//     invalid                  = (std::uint32_t)(-1) /*!<  */,
//     unknown                  = (std::uint32_t)(-1) /*!<  */,
//     none                     = 0x00 /*!<  */,
//     thousandSeparator        = 0x01 /*!< Separately for each numeral system */,
//     fractionalSeparator      = 0x02 /*!< same as decimal, but for fractional part of floating point numbers */,
//     signDecimal              = 0x03 /*!< Aka decimal separator, decimal mark, decimal point */,
//     signPlus                 = 0x04 /*!<  */,
//     signMinus                = 0x05 /*!<  */,
//     signPercent              = 0x06 /*!<  */,
//     signCurrency             = 0x07 /*!<  */,
//     formatPercentNegative    = 0x08 /*!<  */,
//     formatPercentPositive    = 0x09 /*!<  */,
//     formatCurrencyNegative   = 0x0A /*!<  */,
//     formatCurrencyPositive   = 0x0B /*!<  */
//  
// }; // enum 




} // namespace format
} // namespace marty
// marty::format::
// #include "marty_format/locale_info.h"
