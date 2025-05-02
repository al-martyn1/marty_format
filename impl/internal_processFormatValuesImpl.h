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
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormatString(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, const std::string &str);

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatUnsigned(FormattingOptions formattingOptions, const LocaleInfo *pLocaleInfo, IntType v, size_t valSize);

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename FloatType >
StringType martyFormatValueFormatFloat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, FloatType v);

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Данный шаблон объявлен в utils.h, чтобы компилятор видел хоть какую-то реализацию функции
// martyFormatSimpleConvertToString
template<typename StringType> inline StringType martyFormatSimpleConvertToString(bool b)                  { return StringType(b ? "true" : "false" ); }
//
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
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, bool b);

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormatPointer(FormattingOptions formattingOptions, const LocaleInfo *pLocaleInfo, std::uintptr_t p, bool formatNativePtr=false)
{
    MARTY_ARG_USED(formatNativePtr);
    MARTY_ARG_USED(pLocaleInfo);

    if (formattingOptions.typeChar!='p' && formattingOptions.typeChar!='P')
        formattingOptions.typeChar = 'P';
    formattingOptions.width = 0; // Форматируем по ширине целиком по размеру типа
    formattingOptions.alignment = '=';
    formattingOptions.optionsFlags |= FormattingOptionsFlags::signZero;

    // Надо обдумать, как форматировать сегментный адрес
    return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, p, sizeof(std::uintptr_t));
}

//----------------------------------------------------------------------------
inline
const LocaleInfo* findLocaleInfo(const FormattingOptions &formattingOptions, const LocaleInfo *pUserLocaleInfo, bool useFormatStringLocale)
{
    // Для начала 
    const LocaleInfo *pLocaleInfo = getLocaleInfo(LocaleInfoType::invariant);

    // Вне зависимости от форматной строки используем кастомную локаль, которую нам передал пользователь
    // Если пользователь ничего не передал, то уровнем выше берётся юзер/систем локаль
    if ((formattingOptions.formattingFlags&FormattingFlags::localeForceCustom)!=0)
    {
        if (pUserLocaleInfo)
            pLocaleInfo = pUserLocaleInfo;

        return pLocaleInfo;
    }

    if (!useFormatStringLocale)
        return pLocaleInfo; // в форматной строке не задано использование локали, возвращаем "сишную" invariant локаль

    return pUserLocaleInfo ? pUserLocaleInfo : pLocaleInfo;
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename FloatType >
StringType martyFormatValueFormatFloat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, FloatType v)
{
    #if 0

    const bool localeFormattingOpt = ((formattingOptions.optionsFlags&FormattingOptionsFlags::localeFormatting)!=0); // L option char - C++
    const bool useLocale = (typeChar=='n') || localeFormattingOpt;
    const LocaleInfo *pLocaleInfo = findLocaleInfo(formattingOptions, pUserLocaleInfo, useLocale);

    bool bNegative = std::signbit(v); // v<FloatType(0.0);

    auto typeChar = formattingOptions.typeChar;
    std::string formatString;

    if (!typeChar)
    {
        typeChar = 'f';
    }
    else if (typeChar=='%')
    {
        typeChar = 'f';
        v *= FloatType(100.0);
        formatString = pLocaleInfo->getLocaleInfoValue(bNegative ? LocaleInfoValueType::formatPercentNegative : LocaleInfoValueType::formatPercentPositive);
    }
    else if (typeChar=='$')
    {
        typeChar = 'f';
        formatString = pLocaleInfo->getLocaleInfoValue(bNegative ? LocaleInfoValueType::formatCurrencyNegative : LocaleInfoValueType::formatCurrencyPositive);
    }
    else if (typeChar=='d')
    {
        typeChar = 'f';
    }
    else
    {
        formatString = pLocaleInfo->getLocaleInfoValue(bNegative ? LocaleInfoValueType::formatNumberNegative : LocaleInfoValueType::formatNumberPositive);
    }

    if (typeChar!='a' && typeChar!='A' && typeChar!='e' && typeChar!='E' && typeChar!='f' && typeChar!='F' && typeChar!='g' && typeChar!='G')
    {
        if ((formattingOptions.formattingFlags&FormattingFlags::ignoreTypeMismatchErrors)==0)
            throw mismatch_format_type("invalid format type for float argument");

        typeChar = 'f';
    }


    std::size_t signIdx  = std::size_t(-1);
    std::size_t powerIdx = std::size_t(-1);



// std::string formatFloat( T value, char spec
//                        , std::size_t *pSignIdx  = 0
//                        , std::size_t *pPowerIdx = 
//                        , int precision = -1
//                        )

    #endif

  // static FMT_CONSTEXPR_DECL const int double_significand_size =
  //     std::numeric_limits<double>::digits - 1;
  // static FMT_CONSTEXPR_DECL const uint64_t implicit_bit =
  //     1ULL << double_significand_size;
  // static FMT_CONSTEXPR_DECL const int significand_size =
  //     bits<significand_type>::value;

    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(pLocaleInfo);
    return martyFormatSimpleConvertToString<StringType>(v);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatUnsigned(FormattingOptions formattingOptions, const LocaleInfo *pUserLocaleInfo, IntType v, size_t valSize)
{
    MARTY_ARG_USED(valSize);

    if (formattingOptions.typeChar=='t' || formattingOptions.typeChar=='T' || formattingOptions.typeChar=='y' || formattingOptions.typeChar=='Y')
        return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, pUserLocaleInfo, (v==0 ? false : true) );

    bool bNegative = ((formattingOptions.optionsFlags&FormattingOptionsFlags::internalNegative)!=0) ? true : false;

    if (formattingOptions.typeChar=='$')
    {
        double d = double(v);
        return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, pUserLocaleInfo, bNegative ? -d :d);
    }

    if (formattingOptions.typeChar=='c')
        return martyFormatValueFormatString<WidthCalculator, StringType>(formattingOptions, pUserLocaleInfo, utils::charToStringUtf8(utf32_char_t(v)));


    if (formattingOptions.typeChar=='p' || formattingOptions.typeChar=='P')
    {
        formattingOptions.typeChar = (formattingOptions.typeChar=='p') ? 'x' : 'X';
        formattingOptions.width = 0; // Форматируем по ширине целиком по размеру типа
        formattingOptions.alignment = '=';
        formattingOptions.optionsFlags |= FormattingOptionsFlags::signZero;

        if (bNegative)
        {
            // Нам упало из обработчика знаковых чисел, надо обратно переделать в знаковый, поменять знак, и потом сбиткастить в беззнаковый
            // Нам упало не просто знаковое, но отрицательное знаковое
            // Делаем обратное преобразование
            using SignedT = std::make_signed_t<IntType>;
            SignedT signedValueAbs = utils::toSignedCast(v);
            SignedT signedValue    = -signedValueAbs;
            v = utils::toUnsignedCast(signedValue);

            // Сбрасываем все признаки бывшей знаковости
            formattingOptions.optionsFlags &= ~FormattingOptionsFlags::internalSigned;
            formattingOptions.optionsFlags &= ~FormattingOptionsFlags::internalNegative;
        }
    }


    auto typeChar = formattingOptions.typeChar;

    if (typeChar==0)
        typeChar = 'd';


    std::string formatString;
    // По дефолту используем "C" (invariant) локаль
    //const LocaleInfo *pLocaleInfo = getLocaleInfo(LocaleInfoType::invariant);

    const bool localeFormattingOpt = ((formattingOptions.optionsFlags&FormattingOptionsFlags::localeFormatting)!=0); // L option char - C++
    const bool useLocale = (typeChar=='n') || localeFormattingOpt;
    const LocaleInfo *pLocaleInfo = findLocaleInfo(formattingOptions, pUserLocaleInfo, useLocale);

    if (typeChar=='n')
        typeChar = 'd';

    formatString = pLocaleInfo->getLocaleInfoValue(bNegative ? LocaleInfoValueType::formatNumberNegative : LocaleInfoValueType::formatNumberPositive);

    if (typeChar=='%')
    {
        typeChar = 'd';
        formatString = pLocaleInfo->getLocaleInfoValue(bNegative ? LocaleInfoValueType::formatPercentNegative : LocaleInfoValueType::formatPercentPositive);
    }

    if (formatString.empty() || formatString.find('#')==formatString.npos)
    {
        formatString = bNegative ? "-#" : "+#";
    }


    if (typeChar!='b' && typeChar!='B' && typeChar!='d' && typeChar!='o' && typeChar!='O' && typeChar!='x' && typeChar!='X' && typeChar!='h' && typeChar!='H')
    {
        // type            ::= "a" | "A" | "b" | "B" | "c" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "n" | 
        //                     "o" | "p" | "s" | "S" | "t" | "T" | "x" | "X" | "y" | "Y" | "%"
        if ((formattingOptions.formattingFlags&FormattingFlags::ignoreTypeMismatchErrors)==0)
            throw mismatch_format_type("invalid format type for int/unsigned argument");

        typeChar = 'd';
    }


    const bool isNumberUpper = (formattingOptions.typeChar=='B' || formattingOptions.typeChar=='O' || formattingOptions.typeChar=='X' || formattingOptions.typeChar=='H');
    const bool caseInvert    = ((formattingOptions.optionsFlags&FormattingOptionsFlags::caseInvert)!=0);
    /* case    bInvert    prefix
        0         0          0
        0         1          1
        1         0          1
        1         1          0

        XOR (!=)
    */
    const bool prefixUpper = (isNumberUpper != caseInvert);
    auto fillChar = formattingOptions.fillChar;

    const bool prefixOptional = (typeChar=='o' || typeChar=='O'); // Если префикс опциональный и первая цифра - 0, то префикс не добавляется
    std::string prefixStr;
    std::string postfixStr;

    NumeralSystem numeralSystem = NumeralSystem::dec;

    if (typeChar=='b' || typeChar=='B')
    {
        numeralSystem = NumeralSystem::bin;
        prefixStr.assign(prefixUpper?"0B":"0b");
    }
    else if (typeChar=='d')
    {
    }
    else if (typeChar=='o')
    {
        numeralSystem = NumeralSystem::oct;
        prefixStr.assign(1, '0');
    }
    else if (typeChar=='x' || typeChar=='X')
    {
        numeralSystem = NumeralSystem::hex;
        prefixStr.assign(prefixUpper?"0X":"0x");
    }
    else if (typeChar=='h' || typeChar=='H')
    {
        numeralSystem = NumeralSystem::hex;
        postfixStr.assign(prefixUpper?"H":"h");
    }


    std::string grpSepStr;

    // Разделение на группы разрядов делаем только если указан какой-то из символов разделителей групп разрядов
    // Даже если у нас не "сишная", а пользовательская, всё равно надо явно указать, что используем разделитель
    // Или нет?
    // Для чисел с плавающей точкой мы можем хотеть вывести число с десятичной точкой из локали,
    // но не разделять группы разрядов
    // Поэтому использование разделителя надо указать явно.
    // Для целых чисел использование разделителя надо указать явно для единообразности правил.

    if (formattingOptions.grouppingChar!=0) // Задан какой-то из разделителей - '\'', '_', ','
    {
        if (useLocale) // используем разделитель разрядов из локали
        {
            grpSepStr = pLocaleInfo->getGroupSeparator(LocaleInfoSeparatorType::thousands, numeralSystem);
        }
        else // используем явно заданный разделитель разрядов
        {
            auto outIt = marty::utf::UtfOutputIterator<char>(grpSepStr);
            *outIt++ = marty::utf::utf32_char_t(formattingOptions.grouppingChar);
        }
    }

    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signAlterForm)==0)
    {
        // Очищаем префикс и постфикс - "альтернативная" форма не задана
        prefixStr.clear(); 
        postfixStr.clear();
    }

    auto alignment = formattingOptions.alignment;
    if (alignment==0)
    {
        if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signZero)==0)
            alignment = '<';
        else
            alignment = '=';
    }

    if (fillChar<32 || WidthCalculator()(fillChar)<1)
        fillChar = utf32_char_t(' ');


    std::string numStr = utils::simpleToString(v, numeralSystem, isNumberUpper);


    //FormattingOptionsFlags signOpt = FormattingOptionsFlags::signMinus; // (this is the default behavior)
    // Sign, #, and 0
    // The sign option can be one of following:
    // +: Indicates that a sign should be used for both non-negative and negative numbers. The + sign is inserted before the output value for non-negative numbers.
    // -: Indicates that a sign should be used for negative numbers only (this is the default behavior).
    // space: Indicates that a leading space should be used for non-negative numbers, and a minus sign for negative numbers.
    // Negative zero is treated as a negative number.

    PositiveNumbersMode positiveNumbersMode = PositiveNumbersMode::none;

    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signPlus)!=0)
        positiveNumbersMode = PositiveNumbersMode::sign;

    else if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signSpace)!=0)
        positiveNumbersMode = PositiveNumbersMode::space;


    std::size_t grpSepLen = width_t(WidthCalculator()(grpSepStr.data(), grpSepStr.size()));

    std::size_t numStrLen     = numStr.size(); // число цифровых символов в строке
    // std::size_t numStrFullLen = numStr.size(); // число символов вместе с сепараторами

    LocaleInfo::group_info_t grpInfo = pLocaleInfo->getGroupInfo(numeralSystem, false /* !bFractionalPart */);

    numStr = pLocaleInfo->insertGroupSeparators(numStr, grpSepStr, grpInfo, false /* !bFractionalPart */);
    std::size_t numStrFullLen = WidthCalculator()(numStr.data(), numStr.size()); // число символов вместе с сепараторами

    // Шестнадцатиричные числа по умолчанию форматируются как в питоне с символом '='
    // - заполнение вставляется между знаком и/или префиксом, при этом, если задан '0'
    // первым символом ширины поля, то идёт заполнение нулём до знака или префикса.
    // Если '0' не задан, то форматирует по правому краю.

    // Мы сделаем немного по другому. Если задано выравнивание по ширине ('='),
    // то если задан '0', то дополняем им до нужной ширины до вставки префикса и знака.
    // Если '0' не задан, то префикс, если есть, добавляем вплотную к отформатированному числу,
    // затем добиваем символом заполнения, потом добавляем знак.

    // Во всех других случаях форматируем минимально компактно, потом выводим с выравниванием, как строку.

    if (alignment!='=')
    {
        numStr = prefixStr + numStr + postfixStr;
        numStr = pLocaleInfo->substFormatString(formatString, numStr, positiveNumbersMode);
        return martyFormatValueFormatString<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, numStr);
    }


    std::size_t fieldWidth = formattingOptions.width;

    // Форматирование по ширине. Либо добиванием нулей, либо символом fillChar

    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signZero)==0)
    {
        // Добиваем fillChar'ами спереди

        numStr = prefixStr + numStr + postfixStr;

        std::size_t fillCount = 0;

        // Пробуем составить финальную строку из того, что уже есть
        {   
            std::string tmpFinal = pLocaleInfo->substFormatString(formatString, numStr, positiveNumbersMode);
            std::size_t tmpWidth = WidthCalculator()(tmpFinal.data(), tmpFinal.size());

            // Найдена длина финальной строки после подстановки строки с числом (с префиксами/постфиксами)
            
            if (tmpWidth<fieldWidth)
            {
                // Полученная финальная строка короче заданной ширины поля, надо расширять
                fillCount = fieldWidth - tmpWidth;
            }
        }

        numStr = utils::expandBefore(numStr, fillCount, utils::charToStringUtf8(fillChar));
        return pLocaleInfo->substFormatString(formatString, numStr, positiveNumbersMode);
    }

    // ZeroFlag установлен

    // Нам надо добить строку до нужной ширины ведущими нулями перед тем, как добавить префикс/суффикс/постфикс

    if (fieldWidth==0 && (numeralSystem==NumeralSystem::bin || numeralSystem==NumeralSystem::hex)) 
    {
        // Задана нулевая ширина поля - задаём по размеру целого типа
        // и добиваем до нужного количества цифр
        std::size_t requiredNumDigits = (numeralSystem==NumeralSystem::bin) ? std::size_t(valSize*8u) : std::size_t(valSize*2u);
        numStr = pLocaleInfo->expandWithGroupSeparatorToNumDigits(numStr, grpSepStr, grpInfo, false, numStrLen, requiredNumDigits);
        numStr = prefixStr + numStr + postfixStr;
        return pLocaleInfo->substFormatString(formatString, numStr, positiveNumbersMode);
    }


    if (!prefixOptional)
    {
        // Префикс добавляется безусловно

        // Пробуем составить финальную строку из того, что уже есть, с префиксами/постфиксами,
        // но без самого числа, и получить её длину
        std::size_t numStrWidth = 0; // длина/ширина, до которой надо расширить числовую строку без префиксов и суффиксов
        {
            std::string tmpFinal = pLocaleInfo->substFormatString(formatString, prefixStr + postfixStr, positiveNumbersMode);
            numStrWidth = WidthCalculator()(tmpFinal.data(), tmpFinal.size());
        }

        numStr = pLocaleInfo->expandWithGroupSeparator( numStr, grpSepStr, grpInfo, false /* !bFractionalPart */
                                                      , grpSepLen
                                                      , numStrFullLen // Посчитанная снаружи полная длина строки, которую дополняем, включая сепараторы
                                                      , numStrLen     // aka digitsCount
                                                      , numStrWidth   // maxLen
                                                      );
        numStr = prefixStr + numStr + postfixStr;
        return pLocaleInfo->substFormatString(formatString, numStr, positiveNumbersMode);
    }

    // У нас условный префикс - если отформатированное число начинается с нуля,
    // то префикс не добавляется
    std::size_t numStrWidth = 0; // длина/ширина, до которой надо расширить числовую строку без префиксов и суффиксов
    {
        std::string tmpFinal = pLocaleInfo->substFormatString(formatString, prefixStr + postfixStr, positiveNumbersMode);
        numStrWidth = WidthCalculator()(tmpFinal.data(), tmpFinal.size());
    }

    auto numStrFullLen1 = numStrFullLen;
    auto numStrLen1     = numStrLen;
    auto numStr1 = pLocaleInfo->expandWithGroupSeparator( numStr, grpSepStr, grpInfo, false /* !bFractionalPart */
                                                        , grpSepLen
                                                        , numStrFullLen1 // Посчитанная снаружи полная длина строки, которую дополняем, включая сепараторы
                                                        , numStrLen1     // aka digitsCount
                                                        , numStrWidth    // maxLen
                                                        );
    if (numStr1.empty() || numStr1.front()!='0')
    {
        // Нуля спереди нет, поэтому всё идёт штатно
        numStr1 = prefixStr + numStr1 + postfixStr;
        return pLocaleInfo->substFormatString(formatString, numStr1, positiveNumbersMode);
    }
    
    // Спереди ноль, префикс не нужен
    // пытаемся расширить, исключив префикс
    auto numStrFullLen2 = numStrFullLen;
    auto numStrLen2     = numStrLen;
    auto numStrWidth2    = numStrWidth;
    if (numStrWidth2>=prefixStr.size())
        numStrWidth2 -= prefixStr.size();
    else
        numStrWidth2 = 0;

    auto numStr2 = pLocaleInfo->expandWithGroupSeparator( numStr, grpSepStr, grpInfo, false /* !bFractionalPart */
                                                        , grpSepLen
                                                        , numStrFullLen2 // Посчитанная снаружи полная длина строки, которую дополняем, включая сепараторы
                                                        , numStrLen2     // aka digitsCount
                                                        , numStrWidth2   // maxLen
                                                        );
    if (numStr2.empty() || numStr2.front()!='0')
    {
        // Нуля спереди нет, префикс нужен
        // Но мы формировали эту строку без учёта префикса
        // Откатываемся к первой версии
        numStr1 = prefixStr + numStr1 + postfixStr;
        return pLocaleInfo->substFormatString(formatString, numStr1, positiveNumbersMode);
    }
    
    // У второй строки есть ведущий ноль, всё нормально
    numStr2 = prefixStr + numStr2 + postfixStr;
    return pLocaleInfo->substFormatString(formatString, numStr2, positiveNumbersMode);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatInt(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, IntType v, size_t valSize)
{
    MARTY_ARG_USED(pLocaleInfo);

    // Надо добавить форматный символ ~ - который делает битовый каст к беззнаковому
    // // шестнадцатиричное и двоичные числа битово кастим и отображаем как беззнаковые
    if ( (formattingOptions.optionsFlags&FormattingOptionsFlags::bitCast)!=0
      && (formattingOptions.typeChar=='b' || formattingOptions.typeChar=='B' || formattingOptions.typeChar=='x' || formattingOptions.typeChar=='X' || formattingOptions.typeChar=='h' || formattingOptions.typeChar=='H')
       )
    {
        return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, utils::toUnsignedCast(v), valSize);
    }

    if (formattingOptions.typeChar=='p' || formattingOptions.typeChar=='P')
    {
        return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, utils::toUnsignedCast(v), valSize);
    }

    FormattingOptions fc = formattingOptions;
    fc.optionsFlags |= FormattingOptionsFlags::internalSigned;

    if (v<0)
    {
        fc.optionsFlags |= FormattingOptionsFlags::internalNegative;
        return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(fc, pLocaleInfo, utils::toUnsignedAbs(v), valSize);
    }

    return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(fc, pLocaleInfo, utils::toUnsignedAbs(v), valSize);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormatString(const FormattingOptions &formattingOptions, const LocaleInfo* pLocaleInfo, const std::string &str)
{
    MARTY_ARG_USED(pLocaleInfo);

    // Extraction
    auto optionsFlags  = formattingOptions.optionsFlags;
    auto typeChar      = formattingOptions.typeChar;
    auto alignmentChar = formattingOptions.alignment;

    // Adaption
    if (!typeChar)
        typeChar = 's';

    if (typeChar!='s' && typeChar!='?')
    {
        if ((formattingOptions.formattingFlags&FormattingFlags::ignoreTypeMismatchErrors)==0)
            throw mismatch_format_type("invalid format type for string argument");
    }

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

    return StringType(strRes.c_str());
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, bool b)
{
    MARTY_ARG_USED(pLocaleInfo);

    FormattingOptions fc = formattingOptions;

    try_again:

    if (fc.typeChar==0 || fc.typeChar=='s' || fc.typeChar=='S' || fc.typeChar=='t' || fc.typeChar=='T' || fc.typeChar=='y' || fc.typeChar=='Y')
    {
        bool bBaseUpper = (fc.typeChar=='S' || fc.typeChar=='T' || fc.typeChar=='Y');
        bool yesNo      = (fc.typeChar=='y' || fc.typeChar=='Y');
        std::string boolStr = b ? (yesNo?"yes":"true") : (yesNo?"no":"false");

        if (bBaseUpper)
        {
            // Верхний регистр
            utils::toupper(boolStr);

            // Если установлена опция caseInvert, то меняем регистр первому символу - получаем странное tRUE/fALSE, но кому-то может понравится
            if ((fc.optionsFlags&FormattingOptionsFlags::caseInvert)!=0)
                boolStr[0] = utils::tolower(boolStr[0]);
        }
        else
        {
            // Регистр уже нижний
            // Если установлена опция caseInvert, то меняем регистр первому символу - получаем True/False
            if ((fc.optionsFlags&FormattingOptionsFlags::caseInvert)!=0)
                boolStr[0] = utils::toupper(boolStr[0]);
        }

        // Меняем на явный 's' для передачи в функцию форматирования строки, она не понимает 'S'/'t'/'T'/'y'/'Y'
        fc.typeChar = 's';

        if ((fc.optionsFlags&FormattingOptionsFlags::signAlterForm)!=0) // '#'
        {
            // AlterForm для bool задаёт формат с отображением единственного символа f/F/t/T/y/Y/n/N
            fc.optionsFlags |= FormattingOptionsFlags::precisionTaken; // Устанавливаем флаг, что precision установлено
            // Задаём precision в единицу, а форматирование строк само уже знает, что с этим делать
            fc.precision = 1;
        }

        return martyFormatValueFormatString<WidthCalculator, StringType>(fc, pLocaleInfo, boolStr);
    }
    else if (fc.typeChar=='b' || fc.typeChar=='B' || fc.typeChar=='d' || fc.typeChar=='o' || fc.typeChar=='x' || fc.typeChar=='X')
    {
        // Форматируем как байт
        return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(fc, pLocaleInfo, (unsigned char)(b?1u:0u), 1u);
    }

    if ((formattingOptions.formattingFlags&FormattingFlags::ignoreTypeMismatchErrors)==0)
        throw mismatch_format_type("invalid format type for bool argument");

    fc.typeChar = 's';

    goto try_again;

    //return martyFormatSimpleConvertToString<StringType>(b);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, char ch)
{
    return martyFormatValueFormatString<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, std::string(1, ch));
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, const void* ptr)
{
    return martyFormatValueFormatPointer<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, std::uintptr_t(ptr), true  /* formatNativePtr */);
}

//----------------------------------------------------------------------------
// template< typename WidthCalculator, typename StringType=std::string >
// StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, std::nullptr_t ptr)
// {
//     return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, (const void*)ptr);
// }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, const marty::Decimal &d)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(pLocaleInfo);
    return martyFormatSimpleConvertToString<StringType>(d);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, signed char  v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, signed short v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, int          v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, long         v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, long long    v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }

template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, unsigned char      v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, unsigned short     v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, unsigned int       v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, unsigned long      v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, unsigned long long v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v, sizeof(v)); }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, float v)       { return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, double v)      { return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, long double v) { return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, v); }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, const std::string &str)  { return martyFormatValueFormatString<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, str); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, const char* str)         { return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, str ? std::string(str) : std::string()); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, const std::wstring &str) { return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, marty::utf::string_from_wstring(str)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, const wchar_t* str)      { return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, pLocaleInfo, str ? std::wstring(str) : std::wstring()); }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, FormatValueFilter filter)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(pLocaleInfo);
    MARTY_ARG_USED(filter);
    throw filter_as_value_error("invalid filter usage: filter used as value");
}

//----------------------------------------------------------------------------

