#pragma once


//----------------------------------------------------------------------------
inline
LocaleInfo::LocaleInfo()
{
    auto grp_3_0 = parseGroupingString("3;0");
    auto grp_4_0 = parseGroupingString("4;0");

    groupInfoCur[0] = groupInfoCur[1] = grp_3_0;
    groupInfoBin[0] = groupInfoBin[1] = grp_4_0;
    groupInfoOct[0] = groupInfoOct[1] = grp_3_0;
    groupInfoDec[0] = groupInfoDec[1] = grp_3_0;
    groupInfoHex[0] = groupInfoHex[1] = grp_4_0;

    groupSeparatorCur[0] = groupSeparatorCur[1] = " ";
    groupSeparatorBin[0] = groupSeparatorBin[1] = "_";
    groupSeparatorOct[0] = groupSeparatorOct[1] = "'";
    groupSeparatorDec[0] = groupSeparatorDec[1] = "'";
    groupSeparatorHex[0] = groupSeparatorHex[1] = "_";

}

//----------------------------------------------------------------------------
inline
std::string LocaleInfo::getGroupSeparator( LocaleInfoValueType vt //! 
                                         , NumeralSystem ns       //!
                                         ) const
{
    if (vt!=LocaleInfoValueType::thousandSeparator && vt!=LocaleInfoValueType::fractionalSeparator)
    {
        throw std::invalid_argument("LocaleInfo::getGroupSeparator: vt is not thousandSeparator nor fractionalSeparator");
    }

    std::size_t idx = (vt!=LocaleInfoValueType::thousandSeparator) ? 1u : 0u;

    switch(ns)
    {
        case NumeralSystem::currency : return groupSeparatorCur[idx];
        case NumeralSystem::bin      : return groupSeparatorBin[idx];
        case NumeralSystem::hex      : return groupSeparatorOct[idx];
        case NumeralSystem::oct      : return groupSeparatorDec[idx];
        case NumeralSystem::dec      : return groupSeparatorHex[idx];
        case NumeralSystem::unknown  : [[fallthrough]];
        case NumeralSystem::none     : [[fallthrough]];
        default: throw std::invalid_argument("LocaleInfo::getGroupSeparator: invalid numeral system taken");
    }
}

//----------------------------------------------------------------------------
inline
std::string LocaleInfo::getLocaleInfoValue(LocaleInfoValueType vt) const
{
    switch(vt)
    {
        case LocaleInfoValueType::signDecimal           : return signDecimal           ;
        case LocaleInfoValueType::signPlus              : return signPlus              ;
        case LocaleInfoValueType::signMinus             : return signMinus             ;
        case LocaleInfoValueType::signPercent           : return signPercent           ;
        case LocaleInfoValueType::signCurrency          : return signCurrency          ;
        case LocaleInfoValueType::formatPercentNegative : return formatPercentNegative ;
        case LocaleInfoValueType::formatPercentPositive : return formatPercentPositive ;
        case LocaleInfoValueType::formatCurrencyNegative: return formatCurrencyNegative;
        case LocaleInfoValueType::formatCurrencyPositive: return formatCurrencyPositive;
        case LocaleInfoValueType::formatNumberNegative  : return formatNumberNegative  ;
        case LocaleInfoValueType::formatNumberPositive  : return formatNumberPositive  ;
        case LocaleInfoValueType::thousandSeparator     : [[fallthrough]];
        case LocaleInfoValueType::fractionalSeparator   : [[fallthrough]];
        case LocaleInfoValueType::unknown               : [[fallthrough]];
        case LocaleInfoValueType::none                  : [[fallthrough]];
        // case LocaleInfoValueType::: return "";
        default: throw std::invalid_argument("LocaleInfo::getLocaleInfoValue: value type taken");
    }
}

//----------------------------------------------------------------------------
inline
LocaleInfo::group_info_t LocaleInfo::getGroupInfo(NumeralSystem ns, bool bFractionalPart) const
{
    std::size_t idx = bFractionalPart ? 1u : 0u;

    switch(ns)
    {
        case NumeralSystem::currency : return groupInfoCur[idx];
        case NumeralSystem::bin      : return groupInfoBin[idx];
        case NumeralSystem::hex      : return groupInfoOct[idx];
        case NumeralSystem::oct      : return groupInfoDec[idx];
        case NumeralSystem::dec      : return groupInfoHex[idx];
        case NumeralSystem::unknown  : [[fallthrough]];
        case NumeralSystem::none     : [[fallthrough]];
        default: throw std::invalid_argument("LocaleInfo::getGroupSeparator: invalid numeral system taken");
    }
}

//----------------------------------------------------------------------------
inline
std::string LocaleInfo::substFormatString( const std::string &fmt
                                         , const std::string &numStr
                                         , const std::string &symbolPlus
                                         , const std::string &symbolMinus
                                         , const std::string &symbolPercent
                                         , const std::string &symbolCurrency
                                         , PositiveNumbersMode pm
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
            case '+': if (pm==PositiveNumbersMode::sign)
                          res.append(symbolPlus);
                      else if (pm==PositiveNumbersMode::space)
                          res.append(1, ' ');
                       break;
            case '%': res.append(symbolPercent ); break;
            case '$': res.append(symbolCurrency); break;
            default : res.append(1, ch);
        }
    }

    return res;
}

//----------------------------------------------------------------------------
inline
std::string LocaleInfo::substFormatString( const std::string &fmt
                                         , const std::string &numStr
                                         , PositiveNumbersMode pm
                                         ) const
{
    return substFormatString( fmt
                            , numStr
                            , signPlus    
                            , signMinus   
                            , signPercent 
                            , signCurrency
                            , pm
                            );
}

//----------------------------------------------------------------------------
inline
LocaleInfo::group_info_t LocaleInfo::fixGroupInfo(LocaleInfo::group_info_t &&grpInfo)
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

//----------------------------------------------------------------------------
inline
std::string LocaleInfo::composeGroupingString(const group_info_t &grpInfo)
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

//----------------------------------------------------------------------------
inline
LocaleInfo::group_info_t LocaleInfo::parseGroupingString(const std::string &str)
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

//----------------------------------------------------------------------------
inline
bool LocaleInfo::testAppendGroupSeparator(std::size_t curDigitIndex, const LocaleInfo::group_info_t &grpInfo)
{
    if (curDigitIndex==0) // группа не может начатся с самой первой позиции
        return false;

    if (grpInfo.empty()) // нет информации по группам, возвращаем признак, что не надо вставлять разделитель групп
        return false;

    // Инфа по группам есть, но первый элемент - нулевой.
    // Нулевым у нас может быть только последний элемент (сигнализирует, что предпоследняя группа повторяется),
    // а раз он также и первый, то предпоследнего элемента нет, значит повторений нет
    if (grpInfo.front()==0u) // возвращаем признак, что не надо вставлять разделитель групп
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

//----------------------------------------------------------------------------
inline
std::string LocaleInfo::insertGroupSeparatorsImplHelper(const std::string &numStr, const std::string &sep, const LocaleInfo::group_info_t &grpInfo)
{
    std::string res; res.reserve(numStr.size());

    for(std::size_t i=0; i!=numStr.size(); ++i)
    {
        if (testAppendGroupSeparator(i, grpInfo))
            res.append(sep);
        res.append(1u, numStr[i]);
    }

    return res;
}

//----------------------------------------------------------------------------
inline
std::string LocaleInfo::insertGroupSeparators( std::string numStr, std::string sep, const LocaleInfo::group_info_t &grpInfo, bool bFractionalPart)
{
    if (!bFractionalPart)
    {
        // Целая часть числа. 
        // Цифры числа у нас группируются справа налево
        // Значит, мы реверснём цифры числа, обработаем строку числа в нормальном порядке, затем реверснём результат обратно
        // Разделитель групп тоже надо реверснуть

        std::reverse(numStr.begin(), numStr.end());
        std::reverse(sep.begin()   , sep.end());
        auto res = insertGroupSeparatorsImplHelper(numStr, sep, grpInfo);
        std::reverse(res.begin(), res.end());
        return res;
    }
    else
    {
        // Для дробной части реверсы делать не надо, обработка идёт слева направо
        return insertGroupSeparatorsImplHelper(numStr, sep, grpInfo);
    }
}

//----------------------------------------------------------------------------
inline
std::string LocaleInfo::expandWithGroupSeparatorImplHelper( std::string numStr, const std::string &sep, const group_info_t &grpInfo
                                                          , std::size_t sepCalculatedLen  // Посчитанная снаружи длина разделителя
                                                          , std::size_t startingNumStrLen // Посчитанная снаружи полная длина строки, которую дополняем, включая сепараторы
                                                          , std::size_t startingDigitsCount
                                                          , std::size_t maxLen
                                                          )
{
    if (startingNumStrLen>=maxLen)
        return numStr;

    std::size_t restLen = maxLen - startingNumStrLen;
    std::size_t digitsCount = startingDigitsCount;

    while(restLen!=0)
    {
        if (testAppendGroupSeparator(digitsCount, grpInfo))
        {
            if (restLen>=sepCalculatedLen)
            {
                numStr.append(sep);
                restLen -= sepCalculatedLen;
            }

            //res.append(sep);
        }

        if (restLen)
        {
            numStr.append(1, '0');
            restLen -= 1u;
            ++digitsCount;
        }
    }

    return numStr;
}

//----------------------------------------------------------------------------
inline
std::string LocaleInfo::expandWithGroupSeparator( std::string numStr, std::string sep, const group_info_t &grpInfo
                                                , bool bFractionalPart
                                                , std::size_t sepCalculatedLen  // Посчитанная снаружи длина разделителя
                                                , std::size_t startingNumStrLen // Посчитанная снаружи полная длина строки, которую дополняем, включая сепараторы
                                                , std::size_t startingDigitsCount
                                                , std::size_t maxLen
                                                )
{
    if (!bFractionalPart)
    {
        // Целая часть числа. 
        // Цифры числа у нас группируются справа налево
        // Значит, мы реверснём цифры числа, обработаем строку числа в нормальном порядке, затем реверснём результат обратно
        // Разделитель групп тоже надо реверснуть

        std::reverse(numStr.begin(), numStr.end());
        std::reverse(sep.begin()   , sep.end());
        auto res = expandWithGroupSeparatorImplHelper(numStr, sep, grpInfo, sepCalculatedLen, startingNumStrLen, startingDigitsCount, maxLen);
        std::reverse(res.begin(), res.end());
        return res;
    }
    else
    {
        // Для дробной части реверсы делать не надо, обработка идёт слева направо
        return expandWithGroupSeparatorImplHelper(numStr, sep, grpInfo, sepCalculatedLen, startingNumStrLen, startingDigitsCount, maxLen);
    }
}


