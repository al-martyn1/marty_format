#pragma once

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormatString(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, const std::string &str);

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatUnsigned(FormattingOptions formattingOptions, const LocaleInfo *pLocaleInfo, IntType v, size_t valSize);

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename FloatType >
StringType martyFormatValueFormatFloat(FormattingOptions formattingOptions, const LocaleInfo *pLocaleInfo, FloatType v);

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, bool b);
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormat(FormattingOptions formattingOptions, const LocaleInfo *pUserLocaleInfo, const marty::BigInt &b);
