/*!
    \file
    \brief Форматирование типа std::format в рантайме
 */
#pragma once

#include "marty_format_types.h"
#include "exceptions.h"
#include "utils.h"
#include "defs.h"

//
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <typeinfo>
#include <utility>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <map>
#include <initializer_list>


/*
Делаем форматирование по типу std::format:
1) для встраивания во всякую самодельную скриптоту;
2) для использования в своих плюсовых сорцах, ориентированных на
С++17, в котором нет std::format - типа бэкпорт.
3) именованые ссылки. В плюсах нет по объективным причинам, только индексы аргументов,
в питоне есть, и это может быть удобно, особенно из скриптоты.

Форматную строку делаем совместимую с плюсовой.
Заодно тырим из оригинального питона (https://docs.python.org/3/library/string.html#formatspec):
1) фичу с разделителем разрядов (которой нет в плюсовой версии),
в питоне это запятая и андерскор, дополнительно присунем apos
2) фичу с форматным символом процента
3) фичу с форматным символом 'n', который как 'd', но с разделителем разрядов.
В питоновском оригинале разделитель разрядов должен браться из локали, но
мы пока не хотим связываться с операционной системой, поэтому будет apos.

---

Что мы не делаем (пока или вообще, там видно будет)
1) Поддерживаем плюсовый форматный символ 'L', который говорит нам использовать
пользовательскую локаль, но используем какой-то фиксированный способ отображения;
2)

Что не понятно.
1) Ширина поля задаётся как NN[.MM], это понятно для плавучки, но вроде для строки это тоже работает.
Я что-то не понял как.




*/


// #include "marty_format/marty_format.h"
// marty::format::
namespace marty{
namespace format{


template<typename FormatHandler> inline
std::string processFormatStringImpl(const std::string &str, FormatHandler handler, bool ignoreErrors=true)
{
    // UMBA_USED(ignoreErrors);

    // https://en.cppreference.com/w/cpp/utility/format/spec
    // Based on the format specification in Python - https://docs.python.org/3/library/string.html#formatspec

    // format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]

    // '+' - indicates that a sign should be used for both positive as well as negative numbers.
    // '-' - indicates that a sign should be used only for negative numbers (this is the default behavior).
    // space - indicates that a leading space should be used on positive numbers, and a minus sign on negative numbers.

    // The '#' option causes the “alternate form” to be used for the conversion. 
    // The ',' option signals the use of a comma for a thousands separator for floating-point presentation types and for integer presentation type 'd'
    // he '_' option signals the use of an underscore for a thousands separator for floating-point presentation types and for integer presentation type 'd'. For integer presentation types 'b', 'o', 'x', and 'X', underscores will be inserted every 4 digits. For other presentation types, specifying this option is an error.

    std::string strRes;

    // using namespace marty::format;
    FormattingOptions formattingOptions;

    auto outIt = marty::utf::UtfOutputIterator<char>(strRes);
    auto b = marty::utf::UtfInputIterator<char>(str);
    auto e = marty::utf::UtfInputIterator<char>();

    //State st = State::normalChars;

    using utf32_char_t = marty::utf::utf32_char_t;
    using utfch_t = marty::utf::utf32_char_t;

    bool parsingFormat = false; // else in processing text mode

    auto doFormat = [&]()
    {
        //out << "\n" << formattingOptions << "\n" << umba::omanip::flush;

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
        return strRes;
    };

    auto optionalError = [&](const std::string &msg)
    {
        if (ignoreErrors)
            throw std::runtime_error(msg);
    };


    utfch_t ch = 0;
    if (b!=e)
        ch = *b;

    auto incB = [&]()
    {
        ++b;
        if (b!=e)
            ch = *b;
    };

    // b++;


    while(b!=e)
    {

        // Обычный режим - копируем символы со входа на выход, и ждем управляющие
        parsingFormat = false;
        formattingOptions = FormattingOptions{}; // сбрасываем опции форматирования на дефолтные

        while(b!=e)
        {
            if (ch==utfch_t('{') || ch==utfch_t('}'))
                break; // Встретили управляющий символ

            *outIt++ = ch;
            incB();
        }


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
        {
            auto argIdOutIt = marty::utf::UtfOutputIterator<char>(formattingOptions.argId);
            while(b!=e)
            {
                if (ch==utfch_t(':') || ch==utfch_t('}'))
                    break;
                *argIdOutIt++ = ch;
                incB();
            }

            if (!formattingOptions.argId.empty())
                formattingOptions.optionsFlags |= FormattingOptionsFlags::argIdTaken;
        }

        if (b==e) // Дошли до конца
            return finalizeParsing("unexpected end reached while reading ArgId");
        if (ch==utfch_t('}'))
        {
            incB(); doFormat(); continue;
        }

    continueFormatParsing:

        incB(); // Пропустили двоеточие

        if (b==e) // Дошли до конца
            return finalizeParsing("unexpected end reached while waiting for format spec");
        if (ch==utfch_t('}'))
        {
            incB(); doFormat(); continue;
        }


        if (!utils::isFormatAnySpecialChar(ch))
        {
            formattingOptions.fillChar = ch;
            formattingOptions.optionsFlags |= FormattingOptionsFlags::fillingTaken;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatAlignMarker(ch))
        {
            formattingOptions.alignment = char(ch); // флаги не нужны - выравнивание задано по умолчанию

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }


        if (utils::isFormatSignMarker(ch))
        {
            formattingOptions.optionsFlags |= (ch==utfch_t('+')) ? FormattingOptionsFlags::signPlus : FormattingOptionsFlags::signMinus;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }


        if (utils::isFormatAlterChar(ch))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::signAlterForm;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatDigit(ch, 0, 0))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::signZero;

            incB(); // Идём дальше
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }


        if (utils::isFormatDigit(ch, 1, 9))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken;
            formattingOptions.fieldWidth   = utils::toDigit(ch);

            incB();
            while(b!=e && utils::isFormatDigit(ch, 0, 9))
            {
                formattingOptions.fieldWidth *= 10;
                formattingOptions.fieldWidth += utils::toDigit(ch);
                incB();
            }

            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }
        else if (ch==utfch_t('{'))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::fieldWidthTaken | FormattingOptionsFlags::fieldWidthIndirect;
            auto widthIdOutIt = marty::utf::UtfOutputIterator<char>(formattingOptions.fieldWidthRef);

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec (WidthRef)");
            while(b!=e)
            {
                if (ch==utfch_t('}'))
                    break;
                *widthIdOutIt++ = ch;
                incB();
            }

            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec (WidthRef)");

            // Тут у нас закрывающая WidthRef скобка
            incB(); // Пропускаем закрывающую скобку
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatFormatThousandSep(ch))
        {
            formattingOptions.grouppingChar = ch;

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

        // python_format_spec ::= [[fill]align][sign]["z"]["#"]["0"][width][grouping_option]["." precision][type]
        // cpp_format_spec    ::= fill-and-align(optional) sign(optional) #(optional) 0(optional) width(optional) precision(optional) L(optional) type(optional)

        if (utils::isFormatPeriodChar(ch))
        {
            formattingOptions.optionsFlags |= FormattingOptionsFlags::precisionTaken;

            incB();
            if (b==e) // Дошли до конца
                return finalizeParsing("unexpected end reached while reading format spec");

            if (ch==utfch_t('{')) // ref
            {
                formattingOptions.optionsFlags |= FormattingOptionsFlags::precisionIndirect;
                auto precisionIdOutIt = marty::utf::UtfOutputIterator<char>(formattingOptions.precisionRef);

                incB();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec (PrecisionRef)");

                while(b!=e)
                {
                    if (ch==utfch_t('}'))
                        break;
                    *precisionIdOutIt++ = ch;
                    incB();
                }

                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec (PrecisionRef)");

                incB();
                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec");
                if (ch==utfch_t('}'))
                {
                    incB(); doFormat(); continue;
                }
            }
            else if (utils::isFormatDigit(ch, 0, 9))
            {
                formattingOptions.precision = utils::toDigit(ch);

                incB();
                while(b!=e && utils::isFormatDigit(ch, 0, 9))
                {
                    formattingOptions.precision *= 10;
                    formattingOptions.precision += utils::toDigit(ch);
                    incB();
                }

                if (b==e) // Дошли до конца
                    return finalizeParsing("unexpected end reached while reading format spec");
                if (ch==utfch_t('}'))
                {
                    incB(); doFormat(); continue;
                }
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
            if (ch==utfch_t('}'))
            {
                incB(); doFormat(); continue;
            }
        }

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

//----------------------------------------------------------------------------
/*

Что нам нужно?

valueGetter - по индексу в FormattingOptions (или переопределенному) должен возвращать элемент контейнера.
Принимает ссылку на контейнер, FormattingOptions, и ссылку на значение.

Ссылка на значение какой имеет тип?
Нам нужен шаблон, который принимает тип контейнера, и содержит value_type вложенный тип.
Если элемент контейнера - это нечто, содержащее first и second, тогда value_type - это second, иначе
value_type - это тип самого элемента контейнера.

Предполагается, что в контейнере лежит std::variant.

valueFormatter - свободная функция, принимает FormattingOptions и значение своего типа (int, const char*, float, etc)

*/



//----------------------------------------------------------------------------
//! "Выводитель" (дедуктор/deducer) типа данных для типа элементов контейнера - базовый шаблон
template<typename ContainerType, typename ConditionType=void>
struct ContainerValueTypeDeducer;

//----------------------------------------------------------------------------
template<typename ContainerType>
struct ContainerValueTypeDeducer< ContainerType
                                , typename std::enable_if< utils::has_first_type<typename ContainerType::value_type>::value 
                                                        && utils::has_second_type<typename ContainerType::value_type>::value 
                                                         >::type
                                >
{
    // has members 'first' and 'second'
    // 'first' is a key
    // 'second' is an value
    using value_type = typename ContainerType::value_type::second_type; // 
};

template<typename ContainerType>
struct ContainerValueTypeDeducer< ContainerType
                                , typename std::enable_if< !utils::has_first_type<typename ContainerType::value_type>::value
                                                        || !utils::has_second_type<typename ContainerType::value_type>::value
                                                         >::type
                                >
{
    // no members 'first' nor 'second'
    using value_type = typename ContainerType::value_type;
};




//----------------------------------------------------------------------------
template<typename ContainerType, typename ConditionType=void>
struct MartyFormatValueGetter;

//----------------------------------------------------------------------------
// Версия для контейнеров, у которых есть find как по строке, так и по индексу (find_by_pos), а элементы имеют second_type и соотв поле second
template<typename ContainerType>
struct MartyFormatValueGetter< ContainerType
                             , typename std::enable_if< utils::has_string_find<ContainerType>::value 
                                                     && utils::has_find_by_pos<ContainerType>::value
                                                     && utils::has_end<ContainerType>::value // Сравнивать с результатом find
                                                     && utils::has_second_type<typename ContainerType::value_type>::value
                                                     // && is_range<ContainerType>::value
                                                      >::type
                             >
{
    using value_type = typename ContainerValueTypeDeducer<ContainerType>::value_type;

    const value_type& operator()(const ContainerType &container, std::string argId, std::size_t &idxDefault) const
    {
        std::size_t szIdx = std::size_t(-1);

        argId = utils::trim_copy(argId);
        if (argId.empty())
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(std::stoul(argId, 0, 10));
            }
            catch(...)
            {}
        }
        
        if (szIdx!=std::size_t(-1))
        {
            auto it = container.find_by_pos(szIdx);
            if (it==container.end())
                throw argid_out_of_range("integer argId is out of range");

            return it->second;
        }

        auto it = container.find(argId);
        if (it==container.end())
            throw argid_not_found("string argId not found");

        return it->second;
    }

}; // struct MartyFormatValueGetter


//----------------------------------------------------------------------------
// Версия для контейнеров, у которых есть find как по строке, так и по индексу (find_by_pos), а элементы не имеют second_type
template<typename ContainerType>
struct MartyFormatValueGetter< ContainerType
                             , typename std::enable_if< utils::has_string_find<ContainerType>::value 
                                                     && utils::has_find_by_pos<ContainerType>::value
                                                     && utils::has_end<ContainerType>::value // Сравнивать с результатом find
                                                     && !utils::has_second_type<typename ContainerType::value_type>::value
                                                     // && is_range<ContainerType>::value
                                                      >::type
                             >
{
    using value_type = typename ContainerValueTypeDeducer<ContainerType>::value_type;

    const value_type& operator()(const ContainerType &container, std::string argId, std::size_t &idxDefault) const
    {
        std::size_t szIdx = std::size_t(-1);

        argId = utils::trim_copy(argId);
        if (argId.empty())
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(std::stoul(argId, 0, 10));
            }
            catch(...)
            {}
        }

        if (szIdx!=std::size_t(-1))
        {
            auto it = container.find_by_pos(szIdx);
            if (it==container.end())
                throw argid_out_of_range("integer argId is out of range");

            return *it;
        }

        auto it = container.find(argId);
        if (it==container.end())
            throw argid_not_found("string argId not found");

        return *it;
    }

}; // struct MartyFormatValueGetter


//----------------------------------------------------------------------------
// Версия для std::map<std::string, ...> и совместимых контейнеров
template<typename ContainerType>
struct MartyFormatValueGetter< ContainerType
                             , typename std::enable_if< utils::has_string_find<ContainerType>::value
                                                     && utils::is_range<ContainerType>::value
                                                     && utils::has_size<ContainerType>::value
                                                     && utils::has_second_type<typename ContainerType::value_type>::value
                                                      >::type
                             >
{
    using value_type = typename ContainerValueTypeDeducer<ContainerType>::value_type;

    const value_type& operator()(const ContainerType &container, std::string argId, std::size_t &idxDefault) const
    {
        std::size_t szIdx = std::size_t(-1);

        argId = utils::trim_copy(argId);
        if (argId.empty())
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(std::stoul(argId, 0, 10));
            }
            catch(...)
            {}
        }

        if (szIdx!=std::size_t(-1))
        {
            if (szIdx>=container.size())
                throw argid_out_of_range("integer argId is out of range");

            auto it = container.begin();
            advance(it, std::ptrdiff_t(szIdx));

            return it->second;
        }

        auto it = container.find(argId);
        if (it==container.end())
            throw argid_not_found("string argId not found");

        return it->second;
    }

}; // struct MartyFormatValueGetter

//----------------------------------------------------------------------------
// Версия для std::vector< std::pair<...> > и совместимых контейнеров
template<typename ContainerType>
struct MartyFormatValueGetter< ContainerType
                             , typename std::enable_if< !utils::has_string_find<ContainerType>::value
                                                     && utils::is_range<ContainerType>::value
                                                     && !utils::has_find_by_pos<ContainerType>::value
                                                     && utils::has_first_type <typename ContainerType::value_type>::value
                                                     && utils::has_second_type<typename ContainerType::value_type>::value
                                                      >::type
                             >
{
    using value_type = typename ContainerValueTypeDeducer<ContainerType>::value_type;

    const value_type& operator()(const ContainerType &container, std::string argId, std::size_t &idxDefault) const
    {
        std::size_t szIdx = std::size_t(-1);

        argId = utils::trim_copy(argId);
        if (argId.empty())
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(std::stoul(argId, 0, 10));
            }
            catch(...)
            {}
        }

        if (szIdx!=std::size_t(-1))
        {
            if (szIdx>=container.size())
                throw argid_out_of_range("integer argId is out of range");

            auto it = container.begin();
            advance(it, std::ptrdiff_t(szIdx));

            return it->second;
        }

        for(const auto &kv : container)
        {
            if (kv.first==argId)
                return kv.second;
        }

        throw argid_not_found("string argId not found");
    }

}; // struct MartyFormatValueGetter

//----------------------------------------------------------------------------
// Версия для std::vector< ... > и совместимых контейнеров
template<typename ContainerType>
struct MartyFormatValueGetter< ContainerType
                             , typename std::enable_if< utils::is_range<ContainerType>::value
                                                     && !utils::has_find_by_pos<ContainerType>::value
                                                     && !utils::has_first_type <typename ContainerType::value_type>::value
                                                     && !utils::has_second_type<typename ContainerType::value_type>::value
                                                      >::type
                             >
{
    using value_type = typename ContainerValueTypeDeducer<ContainerType>::value_type;

    const value_type& operator()(const ContainerType &container, std::string argId, std::size_t &idxDefault) const
    {
        std::size_t szIdx = std::size_t(-1);

        argId = utils::trim_copy(argId);
        if (argId.empty())
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(std::stoul(argId, 0, 10));
            }
            catch(...)
            {}
        }

        if (szIdx!=std::size_t(-1))
        {
            if (szIdx>=container.size())
                throw argid_out_of_range("integer argId is out of range");

            auto it = container.begin();
            advance(it, std::ptrdiff_t(szIdx));

            return *it;
        }

        throw named_argid_not_supported("named argIds are not supported");
    }

}; // struct MartyFormatValueGetter

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename ArgumentVariantType=FormatArgumentVariant
        , typename VectorType=std::vector<ArgumentVariantType>
        , typename MapType=std::unordered_map<std::string, std::size_t>
        >
class BasicArgs
{

public:

    using value_type  = ArgumentVariantType;
    using vector_type = VectorType;
    using map_type    = MapType;

    using size_type               = typename vector_type::size_type             ;
    using difference_type         = typename vector_type::difference_type       ;
    using reference               = typename vector_type::reference             ;
    using const_reference         = typename vector_type::const_reference       ;
    using pointer                 = typename vector_type::pointer               ;
    using const_pointer	          = typename vector_type::const_pointer         ;
    using iterator                = typename vector_type::iterator              ;
    using const_iterator          = typename vector_type::const_iterator        ;
    using reverse_iterator        = typename vector_type::reverse_iterator      ;
    using const_reverse_iterator  = typename vector_type::const_reverse_iterator;

    using key_type                = typename map_type::key_type;


protected:

    bool          m_caseIgnore = true;
    map_type      m_nameMap;
    vector_type   m_values ;

    key_type caseConvert(const key_type &k) const
    {
        return m_caseIgnore ? utils::tolower_copy(k) : k;
    }

    BasicArgs& addArg(const value_type &v)
    {
        m_values.emplace_back(v);
        return *this;
    }

    BasicArgs& addArg(const key_type &k_, const value_type &v)
    {
        key_type k = caseConvert(k_);
        auto nameIt = m_nameMap.find(caseConvert(k));
        if (nameIt!=m_nameMap.end())
            throw argid_already_exist("argId '" + k + " already exist");

        m_nameMap.insert(typename map_type::value_type{k, m_values.size()});
        m_values.emplace_back(v);

        return *this;
    }


public:

    BasicArgs(bool caseIgnore=true) : m_caseIgnore(caseIgnore) {}
    BasicArgs(const BasicArgs &) = default;
    BasicArgs(BasicArgs &&) = default;
    BasicArgs& operator=(const BasicArgs &) = default;
    BasicArgs& operator=(BasicArgs &&) = default;

    const_iterator begin () const { return m_values.begin(); }
    const_iterator end   () const { return m_values.end  (); }
    const_iterator cbegin() const { return m_values.begin(); }
    const_iterator cend  () const { return m_values.end  (); }

    std::size_t size() const
    {
        return m_values.size();
    }

    bool empty() const
    {
        return m_values.size();
    }

    const_iterator find(const key_type &k) const
    {
        auto nameIt = m_nameMap.find(caseConvert(k));
        if (nameIt==m_nameMap.end())
            return m_values.end();

        auto idx = nameIt->second;
        if (idx>=m_values.size())
            return m_values.end();

        return m_values.begin() + std::ptrdiff_t(idx);
    }
    
    const_iterator find_by_pos(std::size_t idx) const
    {
        if (idx>=m_values.size())
            return m_values.end();

        return m_values.begin() + std::ptrdiff_t(idx);
    }

    template<typename T> BasicArgs& arg(T t)
    {
        return addArg(value_type{t});
    }

    template<typename T> BasicArgs& arg(const char* k, T t)
    {
        return addArg(key_type{k}, value_type{t});
    }

    template<typename T> BasicArgs& arg(const key_type &k, T t)
    {
        return addArg(k, value_type{t});
    }

    BasicArgs& arg()
    {
        return addArg(value_type{0});
    }

}; // class BasicArgs

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StringType> inline
StringType martyFormatSimpleConvertToString(bool b)
{
    return StringType(b ? "true" : "false" );
}

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType martyFormatSimpleConvertToString(char ch)
{
    return StringType(1, ch);
}

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
// template<typename StringType, typename IntType> inline
// std::string martyFormatSimpleConvertToString( typename std::enable_if< std::is_integral<IntType>::value, IntType >::type i)
// {
//     return martyFormatSimpleConvertToString<StringType>(std::to_string(unsigned(i)).c_str());
// }

//----------------------------------------------------------------------------
// #if 0
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(std::uint8_t i)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string(unsigned(i)).c_str());
}

template<typename StringType> inline
std::string martyFormatSimpleConvertToString(std::int8_t i)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string(int(i)).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(std::uint16_t i)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string(unsigned(i)).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(std::int16_t i)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string(int(i)).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(std::uint32_t i)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned long)(i)).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(std::int32_t i)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string(long(i)).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(std::uint64_t i)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string((unsigned long long)(i)).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(std::int64_t i)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string((long long)(i)).c_str());
}
// #endif
//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(float f)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string(f).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(double d)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string(d).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(long double d)
{
    return martyFormatSimpleConvertToString<StringType>(std::to_string(d).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(const std::wstring &str)
{
    return martyFormatSimpleConvertToString<StringType>(marty::utf::string_from_wstring(str).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(const wchar_t *str)
{
    return martyFormatSimpleConvertToString<StringType>(marty::utf::string_from_wstring(str?std::wstring(str):std::wstring()).c_str());
}

//----------------------------------------------------------------------------
template<typename StringType> inline
std::string martyFormatSimpleConvertToString(const marty::Decimal &d)
{
    return martyFormatSimpleConvertToString<StringType>(to_string(d).c_str());
}

//----------------------------------------------------------------------------



                // , marty::Decimal



//----------------------------------------------------------------------------
template< typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, bool b)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(b);
}

//----------------------------------------------------------------------------
template< typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, char ch)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(ch);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename StringType, typename IntType >
StringType martyFormatValueFormatInt(const FormattingOptions &formattingOptions, IntType v, size_t valSize)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(valSize);
    return martyFormatSimpleConvertToString<StringType>(v);
}

template< typename StringType, typename IntType >
StringType martyFormatValueFormatUnsigned(const FormattingOptions &formattingOptions, IntType v, size_t valSize)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(valSize);
    return martyFormatSimpleConvertToString<StringType>(v);
}

template< typename StringType, typename FloatType >
StringType martyFormatValueFormatFloat(const FormattingOptions &formattingOptions, FloatType v)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(v);
}

//----------------------------------------------------------------------------
template< typename StringType=std::string >
StringType martyFormatValueFormatString(const FormattingOptions &formattingOptions, const std::string &str)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(str);
}

//----------------------------------------------------------------------------
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, std::int8_t   v) { return martyFormatValueFormatInt<StringType>(formattingOptions, v, sizeof(v)); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, std::int16_t  v) { return martyFormatValueFormatInt<StringType>(formattingOptions, v, sizeof(v)); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, std::int32_t  v) { return martyFormatValueFormatInt<StringType>(formattingOptions, v, sizeof(v)); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, std::int64_t  v) { return martyFormatValueFormatInt<StringType>(formattingOptions, v, sizeof(v)); }

template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, std::uint8_t  v) { return martyFormatValueFormatUnsigned<StringType>(formattingOptions, v, sizeof(v)); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, std::uint16_t v) { return martyFormatValueFormatUnsigned<StringType>(formattingOptions, v, sizeof(v)); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, std::uint32_t v) { return martyFormatValueFormatUnsigned<StringType>(formattingOptions, v, sizeof(v)); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, std::uint64_t v) { return martyFormatValueFormatUnsigned<StringType>(formattingOptions, v, sizeof(v)); }

//----------------------------------------------------------------------------
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, float v)       { return martyFormatValueFormatFloat<StringType>(formattingOptions, v); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, double v)      { return martyFormatValueFormatFloat<StringType>(formattingOptions, v); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, long double v) { return martyFormatValueFormatFloat<StringType>(formattingOptions, v); }

//----------------------------------------------------------------------------
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const std::string &str)  { return martyFormatValueFormatString<StringType>(formattingOptions, str); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const char* str)         { return martyFormatValueFormat<StringType>(formattingOptions, str ? std::string(str) : std::string()); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const std::wstring &str) { return martyFormatValueFormat<StringType>(formattingOptions, marty::utf::string_from_wstring(str)); }
template< typename StringType=std::string > StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const wchar_t* str)      { return martyFormatValueFormat<StringType>(formattingOptions, str ? std::wstring(str) : std::wstring()); }

//----------------------------------------------------------------------------
template< typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const marty::Decimal &d)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(d);
}

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
template< typename IntType
        , typename std::enable_if< ( std::is_integral<IntType>::value
                                   )
                                 , bool
                                 >::type = true
        >
int convertFormatArgumentVariantValueToInt(IntType i)
{
    return int(i);
}

//----------------------------------------------------------------------------
template< typename IntType
        , typename std::enable_if< ( !std::is_integral<IntType>::value
                                   )
                                 , bool
                                 >::type = true
        >
int convertFormatArgumentVariantValueToInt(IntType i)
{
    MARTY_ARG_USED(i);
    throw invalid_argument_type("invalid argument type (required any kind of integer)");
}

//----------------------------------------------------------------------------
template<typename VariantType>
int convertFormatArgumentVariantToInt(VariantType v)
{
    return std::visit( [](auto && a) -> int
                       {
                           return convertFormatArgumentVariantValueToInt(a);
                       }
                     , v
                     );
}
//----------------------------------------------------------------------------





// auto val = std::visit([](auto && arg) -> std::optional<uint64_t)
// {
//     if constexpr(std::numeric_limits<decltype(arg)>::is_integer)
//         return arg;
//     return std::nullopt}, myVariant);


    // template< typename IntType >
    // static
    // typename std::enable_if<std::is_signed<IntType>::value,  /* typename */  StringType>::type
    // formatIntDecimal(IntType intVal, bool showSign = false)
    // {
    // typename std::enable_if<std::is_unsigned<UnsignedType>::value,  /* typename  */ StringType >::type

// template< typename EnumType, typename std::enable_if< (!std::is_enum<EnumType>::value
//                                                     &&  std::is_integral<EnumType>::value
//                                                       )
//                                                     , bool
//                                                     >::type = true
//         > inline




// using FormatArgumentVariant = std::variant< char
//                                           , std::int8_t
//                                           , std::uint8_t
//                                           , std::int16_t
//                                           , std::uint16_t
//                                           , std::int32_t
//                                           , std::uint32_t
//                                           , std::int64_t
//                                           , std::uint64_t
//                                           , float
//                                           , double
//                                           , long double
//                                           , const char*
//                                           , const wchar_t*
//                                           , std::string
//                                           , std::wstring
//                                           , marty::Decimal
//                                           >;



//----------------------------------------------------------------------------
using Args = BasicArgs<FormatArgumentVariant, std::vector<FormatArgumentVariant>, std::unordered_map<std::string, std::size_t> >;


template< typename StringType = std::string
        , typename ArgsType   = Args
        >
StringType formatMessageImpl( const StringType &fmt
                            , const ArgsType   &args
                            , FormattingFlags  formattingFlags=FormattingFlags::all
                            )
{
    using ContainerType = ArgsType;

    std::size_t argIdx = 0;

    auto formatHandler = [&](marty::format::FormattingOptions formattingOptions)
    {
        using value_type = typename ContainerValueTypeDeducer<ArgsType>::value_type;

        // typename ContainerType::value_type valToFormat = typename ContainerType::value_type("<ERR>");
        value_type valToFormat = value_type{"<ERR>"};

        try
        {
            valToFormat = MartyFormatValueGetter<ContainerType>()(args, formattingOptions.argId, argIdx);
        }
        catch(const base_error &)
        {
            if ((formattingFlags&FormattingFlags::ignoreArgumentErrors)==0) // Ошибки поиска аргументов не игнорируем
                throw; // Прокидываем исключение выше
        }

        if ((formattingOptions.optionsFlags&FormattingOptionsFlags::fieldWidthIndirectTaken)==FormattingOptionsFlags::fieldWidthIndirectTaken)
        {
            // Ширина задана, и задана ссылкой
            try
            {
                auto widthVal = MartyFormatValueGetter<ContainerType>()(args, formattingOptions.fieldWidthRef, argIdx);
                formattingOptions.fieldWidth = convertFormatArgumentVariantToInt(widthVal);
            }
            catch(const base_error &)
            {
                if ((formattingFlags&FormattingFlags::ignoreWidthIndirectErrors)==0) // Ошибки поиска аргументов не игнорируем
                    throw; // Прокидываем исключение выше

                formattingOptions.optionsFlags &= ~FormattingOptionsFlags::fieldWidthTaken; // При ошибке просто выставляем, что ширина не задана
            }

            formattingOptions.optionsFlags &= ~FormattingOptionsFlags::fieldWidthIndirect; // Сбрасываем признак, что ширина косвенная

        }

        if ((formattingOptions.optionsFlags&FormattingOptionsFlags::precisionIndirectTaken)==FormattingOptionsFlags::precisionIndirectTaken)
        {
            // Точность задана, и задана ссылкой
            try
            {
                auto precisionVal = MartyFormatValueGetter<ContainerType>()(args, formattingOptions.precisionRef, argIdx);
                formattingOptions.precision = convertFormatArgumentVariantToInt(precisionVal);
            }
            catch(const base_error &)
            {
                if ((formattingFlags&FormattingFlags::ignorePrecisionIndirectErrors)==0) // Ошибки поиска аргументов не игнорируем
                    throw; // Прокидываем исключение выше

                formattingOptions.optionsFlags &= ~FormattingOptionsFlags::precisionTaken; // При ошибке просто выставляем, что точность не задана
            }

            formattingOptions.optionsFlags &= ~FormattingOptionsFlags::precisionIndirect; // Сбрасываем признак, что точность косвенная

        }

        try
        {
            return std::visit( [&](auto && a) -> StringType
                               {
                                   return martyFormatValueFormat<StringType>(formattingOptions, a);
                               }
                             , valToFormat
                             );
        }
        catch(const base_error &)
        {
            if ((formattingFlags&FormattingFlags::ignoreFormattingErrors)==0)
                throw; // Прокидываем исключение выше
        }

        return std::visit( [](auto && a) -> StringType
                           {
                               return martyFormatSimpleConvertToString<StringType>(a);
                           }
                         , valToFormat
                         );

    };

    return processFormatStringImpl(fmt, formatHandler, (formattingFlags&FormattingFlags::ignoreFormatStringErrors)!=0);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename StringType = std::string
        , typename ArgsType   = Args
        >
StringType formatMessage( const StringType &fmt
                        , const ArgsType   &args
                        , FormattingFlags  formattingFlags=FormattingFlags::all
                        )
{
    return formatMessageImpl<StringType, ArgsType>(fmt, args, formattingFlags);
}

//----------------------------------------------------------------------------
template< typename ArgsType = Args >
std::string formatMessage( const char *fmt
                         , const ArgsType   &args
                         , FormattingFlags  formattingFlags=FormattingFlags::all
                         )
{
    return formatMessageImpl<std::string, ArgsType>(fmt, args, formattingFlags);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename StringType = std::string >
StringType formatMessage( const StringType                              &fmt
                        , std::initializer_list<FormatArgumentVariant>  &&args
                        , FormattingFlags                               formattingFlags=FormattingFlags::all
                        )
{
    using ArgsType = std::initializer_list<FormatArgumentVariant>;
    return formatMessageImpl<StringType, ArgsType>(fmt, args, formattingFlags);
}

//----------------------------------------------------------------------------
inline
std::string formatMessage( const char                                    *fmt
                         , std::initializer_list<FormatArgumentVariant>  &&args
                         , FormattingFlags                               formattingFlags=FormattingFlags::all
                         )
{
    using ArgsType = std::initializer_list<FormatArgumentVariant>;
    return formatMessageImpl<std::string, ArgsType>(fmt, args, formattingFlags);
}


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace format
} // namespace marty
// marty::format::
// #include "marty_format/marty_format.h"






