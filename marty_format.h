/*!
    \file
    \brief Форматирование типа std::format в рантайме
 */
#pragma once

#include "defs.h"
#include "exceptions.h"
#include "marty_format_types.h"
#include "utils.h"

//
#include <algorithm>
#include <array>
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
#include <typeinfo>
#include <initializer_list>
#include<iterator>

//
#include "undef_min_max.h"



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

// std::size_t (const char* str, FormatIndexType indexType)

#include "internal_processFormatStringImpl.h"


//----------------------------------------------------------------------------



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



//----------------------------------------------------------------------------
template<typename ContainerType, typename ConditionType=void>
struct MartyFormatValueIndexGetter;

//----------------------------------------------------------------------------
// Версия для контейнеров, у которых есть find как по строке, так и по индексу (find_by_pos), а элементы имеют second_type и соотв поле second
template<typename ContainerType>
struct MartyFormatValueIndexGetter< ContainerType
                                  , typename std::enable_if< utils::has_char_ptr_find<ContainerType>::value 
                                                          && utils::has_find_by_pos<ContainerType>::value
                                                          && utils::has_end<ContainerType>::value // Сравнивать с результатом find
                                                          && utils::has_second_type<typename ContainerType::value_type>::value
                                                          // && is_range<ContainerType>::value
                                                           >::type
                                  >
{
    std::size_t operator()(const ContainerType &container, const char* argIdB, const char* argIdE, std::size_t &idxDefault) const
    {
        std::size_t szIdx = std::size_t(-1);

        argIdB = utils::ltrim_copy(argIdB, argIdE);
        argIdE = utils::rtrim_copy(argIdB, argIdE);
        if (argIdB==argIdE)
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(utils::charRangeToUnsigned(argIdB, argIdE));
            }
            catch(...)
            {}
        }
        

        if (szIdx!=std::size_t(-1))
            return szIdx;

        auto it = container.find(typename ContainerType::key_type(argIdB, argIdE));
        if (it==container.end())
            return std::size_t(-1);

        return std::size_t(std::distance(container.begin(), it));
    }
};

//----------------------------------------------------------------------------
// Версия для контейнеров, у которых есть find как по строке, так и по индексу (find_by_pos), а элементы не имеют second_type
template<typename ContainerType>
struct MartyFormatValueIndexGetter< ContainerType
                                  , typename std::enable_if< utils::has_char_ptr_find<ContainerType>::value 
                                                          && utils::has_find_by_pos<ContainerType>::value
                                                          && utils::has_end<ContainerType>::value // Сравнивать с результатом find
                                                          && !utils::has_second_type<typename ContainerType::value_type>::value
                                                          // && is_range<ContainerType>::value
                                                           >::type
                                  >
{
    std::size_t operator()(const ContainerType &container, const char* argIdB, const char* argIdE, std::size_t &idxDefault) const
    {
        std::size_t szIdx = std::size_t(-1);

        argIdB = utils::ltrim_copy(argIdB, argIdE);
        argIdE = utils::rtrim_copy(argIdB, argIdE);
        if (argIdB==argIdE)
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(utils::charRangeToUnsigned(argIdB, argIdE));
            }
            catch(...)
            {}
        }

        if (szIdx!=std::size_t(-1))
            return szIdx;

        // {
        //     // auto it = container.find_by_pos(szIdx);
        //     // if (it==container.end())
        //     //     throw argid_out_of_range("integer argId is out of range");
        //     //  
        //     // return std::size_t(std::distance(container.begin(), it));
        // }

        auto it = container.find(typename ContainerType::key_type(argIdB, argIdE));
        if (it==container.end())
            return std::size_t(-1);

        return std::size_t(std::distance(container.begin(), it));
    }
};

//----------------------------------------------------------------------------
// Версия для std::map<std::string, ...> и совместимых контейнеров
template<typename ContainerType>
struct MartyFormatValueIndexGetter< ContainerType
                                  , typename std::enable_if< utils::has_char_ptr_find<ContainerType>::value
                                                          && utils::is_range<ContainerType>::value
                                                          && utils::has_size<ContainerType>::value
                                                          && utils::has_second_type<typename ContainerType::value_type>::value
                                                           >::type
                                  >
{
    std::size_t operator()(const ContainerType &container, const char* argIdB, const char* argIdE, std::size_t &idxDefault) const
    {
        std::size_t szIdx = std::size_t(-1);

        argIdB = utils::ltrim_copy(argIdB, argIdE);
        argIdE = utils::rtrim_copy(argIdB, argIdE);
        if (argIdB==argIdE)
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(utils::charRangeToUnsigned(argIdB, argIdE));
            }
            catch(...)
            {}
        }

        if (szIdx!=std::size_t(-1))
            return szIdx;

        // {
        //     if (szIdx>=container.size())
        //         throw argid_out_of_range("integer argId is out of range");
        //  
        //     auto it = container.begin();
        //     // advance(it, std::ptrdiff_t(szIdx));
        //  
        //     return it->second;
        // }

        auto it = container.find(typename ContainerType::key_type(argIdB, argIdE));
        if (it==container.end())
            return std::size_t(-1);
            //throw argid_not_found("string argId not found");

        return std::size_t(std::distance(container.begin(), it));
    }
};

//----------------------------------------------------------------------------
// Версия для std::vector< std::pair<...> > и совместимых контейнеров
template<typename ContainerType>
struct MartyFormatValueIndexGetter< ContainerType
                                  , typename std::enable_if< !utils::has_char_ptr_find<ContainerType>::value
                                                          && utils::is_range<ContainerType>::value
                                                          && !utils::has_find_by_pos<ContainerType>::value
                                                          && utils::has_first_type <typename ContainerType::value_type>::value
                                                          && utils::has_second_type<typename ContainerType::value_type>::value
                                                           >::type
                                  >
{
    std::size_t operator()(const ContainerType &container, const char* argIdB, const char* argIdE, std::size_t &idxDefault) const
    {
        std::size_t szIdx = std::size_t(-1);

        argIdB = utils::ltrim_copy(argIdB, argIdE);
        argIdE = utils::rtrim_copy(argIdB, argIdE);
        if (argIdB==argIdE)
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(utils::charRangeToUnsigned(argIdB, argIdE));
            }
            catch(...)
            {}
        }

        if (szIdx!=std::size_t(-1))
            return szIdx;

        // {
        //     if (szIdx>=container.size())
        //         throw argid_out_of_range("integer argId is out of range");
        //  
        //     auto it = container.begin();
        //     advance(it, std::ptrdiff_t(szIdx));
        //  
        //     return it->second;
        // }

        using value_type = typename ContainerType::value_type;

        for(const auto &kv : container)
        {
            ++szIdx;
            if (kv.first==typename value_type::first_type(argIdB, argIdE))
                return szIdx;
                //return kv.second;
        }

        //throw argid_not_found("string argId not found");
        return std::size_t(-1);
    }
};

//----------------------------------------------------------------------------
// Версия для std::vector< ... > и совместимых контейнеров
template<typename ContainerType>
struct MartyFormatValueIndexGetter< ContainerType
                                  , typename std::enable_if< utils::is_range<ContainerType>::value
                                                          && !utils::has_find_by_pos<ContainerType>::value
                                                          && !utils::has_first_type <typename ContainerType::value_type>::value
                                                          && !utils::has_second_type<typename ContainerType::value_type>::value
                                                           >::type
                                  >
{
    std::size_t operator()(const ContainerType &container, const char* argIdB, const char* argIdE, std::size_t &idxDefault) const
    {
        MARTY_ARG_USED(container);

        std::size_t szIdx = std::size_t(-1);

        argIdB = utils::ltrim_copy(argIdB, argIdE);
        argIdE = utils::rtrim_copy(argIdB, argIdE);
        if (argIdB==argIdE)
        {
            szIdx = idxDefault++;
        }
        else
        {
            try
            {
                szIdx = std::size_t(utils::charRangeToUnsigned(argIdB, argIdE));
            }
            catch(...)
            {}
        }

        if (szIdx!=std::size_t(-1))
            return szIdx;

        // {
        //     if (szIdx>=container.size())
        //         throw argid_out_of_range("integer argId is out of range");
        //  
        //     auto it = container.begin();
        //     advance(it, std::ptrdiff_t(szIdx));
        //  
        //     return *it;
        // }

        // throw named_argid_not_supported("named argIds are not supported");

        return std::size_t(-1);
    }
};

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename ContainerType, typename ConditionType=void>
struct MartyFormatValueGetter;

//----------------------------------------------------------------------------
// Версия для контейнеров, у элементов которых есть second_type и мембер second, соответственно
template<typename ContainerType>
struct MartyFormatValueGetter< ContainerType
                             , typename std::enable_if< utils::has_second_type<typename ContainerType::value_type>::value
                                                      >::type
                             >
{
    using value_type = typename ContainerValueTypeDeducer<ContainerType>::value_type;

    const value_type& operator()(const ContainerType &container, std::size_t idx) const
    {
        if (idx==std::size_t(-1))
            throw argid_not_found("argId not found");
        if (idx>=container.size())
            throw argid_out_of_range("argId is out of range");

        auto it = container.begin();
        std::advance(it, idx);
        return it->second;
    }

    const value_type& operator()(const ContainerType &container, std::string argId, std::size_t &idxDefault) const
    {
        return operator()(container, MartyFormatValueIndexGetter<ContainerType>()(container, argId.data(), argId.data()+std::ptrdiff_t(argId.size()), idxDefault));
    }

}; // struct MartyFormatValueGetter

//----------------------------------------------------------------------------
// Версия для контейнеров без second_type и мембера second, соответственно
template<typename ContainerType>
struct MartyFormatValueGetter< ContainerType
                             , typename std::enable_if< !utils::has_second_type<typename ContainerType::value_type>::value
                                                      >::type
                             >
{
    using value_type = typename ContainerValueTypeDeducer<ContainerType>::value_type;

    const value_type& operator()(const ContainerType &container, std::size_t idx) const
    {
        if (idx==std::size_t(-1))
            throw argid_not_found("argId not found");
        if (idx>=container.size())
            throw argid_out_of_range("argId is out of range");

        auto it = container.begin();
        std::advance(it, idx);
        return *it;
    }

    const value_type& operator()(const ContainerType &container, std::string argId, std::size_t &idxDefault) const
    {
        return operator()(container, MartyFormatValueIndexGetter<ContainerType>()(container, argId.data(), argId.data()+std::ptrdiff_t(argId.size()), idxDefault));
    }

}; // struct MartyFormatValueGetter


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#include "internal_processFormatValuesImpl.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename IntType
        , typename std::enable_if< ( std::is_integral<IntType>::value
                                 && !std::is_pointer<IntType>::value
                                 && !utils::is_bool<IntType>::value
                                   )
                                 , bool
                                 >::type = true
        >
int convertFormatArgumentVariantValueToInt(IntType i)
{
    return int(i);
}

template< typename IntType
        , typename std::enable_if< ( !std::is_integral<IntType>::value
                                  || std::is_pointer<IntType>::value
                                  || utils::is_bool<IntType>::value
                                   )
                                 , bool
                                 >::type = true
        >
int convertFormatArgumentVariantValueToInt(IntType i)
{
    MARTY_ARG_USED(i);
    throw invalid_argument_type("invalid argument type (required any kind of int)");
}

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
template< typename IntType
        , typename std::enable_if< ( std::is_integral<IntType>::value
                                 && !std::is_pointer<IntType>::value
                                 && !utils::is_bool<IntType>::value
                                   )
                                 , bool
                                 >::type = true
        >
unsigned convertFormatArgumentVariantValueToUnsigned(IntType i)
{
    return unsigned(i);
}

template< typename IntType
        , typename std::enable_if< ( !std::is_integral<IntType>::value
                                  || std::is_pointer<IntType>::value
                                  || utils::is_bool<IntType>::value
                                   )
                                 , bool
                                 >::type = true
        >
unsigned convertFormatArgumentVariantValueToUnsigned(IntType i)
{
    MARTY_ARG_USED(i);
    throw invalid_argument_type("invalid argument type (required any kind of unsigned)");
}

template<typename VariantType>
unsigned convertFormatArgumentVariantToUnsigned(VariantType v)
{
    return std::visit( [](auto && a) -> unsigned
                       {
                           return convertFormatArgumentVariantValueToUnsigned(a);
                       }
                     , v
                     );
}

//----------------------------------------------------------------------------
template< typename IntType
        , typename std::enable_if< ( std::is_integral<IntType>::value 
                                 && !std::is_pointer<IntType>::value
                                 && !utils::is_bool<IntType>::value
                                   )
                                 , bool
                                 >::type = true
        >
marty::utf::utf32_char_t convertFormatArgumentVariantValueToChar(IntType i)
{
    return marty::utf::utf32_char_t(i);
}

template< typename IntType
        , typename std::enable_if< ( !std::is_integral<IntType>::value
                                  || std::is_pointer<IntType>::value
                                  || utils::is_bool<IntType>::value
                                   )
                                 , bool
                                 >::type = true
        >
marty::utf::utf32_char_t convertFormatArgumentVariantValueToChar(IntType i)
{
    MARTY_ARG_USED(i);
    throw invalid_argument_type("invalid argument type (required any kind of integer or char)");
}

//----------------------------------------------------------------------------
template<typename VariantType>
marty::utf::utf32_char_t convertFormatArgumentVariantToChar(VariantType v)
{
    return std::visit( [](auto && a) -> marty::utf::utf32_char_t
                       {
                           return convertFormatArgumentVariantValueToChar(a);
                       }
                     , v
                     );
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//#! DefaultUtfWidthCalculator
using DefaultUtfWidthCalculator = utils::DefaultUtfWidthCalculator;
//#!

//#! Args
using Args = BasicArgs< FormatArgumentVariant
                      , std::vector<FormatArgumentVariant>
                      , std::unordered_map<std::string, std::size_t>
                      , DefaultUtfWidthCalculator
                      >;
//#!

//#! BasicImpl
template< typename StringType      = std::string
        , typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
StringType formatMessageImpl( const StringType &fmt
                            , const ArgsType   &args
                            , FormattingFlags  formattingFlags=FormattingFlags::all
                            )
//#!
{
    using ContainerType = ArgsType;

    using value_type = typename ContainerValueTypeDeducer<ContainerType>::value_type;

    using Traits = variant_filter_traits<FormatArgumentVariant>;

    static_assert((Traits::has_filter), "Args value_type should contain filter functor");
    using FilterType      = typename Traits::first_filter;
    using FilterTypeArgs  = typename Traits::first_filter_args;

    using FilterInputIteratorType  = typename FilterTypeArgs::input_type ;
    using FilterOutputIteratorType = typename FilterTypeArgs::output_type;

    // static_assert(variant_has_filter_v<value_type>, "Should have filters");

    std::vector<FilterType> filters;

    // struct FiltersClearer
    // {
    //     FiltersClearer(std::vector<FilterType> &v) : m_v(v) {}
    //     ~FiltersClearer() { m_v.clear(); }
    //     std::vector<FilterType> &m_v;
    // };

    std::size_t argIdx = 0;

    // std::vector<>

// using FormatValueFilter = BasicFormatValueFilter< marty::utf::UtfInputIterator<char>
//                                                 , marty::utf::UtfOutputIterator<char>
//                                                 >;



    auto indexStringConverter = [&](const char* strB, const char* strE, FormatIndexType indexType)
    {
        // return std::size_t(0);
        

        if (indexType!=FormatIndexType::filterRef)
        {
            return MartyFormatValueIndexGetter<ArgsType>()(args, strB, strE /* argId */ , argIdx);
        }
        else
        {
            // Надо проверить, является ли аргумент фильтром, и вообще, был ли он найден
            // если это фильтр, пытаемся его скопировать в вектор фильтров

            bool isNone = false;
            auto stdFilter = FilterFactory()(StringType(strB, strE), &isNone);
            if (!isNone)
            {
                std::size_t resIdx = filters.size();
                filters.emplace_back(stdFilter);
                return resIdx;
            }

            //auto userMayBeFilterIdx =  = 
            std::size_t userMayBeFilterIdx = MartyFormatValueIndexGetter<ArgsType>()(args, strB, strE /* argId */ , argIdx);
            try
            {
                value_type userMayBeFilter = MartyFormatValueGetter<ContainerType>()(args, userMayBeFilterIdx);
                if (!std::holds_alternative<FilterType>(userMayBeFilter))
                {
                    if ((formattingFlags&FormattingFlags::ignoreNotFilterErrors)==0) // Ошибки поиска аргументов не игнорируем
                        throw value_as_filter_error("not filter argument used as filter");
                    return std::size_t(-1);
                }

                std::size_t resIdx = filters.size();
                filters.emplace_back(std::get<FilterType>(userMayBeFilter));
                return resIdx;
                
            }
            catch(const base_error &)
            {
                if ((formattingFlags&FormattingFlags::ignoreArgumentErrors)==0) // Ошибки поиска аргументов не игнорируем
                    throw; // Прокидываем исключение выше

                return std::size_t(-1);
            }
        }
    };


    auto formatHandlerImpl = [&](marty::format::FormattingOptions formattingOptions)
    {
        using value_type = typename ContainerValueTypeDeducer<ArgsType>::value_type;

        formattingOptions.formattingFlags = formattingFlags;

        // typename ContainerType::value_type valToFormat = typename ContainerType::value_type("<ERR>");
        value_type valToFormat = value_type{"<ERR>"};

        // auto filtersClearer = FiltersClearer(filters);

        // Если явно задано флагами, что дробную часть надо тоже разделять по разрядам,
        // и разделитель разрядов не задан явно для дробной части
        // то устанавливаем разделитель разрядов такой же, как и для целой части числа
        if ((formattingOptions.formattingFlags&FormattingFlags::fractionalGroupping)!=0 && formattingOptions.fractionalGrouppingChar==0)
            formattingOptions.fractionalGrouppingChar = formattingOptions.grouppingChar;

        try
        {
            valToFormat = MartyFormatValueGetter<ContainerType>()(args, formattingOptions.argIdx);
        }
        catch(const base_error &)
        {
            if ((formattingFlags&FormattingFlags::ignoreArgumentErrors)==0) // Ошибки поиска аргументов не игнорируем
                throw; // Прокидываем исключение выше
        }

        //convertFormatArgumentVariantToChar
        // fillingIndirectTaken

        if ((formattingOptions.optionsFlags&FormattingOptionsFlags::fillingIndirectTaken)==FormattingOptionsFlags::fillingIndirectTaken)
        {
            // Filling char задан, и задана ссылкой
            try
            {
                auto fillingVal = MartyFormatValueGetter<ContainerType>()(args, formattingOptions.fillIdx);
                formattingOptions.fillChar = convertFormatArgumentVariantToChar(fillingVal);
            }
            catch(const base_error &)
            {
                if ((formattingFlags&FormattingFlags::ignoreFillIndirectErrors)==0) // Ошибки поиска аргументов не игнорируем
                    throw; // Прокидываем исключение выше

                formattingOptions.optionsFlags &= ~FormattingOptionsFlags::fillingTaken; // При ошибке просто выставляем, что filling не задан
            }

            formattingOptions.optionsFlags &= ~FormattingOptionsFlags::fillingIndirect; // Сбрасываем признак, что filling косвенный
        }

        if ((formattingOptions.optionsFlags&FormattingOptionsFlags::fieldWidthIndirectTaken)==FormattingOptionsFlags::fieldWidthIndirectTaken)
        {
            // Ширина задана, и задана ссылкой
            try
            {
                auto widthVal = MartyFormatValueGetter<ContainerType>()(args, formattingOptions.width);
                formattingOptions.width = (width_t)convertFormatArgumentVariantToUnsigned(widthVal);
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
                auto precisionVal = MartyFormatValueGetter<ContainerType>()(args, formattingOptions.precision);
                formattingOptions.precision = (width_t)convertFormatArgumentVariantToUnsigned(precisionVal);
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
                                   return martyFormatValueFormat<WidthCalculator, StringType>(formattingOptions, a);
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

    auto formatHandler = [&](marty::format::FormattingOptions formattingOptions)
    {
        StringType formattedValue = formatHandlerImpl(formattingOptions);

        // Тут надо добавить вызов цепочки фильтров

        for(std::size_t idxIdx=0; idxIdx!=formattingOptions.numFilters; ++idxIdx)
        {
            auto fltIdx = formattingOptions.filters[idxIdx];
            if (fltIdx>=filters.size()) // Индекс почему-то вылезает за пределы массива
                continue;

            auto &filter = filters[fltIdx];

            using namespace marty::format::utils;

            auto pFmtStrBegin = rawConstCharPtrFromIterator(formattedValue.data());
            auto pFmtStrEnd   = pFmtStrBegin;
            std::advance(pFmtStrEnd, std::ptrdiff_t(formattedValue.size()));

            StringType filteredVal;

            if constexpr (utils::has_ctor_require_two_pointers<FilterInputIteratorType>)
            {
                filter(FilterInputIteratorType(pFmtStrBegin, pFmtStrEnd), FilterInputIteratorType(pFmtStrEnd), FilterOutputIteratorType(filteredVal));
            }
            else
            {
                filter(FilterInputIteratorType(pFmtStrBegin), FilterInputIteratorType(pFmtStrEnd), FilterOutputIteratorType(filteredVal));
            }

            formattedValue = filteredVal;
        }

        filters.clear(); // очищаем текущий набор фильтров

        return formattedValue;
    };


    return processFormatStringImpl<StringType, typename StringType::const_iterator>(fmt.begin(), fmt.end(), formatHandler, indexStringConverter, (formattingFlags&FormattingFlags::ignoreFormatStringErrors)!=0);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//#! formatMessageGeneric
template< typename StringType      = std::string
        , typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
StringType formatMessage( const StringType &fmt
                        , const ArgsType   &args
                        , FormattingFlags  formattingFlags=FormattingFlags::all
                        )
//#!
{
    return formatMessageImpl<StringType, ArgsType, WidthCalculator, FilterFactory>(fmt, args, formattingFlags);
}

//----------------------------------------------------------------------------
//#! formatMessageGenericConstCharPtr
template< typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
std::string formatMessage( const char *fmt
                         , const ArgsType   &args
                         , FormattingFlags  formattingFlags=FormattingFlags::all
                         )
//#!
{
    return formatMessageImpl<std::string, ArgsType, WidthCalculator, FilterFactory>(fmt, args, formattingFlags);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//#! formatMessageInitializerList
using FormatArgumentVariantList = std::initializer_list<FormatArgumentVariant>;

template< typename StringType = std::string
        , typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
StringType formatMessage( const StringType          &fmt
                        , FormatArgumentVariantList &&args
                        , FormattingFlags           formattingFlags=FormattingFlags::all
                        )
//#!
{
    using ArgsType = std::initializer_list<FormatArgumentVariant>;
    return formatMessageImpl<StringType, ArgsType, WidthCalculator, FilterFactory>(fmt, args, formattingFlags);
}

//----------------------------------------------------------------------------
//#! formatMessageInitializerListConstCharPtr
template< typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
inline
std::string formatMessage( const char                *fmt
                         , FormatArgumentVariantList &&args
                         , FormattingFlags           formattingFlags=FormattingFlags::all
                         )
//#!
{
    using ArgsType = std::initializer_list<FormatArgumentVariant>;
    return formatMessageImpl<std::string, ArgsType, WidthCalculator, FilterFactory>(fmt, args, formattingFlags);
}


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace format
} // namespace marty
// marty::format::
// #include "marty_format/marty_format.h"






