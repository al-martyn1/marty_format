
template<typename ResultStringType, typename CharIterator, typename FormatHandler, typename IndexStringConverter> inline
ResultStringType processFormatStringImpl(CharIterator pCharB, CharIterator pCharE, FormatHandler handler, IndexStringConverter indexStringConverter, bool ignoreErrors=true)
{
    // UMBA_USED(ignoreErrors);
    MARTY_ARG_USED(indexStringConverter);

    // https://en.cppreference.com/w/cpp/utility/format/spec
    // Based on the format specification in Python - https://docs.python.org/3/library/string.html#formatspec

    // format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]

    // '+' - indicates that a sign should be used for both positive as well as negative numbers.
    // '-' - indicates that a sign should be used only for negative numbers (this is the default behavior).
    // space - indicates that a leading space should be used on positive numbers, and a minus sign on negative numbers.

    // The '#' option causes the “alternate form” to be used for the conversion. 
    // The ',' option signals the use of a comma for a thousands separator for floating-point presentation types and for integer presentation type 'd'
    // he '_' option signals the use of an underscore for a thousands separator for floating-point presentation types and for integer presentation type 'd'. For integer presentation types 'b', 'o', 'x', and 'X', underscores will be inserted every 4 digits. For other presentation types, specifying this option is an error.

    ResultStringType strRes;

    using namespace marty::format::utils;
    FormattingOptions formattingOptions;


    auto outIt        = marty::utf::UtfOutputIterator<char>(strRes);
    auto fmtStrLen    = std::distance(pCharB, pCharE);
    auto pFmtStrBegin = rawConstCharPtrFromIterator(pCharB);
    auto pFmtStrEnd   = pFmtStrBegin;
    std::advance(pFmtStrEnd, fmtStrLen);

    auto b = marty::utf::UtfInputIterator<char>(pFmtStrBegin, pFmtStrEnd);
    auto e = marty::utf::UtfInputIterator<char>();

    //State st = State::normalChars;

    using utf32_char_t = marty::utf::utf32_char_t;
    using utfch_t = marty::utf::utf32_char_t;

    bool parsingFormat = false; // else in processing text mode

    const char* pStrIdBegin = 0;
    const char* pStrIdEnd   = 0;

    auto resetStrId = [&]()
    {
        pStrIdBegin = 0;
        pStrIdEnd   = 0;
    };

    auto captureRefBegin = [&]()
    {
        pStrIdBegin = rawConstCharPtrFromIterator(b);
    };

    auto captureRefEnd = [&]()
    {
        pStrIdEnd   = rawConstCharPtrFromIterator(b);
        pStrIdBegin = ltrim_copy(pStrIdBegin, pStrIdEnd);
        pStrIdEnd   = rtrim_copy(pStrIdBegin, pStrIdEnd);
    };

            // pStrIdBegin = rawConstCharPtrFromIterator(b);
            // while(b!=e)
            // {
            //     if (ch==utfch_t(':') || ch==utfch_t('}') || utils::isFormatConvertMarker(ch))
            //         break;
            //     incB();
            // }
            // pStrIdEnd   = rawConstCharPtrFromIterator(b);
            // pStrIdBegin = ltrim_copy(pStrIdBegin, pStrIdEnd);
            // pStrIdEnd   = rtrim_copy(pStrIdBegin, pStrIdEnd);

    utfch_t ch = 0;
    if (b!=e)
        ch = *b;

    auto incB = [&]()
    {
        ++b;
        if (b!=e)
            ch = *b;
    };

    auto readFilters = [&]()
    {
        while (b != e)
        {
            if (ch != utfch_t('|'))
                break;

            incB();

            captureRefBegin();
            while (b != e)
            {
                if (ch == utfch_t('|') || ch == utfch_t('}'))
                    break;
                incB();
            }
            captureRefEnd();

            pStrIdBegin = ltrim_copy(pStrIdBegin, pStrIdEnd);
            pStrIdEnd = rtrim_copy(pStrIdBegin, pStrIdEnd);
            if (pStrIdBegin != pStrIdEnd)
            {
                std::size_t idx = indexStringConverter(pStrIdBegin, pStrIdEnd, FormatIndexType::filterRef);
                if (idx != std::size_t(-1))
                {
                    // Если больше восьми фильтров в цепочке - лишнее отбрасываем
                    if (formattingOptions.numFilters < std::size_t(FormattingOptions::MaxFilters - 1u))
                    {
                        formattingOptions.filters[std::size_t(formattingOptions.numFilters)] = arg_idx_t(idx);
                        ++formattingOptions.numFilters;
                    }
                }
            }

            resetStrId();
        }
    };


    auto doFormat = [&]()
    {
        //out << "\n" << formattingOptions << "\n" << umba::omanip::flush;

        if ((formattingOptions.optionsFlags&FormattingOptionsFlags::argIdTaken)==0)
        {
            std::size_t idx = indexStringConverter(0, 0, FormatIndexType::argId);
            if (idx!=std::size_t(-1))
            {
                formattingOptions.argIdx = arg_idx_t(idx);
                formattingOptions.optionsFlags |= FormattingOptionsFlags::argIdTaken;
            }
        }

        resetStrId();

        auto formatted = handler(formattingOptions);
        //auto outIt = marty::utf::UtfOutputIterator<char>(strRes);
        #if 0
        auto fb = marty::utf::UtfInputIterator<char>(formatted);
        auto fe = marty::utf::UtfInputIterator<char>();
        while(fb!=fe)
        {
            *outIt++ = *fb++;
        }
        #endif

        strRes.append(formatted);

        parsingFormat = false;
    };

    auto finalizeParsing = [&](const std::string &msg) -> std::string
    {
        if (parsingFormat && !ignoreErrors)
            throw std::runtime_error(msg);
        if (parsingFormat)
            doFormat(); // Незакрытую форматную последовательность обрабатываем, как будто она закрыта
        resetStrId();
        return strRes;
    };

    auto optionalError = [&](const std::string &msg)
    {
        if (!ignoreErrors)
            throw std::runtime_error(msg);
    };



    //std::string possibleFillRef;
    bool possibleFillRefGot = false;


    while(b!=e)
    {

        // Обычный режим - копируем символы со входа на выход, и ждем управляющие
        parsingFormat = false;
        formattingOptions = FormattingOptions{}; // сбрасываем опции форматирования на дефолтные
        //possibleFillRef.clear();
        possibleFillRefGot = false;

        while(b!=e)
        {
            if (ch==utfch_t('{') || ch==utfch_t('}'))
                break; // Встретили управляющий символ

            *outIt++ = ch;
            incB();
        }

        if (b==e)
            return strRes;

        {
            bool bPevOpen = ch==utfch_t('{');
            incB();
            if (b==e)
            {
                if (bPevOpen)
                {
                    parsingFormat = true;
                    return finalizeParsing("unexpected end reached after format open bracket (incomplete format specification or unfinished escape sequence)");
                }
                else
                {
                    return finalizeParsing("unexpected end reached after format close bracket (unfinished escape sequence?)");
                }
            }

            //bool bOpen = *b==utfch_t('{');
            if (!bPevOpen) // предыдущая была закрывающей
            {
                if (ch==utfch_t('}')) // текущая - тоже закрывающая
                {
                    *outIt++ = ch; // Двойную закрывающую преобразуем в одинарную закрывающую
                    incB();
                    continue; // Продолжаем обычный разбор
                }

                // if (!ignoreErrors) // Или не реагируем на такие ошибки?
                //     throw std::runtime_error("invalid escape sequence");

                // Одинокую закрывающую скобку копируем как есть
                *outIt++ = utfch_t('}');
                *outIt++ = ch;
                incB();
                continue;
            }
            else // предыдущая была открывающей
            {
                if (ch==utfch_t('{')) // текущая - тоже открывающая
                {
                    *outIt++ = ch; // Двойную открывающую преобразуем в одинарную открывающую
                    incB();
                    continue; // Продолжаем обычный разбор
                }

                if (ch==utfch_t('}')) // текущая - закрывающая - это строка форматирования со ссылкой на аргумент, индекс которого вычисляется автоматически
                {
                    incB(); doFormat(); continue;
                }
            }

            // предыдущая скобка была открывающей, но второй открывающей скобки не последовало, как и закрывающей, значит, началась форматная строка
            // *b - уже символ форматной строки
            parsingFormat = true;
        }

        // Парсим форматную строку

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        // В качестве argId позволяем вообще всё
        // Это позволит использовать именованные параметры
        // Если argId конвертируется в число - то это числовой позиционный индекс
        // Если в качестве списка элементов - плоский вектор без имён, то тогда
        // именованные argId недопустимы

        // **arg_id**
        {
            captureRefBegin();
            while(b!=e)
            {
                if (ch==utfch_t(':') || ch==utfch_t('|') || ch==utfch_t('}') || utils::isFormatConvertMarker(ch))
                    break;
                incB();
            }
            captureRefEnd();

            //if (pArgIdBegin!=pArgIdEnd)
            {
                std::size_t idx = indexStringConverter(pStrIdBegin, pStrIdEnd, FormatIndexType::argId);
                if (idx!=std::size_t(-1))
                {
                    formattingOptions.argIdx = arg_idx_t(idx);
                    formattingOptions.optionsFlags |= FormattingOptionsFlags::argIdTaken;
                }
            }

            resetStrId();
        }

        if (b==e) // Дошли до конца
            return finalizeParsing("unexpected end reached while reading ArgId");

        if (ch==utfch_t('|'))
        {
            readFilters();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading ArgId");
            goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
        }

        if (ch==utfch_t('}'))
        {
            incB(); doFormat(); continue;
        }


        // **conversion**
        if (utils::isFormatConvertMarker(ch))
        {
            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading convert option");

            if (utils::isFormatConvertChar(ch))
            {
                formattingOptions.convertChar = ch;
            }
            else
            {
                optionalError("unexpected symbol reached while parsing format spec");
            }

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading convert option");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }

            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
            
            if (ch!=utfch_t(':'))
            {
                optionalError("unexpected symbol reached while parsing format spec");
            }

        }

    continueFormatParsing:

        incB(); // Пропустили двоеточие

        if (b==e) // Дошли до конца
            return finalizeParsing("unexpected end reached while waiting for format spec");

        if (ch==utfch_t('|'))
        {
            readFilters();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading ArgId");
            goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
        }

        if (ch==utfch_t('}'))
        {
            incB(); doFormat(); continue;
        }


        // **fill**
        if (!utils::isFormatAnySpecialChar(ch))
        {
            formattingOptions.fillChar = ch;
            formattingOptions.optionsFlags |= FormattingOptionsFlags::fillingTaken;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }
        else if (ch==utfch_t('{'))
        {
            // тут ссылка на символ заполнения
            // либо это ссылка на ширину поля

            //auto fillIdOutIt = marty::utf::UtfOutputIterator<char>(possibleFillRef);
            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec (FillRef/WidthRef)");

            possibleFillRefGot = true;

            captureRefBegin();
            while(b!=e)
            {
                if (ch==utfch_t('}'))
                    break;
                //*fillIdOutIt++ = ch;
                incB();
            }
            captureRefEnd();

            if (b==e) // Дошли до конца
            {
                // Так как маркера выравнивания мы тут не дождались, значит, это ширина
                std::size_t idx = indexStringConverter(pStrIdBegin, pStrIdEnd, FormatIndexType::widthRef);
                resetStrId();
                if (idx!=std::size_t(-1))
                {
                    formattingOptions.width = arg_idx_t(idx);
                    formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken | FormattingOptionsFlags::fieldWidthIndirect;
                }

                return finalizeParsing("unexpected end reached while reading format spec (FillRef/WidthRef)");
            }

            // Тут у нас закрывающая FillRef скобка
            incB(); // Пропускаем закрывающую скобку
            if (b==e) // Дошли до конца
            {
                // Так как маркера выравнивания мы тут не дождались, значит, это ширина
                std::size_t idx = indexStringConverter(pStrIdBegin, pStrIdEnd, FormatIndexType::widthRef);
                resetStrId();
                if (idx!=std::size_t(-1))
                {
                    formattingOptions.width = arg_idx_t(idx);
                    formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken | FormattingOptionsFlags::fieldWidthIndirect;
                }

                return finalizeParsing("unexpected end reached while reading format spec");
            }

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                // Так как маркера выравнивания мы тут не дождались, значит, это ширина
                std::size_t idx = indexStringConverter(pStrIdBegin, pStrIdEnd, FormatIndexType::widthRef);
                resetStrId();
                if (idx!=std::size_t(-1))
                {
                    formattingOptions.width = arg_idx_t(idx);
                    formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken | FormattingOptionsFlags::fieldWidthIndirect;
                }

                incB(); doFormat(); continue;
            }

        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        // **align** **width**
        if (utils::isFormatAlignMarker(ch))
        {
            formattingOptions.alignment = char(ch); // флаги не нужны - выравнивание задано по умолчанию

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }

            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }

            if (possibleFillRefGot)
            {
                std::size_t idx = indexStringConverter(pStrIdBegin, pStrIdEnd, FormatIndexType::fillRef);
                resetStrId();
                if (idx!=std::size_t(-1))
                {
                    formattingOptions.fillIdx = arg_idx_t(idx);
                    formattingOptions.optionsFlags |= FormattingOptionsFlags::fillingTaken | FormattingOptionsFlags::fillingIndirect;
                }

                possibleFillRefGot = false;
            }
        }
        else if (possibleFillRefGot)
        {
            // у нас тут какой-то символ, но не признак выравнивания
            // а перед этим у нас была ссылка
            // а ближайшая возможная ссылка - это ссылка на ширину

            std::size_t idx = indexStringConverter(pStrIdBegin, pStrIdEnd, FormatIndexType::widthRef);
            resetStrId();
            if (idx!=std::size_t(-1))
            {
                formattingOptions.width = arg_idx_t(idx);
                formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken | FormattingOptionsFlags::fieldWidthIndirect;
            }

            possibleFillRefGot = false;
            goto waitForPrecision;
        }


        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        // **sign**
        if (utils::isFormatSignMarker(ch) || ch==utfch_t(' '))
        {
            switch(ch)
            {
                case utfch_t('+'): formattingOptions.optionsFlags |= FormattingOptionsFlags::signPlus ; break;
                case utfch_t('-'): formattingOptions.optionsFlags |= FormattingOptionsFlags::signMinus; break;
                case utfch_t(' '): formattingOptions.optionsFlags |= FormattingOptionsFlags::signSpace; break;
            }

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }


        // **z**
        if (ch==utfch_t('z'))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::signZ;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // **!** caseInvert
        if (ch==utfch_t('!'))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::caseInvert;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // **#**
        if (utils::isFormatAlterChar(ch))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::signAlterForm;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        // **0**
        if (utils::isFormatDigit(ch, 0, 0))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::signZero;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // **width**
        if (utils::isFormatDigit(ch, 1, 9))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken;
            formattingOptions.width   = (width_t)utils::toDigit(ch);

            incB();
            while(b!=e && utils::isFormatDigit(ch, 0, 9))
            {
                formattingOptions.width *= 10;
                formattingOptions.width += (width_t)utils::toDigit(ch);
                incB();
            }

            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }
        else if (ch==utfch_t('{'))
        {
            //formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken | FormattingOptionsFlags::fieldWidthIndirect;
            //auto widthIdOutIt = marty::utf::UtfOutputIterator<char>(formattingOptions.fieldWidthRef);

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec (WidthRef)");

            captureRefBegin();
            while(b!=e)
            {
                if (ch==utfch_t('}') || ch==utfch_t('|')) //!!!
                    break;
                //*widthIdOutIt++ = ch;
                incB();
            }
            captureRefEnd();

            std::size_t idx = indexStringConverter(pStrIdBegin, pStrIdEnd, FormatIndexType::widthRef);
            resetStrId();
            if (idx != std::size_t(-1))
            {
                formattingOptions.width = arg_idx_t(idx);
                formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken | FormattingOptionsFlags::fieldWidthIndirect;
            }

            if (b==e) // Дошли до конца
            {
                return finalizeParsing("unexpected end reached while reading format spec (WidthRef)");
            }

            // Тут у нас закрывающая WidthRef скобка
            incB(); // Пропускаем закрывающую скобку
            if (b == e) // Дошли до конца
            {
                return finalizeParsing("unexpected end reached while reading format spec");
            }

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

    waitForPrecision:

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        // **grouping**
        if (utils::isFormatFormatThousandSep(ch))
        {
            formattingOptions.grouppingChar = ch;

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        // **precision** start
        if (utils::isFormatPeriodChar(ch))
        {
            // formattingOptions.optionsFlags |= FormattingOptionsFlags::precisionTaken;

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('{')) // ref
            {
                // formattingOptions.optionsFlags |= FormattingOptionsFlags::precisionIndirect;
                // auto precisionIdOutIt = marty::utf::UtfOutputIterator<char>(formattingOptions.precisionRef);

                incB();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec (PrecisionRef)");

                captureRefBegin();
                while(b!=e)
                {
                    if (ch==utfch_t('}'))
                        break;
                    // *precisionIdOutIt++ = ch;
                    incB();
                }
                captureRefEnd();

                std::size_t idx = indexStringConverter(pStrIdBegin, pStrIdEnd, FormatIndexType::precisionRef);
                resetStrId();
                if (idx != std::size_t(-1))
                {
                    formattingOptions.precision = arg_idx_t(idx);
                    formattingOptions.optionsFlags |= FormattingOptionsFlags::precisionTaken | FormattingOptionsFlags::precisionIndirect;
                }

                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec (PrecisionRef)");

                incB();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec");

                if (ch==utfch_t('|'))
                {
                    readFilters();
                    if (b==e) // Дошли до конца
                        return finalizeParsing("unexpected end reached while reading ArgId");
                    goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
                }
        
                if (ch==utfch_t('}'))
                {
                    incB(); doFormat(); continue;
                }
            }
            else if (utils::isFormatDigit(ch, 0, 9))
            {
                formattingOptions.optionsFlags |= FormattingOptionsFlags::precisionTaken;
                formattingOptions.precision = (width_t)utils::toDigit(ch);

                incB();
                while(b!=e && utils::isFormatDigit(ch, 0, 9))
                {
                    formattingOptions.precision *= 10;
                    formattingOptions.precision += (width_t)utils::toDigit(ch);
                    incB();
                }

                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec");

                if (ch==utfch_t('|'))
                {
                    readFilters();
                    if (b==e) // Дошли до конца
                        return finalizeParsing("unexpected end reached while reading ArgId");
                    goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
                }
        
                if (ch==utfch_t('}'))
                {
                    incB(); doFormat(); continue;
                }
            }

        }

        // **fractional_grouping**
        if (utils::isFormatFormatThousandSep(ch))
        {
            if ((formattingOptions.optionsFlags&FormattingOptionsFlags::precisionTaken)==0)
                optionalError("unexpected symbol reached while parsing format spec");

            formattingOptions.fractionalGrouppingChar = ch;

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }


        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatLocaleChar(ch))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::localeFormatting;

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        if (utils::isFormatTypeChar(ch))
        {
            formattingOptions.typeChar = char(ch);

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('|'))
            {
                readFilters();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading ArgId");
                goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
            }
    
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        if (ch==utfch_t('|'))
        {
            readFilters();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading ArgId");
            goto waitClosingBrace; // Всё, у нас тут может быть только завершение форматного поля
        }


    waitClosingBrace:

        if (ch==utfch_t('}'))
        {
            incB(); doFormat(); continue;
        }

        optionalError("unexpected symbol reached while parsing format spec");

        // incB();
        // if (b==e) // Дошли до конца
        //     return finalizeParsing("unexpected end reached while reading format spec");

        goto continueFormatParsing;

    }

    return strRes;

}

