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
StringType martyFormatValueFormatString(const FormattingOptions &formattingOptions, const std::string &str);

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
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, bool b);

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormatPointer(const FormattingOptions &formattingOptions, unsigned long long p, bool formatNativePtr=false)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(formatNativePtr);
    // Надо обдумать, как форматировать сегментный адрес
    return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned long long)(p)).c_str());
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatUnsigned(const FormattingOptions &formattingOptions, IntType v, size_t valSize)
{
    //MARTY_ARG_USED(valSize);

    if (formattingOptions.typeChar=='t' || formattingOptions.typeChar=='T' || formattingOptions.typeChar=='y' || formattingOptions.typeChar=='Y')
        return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, (v==0 ? false : true) );

    if (formattingOptions.typeChar=='p' || formattingOptions.typeChar=='P')
        return martyFormatValueFormatPointer<WidthCalculator, StringType>(formattingOptions, (unsigned long long)v);

    auto typeChar = formattingOptions.typeChar;

    if (typeChar==0)
        typeChar = 'd';

    const bool doPercent = (typeChar=='%');
    if (typeChar=='%')
        typeChar = 'd';

    bool localeFormattingOpt = ((formattingOptions.optionsFlags&FormattingOptionsFlags::localeFormatting)!=0); // L option char - C++
    const bool useLocale = (typeChar=='n') || localeFormattingOpt;
    if (typeChar=='n')
        typeChar = 'd';

    if (typeChar!='b' && typeChar!='B' && typeChar!='d' && typeChar!='o' && typeChar!='O' && typeChar!='x' && typeChar!='X' && typeChar!='h' && typeChar!='H')
    {
        // type            ::= "a" | "A" | "b" | "B" | "c" | "d" | "e" | "E" | "f" | "F" | "g" | "G" | "n" | 
        //                     "o" | "p" | "s" | "S" | "t" | "T" | "x" | "X" | "y" | "Y" | "%"
        if ((formattingOptions.formattingFlags&FormattingFlags::ignoreTypeMismatchErrors)==0)
            throw mismatch_format_type("invalid format type for int/unsigned argument");

        typeChar = 'd';
    }

    // Максимальная длина разделителя разрядов - 16 символов
    std::array<marty::utf::utf32_char_t, 16> groupSepRaw = { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u }; 
    //const char fracSep[2]  = { 0, 0 }; // не нужно для целых

    const bool grouppingTakenOpt = ((formattingOptions.optionsFlags&FormattingOptionsFlags::grouppingTaken)!=0);
    bool grouppingTaken = false;
    if (grouppingTakenOpt || localeFormattingOpt)
    {
        grouppingTaken = true;

        // if (grouppingTakenOpt)
        {
            groupSepRaw[0] = formattingOptions.grouppingChar;
            if (groupSepRaw[0]==0)
            {
                if (typeChar=='b' || typeChar=='B' || typeChar=='o'  || typeChar=='O' || typeChar=='x' || typeChar=='X' || typeChar=='h' || typeChar=='H')
                    groupSepRaw[0] = marty::utf::utf32_char_t('_');
                else
                    groupSepRaw[0] = marty::utf::utf32_char_t('\'');
            }
        }
        // else // localeFormattingOpt
        // {
        //     // TODO: !!! Извлечь разделитель разрядов из системы
        // }
    }

    std::string grpSepStr;
    {
        auto outIt = marty::utf::UtfOutputIterator<char>(grpSepStr);
        for(auto uch : groupSepRaw)
        {
            if (!uch)
                break;
            *outIt++ = uch;
        }
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
    std::size_t grpSize = 3;
    IntType base = 10u;
    auto fillChar = formattingOptions.fillChar;
    //utf32_char_t  = 

    //const bool prefixOptional = (typeChar=='o'); // Если префикс опциональный и первая цифра - 0, то префикс не добавляется
    std::string prefixStr;
    std::string postfixStr;

    // bBdoxX
    if (typeChar=='b' || typeChar=='B')
    {
        grpSize = 4;
        base    = 2u;
        prefixStr.assign(prefixUpper?"0B":"0b");
        //if (!fillChar)
        //    fillChar = marty::utf::utf32_char_t('0');
    }
    else if (typeChar=='d')
    {
        // if (!fillChar)
        //     fillChar = marty::utf::utf32_char_t('0');
    }
    else if (typeChar=='o')
    {
        grpSize = 3;
        base    = 8u;
        prefixStr.assign(1, '0');
        // if (!fillChar)
        //     fillChar = marty::utf::utf32_char_t('0');
    }
    else if (typeChar=='x' || typeChar=='X')
    {
        grpSize = 4;
        base    = 16u;
        prefixStr.assign(prefixUpper?"0X":"0x");
        // if (!fillChar)
        //     fillChar = marty::utf::utf32_char_t('0');
    }
    else if (typeChar=='h' || typeChar=='H')
    {
        grpSize = 4;
        base    = 16u;
        postfixStr.assign(prefixUpper?"H":"h");
        // if (!fillChar)
        //     fillChar = marty::utf::utf32_char_t('0');
    }

    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signAlterForm)==0)
    {
        prefixStr.clear(); // Очищаем префикс - "альтернативная" форма не задана
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


    std::string numStr;
    {
        auto vTmp = v; // На всякий случай делаем копию, вдруг оригинальное число ещё пригодится
        for(; vTmp; vTmp/=base)
        {
            numStr.append(1, utils::digitToChar(int(vTmp % base), int(base)));
        }

        if (numStr.empty())
            numStr.assign(1, '0');
    }


    char signChar = 0; // No sign

    //FormattingOptionsFlags signOpt = FormattingOptionsFlags::signMinus; // (this is the default behavior)
    // Sign, #, and 0
    // The sign option can be one of following:
    // +: Indicates that a sign should be used for both non-negative and negative numbers. The + sign is inserted before the output value for non-negative numbers.
    // -: Indicates that a sign should be used for negative numbers only (this is the default behavior).
    // space: Indicates that a leading space should be used for non-negative numbers, and a minus sign for negative numbers.
    // Negative zero is treated as a negative number.
    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signPlus)!=0)
    {
        // signOpt = FormattingOptionsFlags::signPlus;
        if ((formattingOptions.optionsFlags&FormattingOptionsFlags::internalNegative)!=0)
            signChar = '-';
        else
            signChar = '+';
    }
    else if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signSpace)!=0)
    {
        // signOpt = FormattingOptionsFlags::signSpace;
        if ((formattingOptions.optionsFlags&FormattingOptionsFlags::internalNegative)!=0)
            signChar = '-';
        else
            signChar = ' ';
    }
    else // FormattingOptionsFlags::signMinus
    {
        if ((formattingOptions.optionsFlags&FormattingOptionsFlags::internalNegative)!=0)
            signChar = '-';
    }


    std::reverse(grpSepStr.begin(), grpSepStr.end()); // делаем реверс разделителя, потом будет обратный реверс

    width_t grpSepLen = width_t(WidthCalculator()(grpSepStr.data(), grpSepStr.size()));
    width_t numStrLen     = width_t(numStr.size()); // число цифровых символов в строке
    width_t numStrFullLen = width_t(numStr.size()); // число символов вместе с сепараторами
    {
        std::string tmpStr;
        for(width_t i=0u; i!=numStrLen; ++i)
        {
            if (i && (i%grpSize)==0)
            {
                tmpStr.append(grpSepStr);
                numStrFullLen += grpSepLen;
            }
            tmpStr.append(1, numStr[i]);
        }

        swap(numStr, tmpStr);
    }

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
        std::reverse(numStr.begin(), numStr.end());

        FormattingOptions fc = formattingOptions;
        fc.typeChar = 's';
        fc.optionsFlags &= FormattingOptionsFlags::precisionTaken;
        fc.precision = 0;

        std::string tmpStr;
        if (signChar)
            tmpStr.append(1u, signChar);

        if (!prefixStr.empty())
            tmpStr.append(prefixStr);

        tmpStr.append(numStr);

        if (!postfixStr.empty())
            tmpStr.append(postfixStr);

        if (doPercent)
            tmpStr.append(1u, '%');

        return martyFormatValueFormatString<WidthCalculator, StringType>(fc, tmpStr);
    }

    width_t extraWidth = 0;

    if (signChar!=0)
        extraWidth += width_t(1u);

    if (!prefixStr.empty())
        extraWidth += width_t(prefixStr.size());

    if (!postfixStr.empty())
        extraWidth += width_t(postfixStr.size());

    if (doPercent)
        extraWidth += width_t(1u);


    // signZero
    // numStrFullLen += width_t(grpSepLen);
    auto restWidth = formattingOptions.width;
    if (restWidth>=numStrFullLen)
        restWidth -= numStrFullLen;
    else
        restWidth = 0;

    if (restWidth>=extraWidth)
        restWidth -= extraWidth;
    else
        restWidth = 0;

    
    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signZero)==0)
    {
        // Заполняем при помощи fillChar после добавления префикса, но до добавления знака

        // if (restWidth>=numStr.size())
        //     restWidth -= width_t(numStr.size());
        // else
        //     restWidth = 0;

        std::reverse(numStr.begin(), numStr.end());

        std::string fillStr;
        auto outIt = marty::utf::UtfOutputIterator<char>(fillStr);
        for(; restWidth; --restWidth)
            *outIt++ = fillChar;

        return StringType((std::string((signChar==0 ? 0u : 1u), signChar) + fillStr + prefixStr + numStr + postfixStr + std::string((doPercent==0 ? 0u : 1u), '%')).c_str());
    }    

    // signZero установлен

    if (formattingOptions.width!=0 || (base!=2 && base!=16 )) 
    {
        while(restWidth!=0)
        {
            if (numStrLen && (numStrLen%grpSize)==0)
            {
                //numStr.append(grpSepStr);
                //numStrFullLen += width_t(grpSepStr.size());
                if (restWidth>=grpSepLen)
                {
                    numStr.append(grpSepStr);
                    restWidth -= grpSepLen;
                }
            }
    
            numStr.append(1, '0');
            restWidth -= width_t(1);
            ++numStrLen;
        }
    }
    else // Если было что-то типа 0x00X в форматной строке, то количество символов равно размеру типа
    {
        width_t totalRequiredWidth = 0;
        if (base==2)
            totalRequiredWidth = width_t(valSize*8u);
        else
            totalRequiredWidth = width_t(valSize*2u);

        while(numStrLen<totalRequiredWidth)
        {
            if (numStrLen && (numStrLen%grpSize)==0)
            {
                //numStr.append(grpSepStr);
                //numStrFullLen += width_t(grpSepStr.size());
                //if (restWidth>=grpSepLen)
                {
                    numStr.append(grpSepStr);
                    restWidth -= grpSepLen;
                }
            }
    
            numStr.append(1, '0');
            restWidth -= width_t(1);
            ++numStrLen;
        }
    }

    std::reverse(numStr.begin(), numStr.end());

    return StringType((std::string((signChar==0 ? 0u : 1u), signChar) + prefixStr + numStr + postfixStr + std::string((doPercent==0 ? 0u : 1u), '%')).c_str());

}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatInt(const FormattingOptions &formattingOptions, IntType v, size_t valSize)
{
    // Надо добавить форматный символ ~ - который делает битовый каст к беззнаковому
    // // шестнадцатиричное и двоичные числа битово кастим и отображаем как беззнаковые
    if ( (formattingOptions.optionsFlags&FormattingOptionsFlags::bitCast)!=0
      && (formattingOptions.typeChar=='b' || formattingOptions.typeChar=='B' || formattingOptions.typeChar=='x' || formattingOptions.typeChar=='X' || formattingOptions.typeChar=='h' || formattingOptions.typeChar=='H')
       )
    {
        return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, utils::toUnsignedCast(v), valSize);
    }

    FormattingOptions fc = formattingOptions;
    fc.optionsFlags |= FormattingOptionsFlags::internalSigned;

    if (v<0)
    {
        fc.optionsFlags |= FormattingOptionsFlags::internalNegative;
        return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(fc, utils::toUnsignedAbs(v), valSize);
    }

    return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(fc, utils::toUnsignedAbs(v), valSize);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormatString(const FormattingOptions &formattingOptions, const std::string &str)
{
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
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, bool b)
{
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

        return martyFormatValueFormatString<WidthCalculator, StringType>(fc, boolStr);
    }
    else if (fc.typeChar=='b' || fc.typeChar=='B' || fc.typeChar=='d' || fc.typeChar=='o' || fc.typeChar=='x' || fc.typeChar=='X')
    {
        // Форматируем как байт
        return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(fc, (unsigned char)(b?1u:0u), 1u);
    }

    if ((formattingOptions.formattingFlags&FormattingFlags::ignoreTypeMismatchErrors)==0)
        throw mismatch_format_type("invalid format type for bool argument");

    fc.typeChar = 's';

    goto try_again;

    //return martyFormatSimpleConvertToString<StringType>(b);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename FloatType >
StringType martyFormatValueFormatFloat(const FormattingOptions &formattingOptions, FloatType v)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(v);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, char ch)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(ch);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const void* ptr)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(ptr);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const marty::Decimal &d)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(d);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, signed char  v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, signed short v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, int          v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, long         v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, long long    v) { return martyFormatValueFormatInt<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }

template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned char      v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned short     v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned int       v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned long      v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, unsigned long long v) { return martyFormatValueFormatUnsigned<WidthCalculator, StringType>(formattingOptions, v, sizeof(v)); }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, float v)       { return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, v); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, double v)      { return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, v); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, long double v) { return martyFormatValueFormatFloat<WidthCalculator, StringType>(formattingOptions, v); }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const std::string &str)  { return martyFormatValueFormatString<WidthCalculator, StringType>(formattingOptions, str); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const char* str)         { return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, str ? std::string(str) : std::string()); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const std::wstring &str) { return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, marty::utf::string_from_wstring(str)); }
template< typename WidthCalculator, typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const wchar_t* str)      { return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, str ? std::wstring(str) : std::wstring()); }

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, FormatValueFilter filter)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(filter);
    throw filter_as_value_error("invalid filter usage: filter used as value");
}

//----------------------------------------------------------------------------

