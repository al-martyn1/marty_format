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
     Что мы хотим от локали?
     Группировка разрядов. Для 2/8/10/16 СС (Numeral system - Система счисления)
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

//----------------------------------------------------------------------------
//! Интерфейс локали для форматирования, он же базовая реализация для "C" локали
/*! 
    В библиотеке получение информации из локали всегда производится через
    виртуальные методы. Их можно переопределить в наследнике для получения
    какого-то дополнительного эффекта.

    Все параметры локали являются открытыми полями класса. Для того, чтобы получить
    модифицированную локаль, достаточно скопировать какую-либо из предопределённых,
    изменить поля по необходимости, и использовать её.

    Форматные строки используются для того, чтобы задать простые базовые правила
    форматирования различных видов чисел: числовое значение, процент, валюта.

    В форматных строках используются знаки для подстановки
    '#' - сюда подставляется отформатированное значение числа (строка)
    '-' - сюда подставляется символ минуса (строка)
    '+' - сюда подставляется символ плюса или пробельный символ (строка)
    '%' - сюда подставляется символ процента (строка)
    '$' - сюда подставляется символ валюты (строка)

*/
//#! LocaleInfo
struct LocaleInfo
{
    virtual ~LocaleInfo() {}

    LocaleInfo();

    LocaleInfo(const LocaleInfo &) = default;
    LocaleInfo& operator=(const LocaleInfo &) = default;
    LocaleInfo(LocaleInfo &&) = default;
    LocaleInfo& operator=(LocaleInfo &&) = default;


// Для отладки удобнее std::vector<std::uint8_t>, а в релизе std::basic_string<std::uint8_t> гораздо быстрее
#if defined(DEBUG) || defined(_DEBUG)
    using group_info_t = std::vector<std::uint8_t>;
    static void groupInfoAppend(group_info_t &gi, std::size_t n, std::uint8_t u) { gi.resize(gi.size()+n, u); }
#else
    using group_info_t = std::basic_string<std::uint8_t>;
    static void groupInfoAppend(group_info_t &gi, std::size_t n, std::uint8_t u) { gi.append(n, u); }
#endif


public: // members

    // Integer part/Fractional part
    group_info_t groupInfoCur[2]; // Currency
    group_info_t groupInfoBin[2];
    group_info_t groupInfoOct[2];
    group_info_t groupInfoDec[2];
    group_info_t groupInfoHex[2];

    // Integer part/Fractional part
    std::string groupSeparatorCur[2];
    std::string groupSeparatorBin[2];
    std::string groupSeparatorOct[2];
    std::string groupSeparatorDec[2];
    std::string groupSeparatorHex[2];

    std::string signDecimal            = ".";  
    std::string signPlus               = "+";  
    std::string signMinus              = "-";  
    std::string signPercent            = "%";  
    std::string signCurrency           = "$";  

    std::string formatPercentNegative  = "-#%";
    std::string formatPercentPositive  = "+#%";
    std::string formatCurrencyNegative = "-$#";
    std::string formatCurrencyPositive = "+$#";

    // Числа любой СС форматируем по тем же правилам.
    // Только перед подстановкой в форматную строку строки числа могут быть дополнены
    // или пробелами или нулями, в зависимости от режима выравнивания
    std::string formatNumberNegative   = "-#";
    std::string formatNumberPositive   = "+#";

    unsigned digitsNumber              = 3;
    unsigned digitsCurrency            = 2;


public: // virtual methods

    //! Обрабатывает только LocaleInfoValueType::thousandSeparator и LocaleInfoValueType::fractionalSeparator
    virtual std::string getGroupSeparator( LocaleInfoSeparatorType vt //! 
                                         , NumeralSystem ns       //!
                                         ) const;

    //! Обрабатывает LocaleInfoValueType::sign* и LocaleInfoValueType::format*
    virtual std::string getLocaleInfoValue(LocaleInfoValueType vt) const;

    virtual group_info_t getGroupInfo(NumeralSystem ns, bool bFractionalPart) const;

    virtual std::string substFormatString( const std::string &fmt
                                         , const std::string &numStr
                                         , PositiveNumbersMode pm
                                         ) const;

    // digitsDecimal / digitsCurrency
    virtual unsigned getNumberOfDigits(LocaleInfoDigitsType vt) const;


public: // static helper methods

    //! Подстановка локализованных символов в шаблон форматной строки
    static
    std::string substFormatString( const std::string &fmt
                                 , const std::string &numStr
                                 , const std::string &symbolPlus
                                 , const std::string &symbolMinus
                                 , const std::string &symbolPercent
                                 , const std::string &symbolCurrency
                                 , PositiveNumbersMode pm
                                 );

    //! Нулем может быть только последний элемент.
    /*! Данная функция фиксит разобранное из строки описание групп
        При дальнейшей работе с группами элемент с нулевым размером 
        будет считаться последним
     */
    static group_info_t fixGroupInfo(group_info_t &&grpInfo);

    //! Разбор строк вида "3;2;0" в group_info_t
    static group_info_t parseGroupingString(const std::string &str);

    //! Восстановление group_info_t в строку
    static std::string composeGroupingString(const group_info_t &grpInfo);


    // 3;0     3,000,000,000,000
    // 3;2;0   30,00,00,00,00,000
    // 3       3000000000,000
    // 3;2     30000000,00,000
    static bool testAppendGroupSeparator(std::size_t curDigitIndex, const group_info_t &grpInfo);

    static std::string insertGroupSeparators(std::string numStr, std::string sep, const group_info_t &grpInfo, bool bFractionalPart);

    static std::string expandWithGroupSeparator( std::string numStr, std::string sep, const group_info_t &grpInfo
                                               , bool bFractionalPart
                                               , std::size_t sepCalculatedLen  // Посчитанная снаружи длина разделителя
                                               , std::size_t &numStrLen        // Посчитанная снаружи полная длина строки, которую дополняем, включая сепараторы
                                               , std::size_t &digitsCount
                                               , std::size_t maxLen
                                               );
    static std::string expandWithGroupSeparatorToNumDigits( std::string numStr, std::string sep, const group_info_t &grpInfo
                                               , bool bFractionalPart
                                               , std::size_t &digitsCount
                                               , std::size_t maxLen            // Макс длина в цифрах
                                               );

protected: // static helper methods

    static std::string insertGroupSeparatorsImplHelper(const std::string &numStr, const std::string &sep, const group_info_t &grpInfo);
    static std::string expandWithGroupSeparatorImplHelper( std::string numStr, const std::string &sep, const group_info_t &grpInfo
                                                         , std::size_t sepCalculatedLen // Посчитанная снаружи длина разделителя
                                                         , std::size_t &numStrLen       // Посчитанная снаружи полная длина строки, которую дополняем, включая сепараторы
                                                         , std::size_t &digitsCount
                                                         , std::size_t maxLen
                                                         );
    static std::string expandWithGroupSeparatorToNumDigitsImplHelper( std::string numStr, const std::string &sep, const group_info_t &grpInfo
                                                         , std::size_t &digitsCount
                                                         , std::size_t maxLen           // Макс длина в цифрах
                                                         );
    
}; // struct LocaleInfo
//#!

#include "impl/internal_locale_info_impl_base.h"


//----------------------------------------------------------------------------
inline
const LocaleInfo* getLocaleInfoInvariant()
{
    static LocaleInfo li;
    return &li;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#if defined(WIN32) || defined(_WIN32)

#include "impl/internal_locale_info_impl_win.h"

//----------------------------------------------------------------------------
inline
const LocaleInfo* getLocaleInfoUser()
{
    static auto li = LocaleInfoWin(false);
    return &li;
}

//----------------------------------------------------------------------------
inline
const LocaleInfo* getLocaleInfoSystem()
{
    static auto li = LocaleInfoWin(true);
    return &li;
}

//----------------------------------------------------------------------------
// inline
// const LocaleInfo* getLocaleInfo(bool bSystem=false)
// {
//     return bSystem ? getLocaleInfoSystem() : getLocaleInfoUser();
// }

//----------------------------------------------------------------------------


#endif
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//#! getLocaleInfo
const LocaleInfo* getLocaleInfo(LocaleInfoType lt);
//#!

inline
const LocaleInfo* getLocaleInfo(LocaleInfoType lt)
{
    if (lt==LocaleInfoType::invariant)
        return getLocaleInfoInvariant();

#if defined(WIN32) || defined(_WIN32)
    if (lt==LocaleInfoType::user)
        return getLocaleInfoUser();
    if (lt==LocaleInfoType::system)
        return getLocaleInfoSystem();
#else
    if (lt==LocaleInfoType::user || lt==LocaleInfoType::system)
        return getLocaleInfoInvariant();
#endif

    throw std::invalid_argument("marty::format::getLocaleInfo: invalid LocaleInfoType taken");
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
} // namespace format
} // namespace marty
// marty::format::
// #include "marty_format/locale_info.h"
