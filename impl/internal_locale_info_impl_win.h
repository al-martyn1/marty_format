#pragma once

//----------------------------------------------------------------------------
struct LocaleInfoWin : public LocaleInfo
{

    LocaleInfoWin(LCID locale) : LocaleInfo(), m_locale(locale)
    {
        init();
    }

    LocaleInfoWin(bool bSystem=false) : LocaleInfo(), m_locale(bSystem ? LOCALE_SYSTEM_DEFAULT : LOCALE_USER_DEFAULT)
    {
        init();
    }

    LocaleInfoWin(const LocaleInfoWin &) = default;
    LocaleInfoWin& operator=(const LocaleInfoWin &) = default;
    LocaleInfoWin(LocaleInfoWin &&) = default;
    LocaleInfoWin& operator=(LocaleInfoWin &&) = default;


protected:

    void init();

    std::size_t getLocaleInfo(LCTYPE LCType, wchar_t* buf, std::size_t bufSize);


    LCID m_locale; // пусть лежит

}; // struct LocaleInfoWin


//----------------------------------------------------------------------------
inline
std::size_t LocaleInfoWin::getLocaleInfo(LCTYPE LCType, wchar_t* buf, std::size_t bufSize)
{
    return (std::size_t)GetLocaleInfoW(m_locale, LCType, buf, (int)bufSize);
}


inline
void LocaleInfoWin::init()
{
    constexpr const std::size_t bufSize = 32;
    wchar_t buf[bufSize];

    std::size_t
    // LOCALE_SGROUPING - Sizes for each group of digits to the left of the decimal - https://learn.microsoft.com/en-us/windows/win32/intl/locale-sgrouping
    szRes = getLocaleInfo(LOCALE_SGROUPING, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        auto strGroupping = marty::utf::string_from_wstring(buf);
        auto grpInfo = LocaleInfo::parseGroupingString(strGroupping);

        // Для валюты и десятичных чисел используем локализованые правила разделения на группы, остальное - как у базовой локали
        // Есть ли отдельное правило для валюты?
        // Есть ли отдельное правило для дробной части?
        groupInfoCur[0] = groupInfoCur[1] = grpInfo;
        groupInfoDec[0] = groupInfoDec[1] = grpInfo;
    }

    // LOCALE_STHOUSAND - Characters that are used to separate groups of digits to the left of the decimal.
    //                    https://learn.microsoft.com/en-us/windows/win32/intl/locale-sthousand
    szRes = getLocaleInfo(LOCALE_STHOUSAND, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        auto strSep = marty::utf::string_from_wstring(buf);

        // Для валюты и десятичных чисел используем локализованый разделитель, остальное - как у базовой локали
        // Есть ли отдельное правило для валюты?
        // Есть ли отдельное правило для дробной части?
        groupSeparatorCur[0] = groupSeparatorCur[1] = strSep;
        groupSeparatorDec[0] = groupSeparatorDec[1] = strSep;
    }

    // LOCALE_SDECIMAL - Character(s) used for the decimal separator, for example, "." in "3.14" or "," in "3,14" - https://learn.microsoft.com/en-us/windows/win32/intl/locale-sdecimal
    szRes = getLocaleInfo(LOCALE_SDECIMAL, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        signDecimal = marty::utf::string_from_wstring(buf);
    }

    // LOCALE_SPOSITIVESIGN - Localized string value for the positive sign for the locale
    szRes = getLocaleInfo(LOCALE_SPOSITIVESIGN, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        signPlus = marty::utf::string_from_wstring(buf);
    }

    // LOCALE_SNEGATIVESIGN - https://learn.microsoft.com/en-us/windows/win32/intl/locale-snegativesign
    szRes = getLocaleInfo(LOCALE_SNEGATIVESIGN, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        signMinus = marty::utf::string_from_wstring(buf);
    }

    // LOCALE_SPERCENT -  Symbol used to indicate percentage, for example, "%". The value is a single string of 0 to 3 characters.
    szRes = getLocaleInfo(LOCALE_SPERCENT, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        signPercent = marty::utf::string_from_wstring(buf);
    }

    // LOCALE_SCURRENCY - String used as the local monetary symbol - https://learn.microsoft.com/en-us/windows/win32/intl/locale-scurrency
    szRes = getLocaleInfo(LOCALE_SCURRENCY, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        signCurrency = marty::utf::string_from_wstring(buf);
    }

    // LOCALE_INEGATIVEPERCENT - Negative percentage formatting pattern for the locale - https://learn.microsoft.com/en-us/windows/win32/intl/locale-inegativepercent 
    szRes = getLocaleInfo(LOCALE_INEGATIVEPERCENT, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        unsigned u = 0;
        utils::simpleParseDecimal(&buf[0], &buf[szRes-1], &u);
        switch(u)
        {
            case 0 : formatPercentShortNegative = "-#";     // Negative sign, number, space, percent; for example, -# %
                     formatPercentNegative = "-# %"; break;
            case 1 : formatPercentShortNegative = "-#";     // Negative sign, number, percent; for example, -#%
                     formatPercentNegative = "-#%" ; break;
            case 2 : formatPercentShortNegative = "-#";     // Negative sign, percent, number; for example, -%#
                     formatPercentNegative = "-%#" ; break;
            case 3 : formatPercentShortNegative = "-#";     // Percent, negative sign, number; for example, %-#
                     formatPercentNegative = "%-#" ; break;
            case 4 : formatPercentShortNegative = "#-";     // Percent, number, negative sign; for example, %#-
                     formatPercentNegative = "%#-" ; break;
            case 5 : formatPercentShortNegative = "#-";     // Number, negative sign, percent; for example, #-%
                     formatPercentNegative = "#-%" ; break;
            case 6 : formatPercentShortNegative = "#-";     // Number, percent, negative sign; for example, #%-
                     formatPercentNegative = "#%-" ; break;
            case 7 : formatPercentShortNegative = "- #";    // Negative sign, percent, space, number; for example, -% #
                     formatPercentNegative = "-% #"; break;
            case 8 : formatPercentShortNegative = "# -";    // Number, space, percent, negative sign; for example, # %-
                     formatPercentNegative = "# %-"; break;
            case 9 : formatPercentShortNegative = "#-";     // Percent, space, number, negative sign; for example, % #-
                     formatPercentNegative = "% #-"; break;
            case 10: formatPercentShortNegative = "-#";     // Percent, space, negative sign, number; for example, % -#
                     formatPercentNegative = "% -#"; break;
            case 11: formatPercentShortNegative = "#-";     // Number, negative sign, space, percent; for example, #- %
                     formatPercentNegative = "#- %"; break;
        }
    }

    // LOCALE_IPOSITIVEPERCENT - Positive percentage formatting pattern for the locale - https://learn.microsoft.com/en-us/windows/win32/intl/locale-ipositivepercent 
    szRes = getLocaleInfo(LOCALE_IPOSITIVEPERCENT, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        unsigned u = 0;
        utils::simpleParseDecimal(&buf[0], &buf[szRes-1], &u);
        switch(u)
        {
            case 0: formatPercentShortPositive = "#";     // Number, space, percent; for example, # %
                    formatPercentPositive = "# %"; break;
            case 1: formatPercentShortPositive = "#";     // Number, percent; for example, #%
                    formatPercentPositive = "#%" ; break;
            case 2: formatPercentShortPositive = "#";     // Percent, number; for example, %#
                    formatPercentPositive = "%#" ; break;
            case 3: formatPercentShortPositive = "#";     // Percent, space, number; for example, % #
                    formatPercentPositive = "% #"; break;
        }
    }

    // LOCALE_ICURRENCY - Position of the monetary symbol in the positive currency mode - https://learn.microsoft.com/en-us/windows/win32/intl/locale-icurrency
    szRes = getLocaleInfo(LOCALE_ICURRENCY, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        std::string formatCurrency = "$#";
        std::string formatCurrencyShort = "#";
        unsigned u = 0;
        utils::simpleParseDecimal(&buf[0], &buf[szRes-1], &u);
        switch(u)
        {
            case 0: formatCurrencyShort = "#";     // Prefix, no separation, for example, $1.1
                    formatCurrency = "$#" ; break;
            case 1: formatCurrencyShort = "#";     // Suffix, no separation, for example, 1.1$
                    formatCurrency = "#$" ; break;
            case 2: formatCurrencyShort = "#";     // Prefix, 1-character separation, for example, $ 1.1
                    formatCurrency = "$ #"; break;
            case 3: formatCurrencyShort = "#";     // Suffix, 1-character separation, for example, 1.1 $
                    formatCurrency = "# $"; break;
        }

        formatCurrencyNegative      = "-" + formatCurrency;
        formatCurrencyPositive      = "+" + formatCurrency;
        formatCurrencyShortNegative = "-" + formatCurrencyShort;
        formatCurrencyShortPositive = "+" + formatCurrencyShort;
    }

    // LOCALE_IDIGITS - Number of fractional digits placed after the decimal separator - https://learn.microsoft.com/en-us/windows/win32/intl/locale-idigits
    szRes = getLocaleInfo(LOCALE_IDIGITS, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        // auto str = marty::utf::string_from_wstring(buf);
        utils::simpleParseDecimal(&buf[0], &buf[szRes-1], &(digitsNumber));
    }
     
    // LOCALE_ICURRDIGITS - Number of fractional digits for the local monetary format - https://learn.microsoft.com/en-us/windows/win32/intl/locale-icurrdigits
    szRes = getLocaleInfo(LOCALE_ICURRDIGITS, buf, bufSize);
    if (szRes)
    {
        buf[szRes-1] = 0; // на всякий случай
        // auto str = marty::utf::string_from_wstring(buf);
        utils::simpleParseDecimal(&buf[0], &buf[szRes-1], &(digitsCurrency));
    }

    // // 
    // szRes = getLocaleInfo(, buf, bufSize);
    // if (szRes)
    // {
    //     buf[szRes-1] = 0; // на всякий случай
    //      = marty::utf::string_from_wstring(buf);
    // }
    //  


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


}



// LOCALE_ICURRDIGITS - Number of fractional digits for the local monetary format - https://learn.microsoft.com/en-us/windows/win32/intl/locale-icurrdigits
// LOCALE_ICURRENCY - Position of the monetary symbol in the positive currency mode - https://learn.microsoft.com/en-us/windows/win32/intl/locale-icurrency

