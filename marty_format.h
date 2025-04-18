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
//#! BasicArgs
template< typename ArgumentVariantType=FormatArgumentVariant
        , typename VectorType=std::vector<ArgumentVariantType>
        , typename MapType=std::unordered_map<std::string, std::size_t>
        >
class BasicArgs
//#!
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

    //#! BasicArgsCtor
    BasicArgs(bool caseIgnore=true)
    : m_caseIgnore(caseIgnore)
    {}
    //#! 

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

    //#! BasicArgs_Find
    const_iterator find(const key_type &k) const
    //#!
    {
        auto nameIt = m_nameMap.find(caseConvert(k));
        if (nameIt==m_nameMap.end())
            return m_values.end();

        auto idx = nameIt->second;
        if (idx>=m_values.size())
            return m_values.end();

        return m_values.begin() + std::ptrdiff_t(idx);
    }
    
    const_iterator find(const char* b, const char* e) const
    {
        return find(key_type(b, e));
    }

    //#! BasicArgs_FindByPos
    const_iterator find_by_pos(std::size_t idx) const
    //#!
    {
        if (idx>=m_values.size())
            return m_values.end();

        return m_values.begin() + std::ptrdiff_t(idx);
    }

    //#! BasicArgs_arg_T
    template<typename T> BasicArgs& arg(T t)
    //#!
    {
        return addArg(value_type{t});
    }

    //#! BasicArgs_arg_CharK_T
    template<typename T> BasicArgs& arg(const char* k, T t)
    //#!
    {
        return addArg(key_type{k}, value_type{t});
    }

    //#! BasicArgs_arg_KeyK_T
    template<typename T> BasicArgs& arg(const key_type &k, T t)
    //#!
    {
        return addArg(k, value_type{t});
    }

    //#! BasicArgs_arg_empty
    BasicArgs& arg()
    //#!
    {
        return addArg(value_type{0});
    }

}; // class BasicArgs

//----------------------------------------------------------------------------



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



//----------------------------------------------------------------------------
template<typename StringType> inline StringType martyFormatSimpleConvertToString(bool b)                  { return StringType(b ? "true" : "false" ); }
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

template<typename StringType> inline StringType martyFormatSimpleConvertToString(FormatValueFilter f)
{ 
    return martyFormatSimpleConvertToString<StringType>("filter: ") + martyFormatSimpleConvertToString<StringType>(typeid(f).name());
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatInt(const FormattingOptions &formattingOptions, IntType v, size_t valSize);

template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatUnsigned(const FormattingOptions &formattingOptions, IntType v, size_t valSize);

template< typename WidthCalculator, typename StringType, typename FloatType >
StringType martyFormatValueFormatFloat(const FormattingOptions &formattingOptions, FloatType v);

template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormatString(const FormattingOptions &formattingOptions, const std::string &str);

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, bool b)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(b);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, char ch)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(ch);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatInt(const FormattingOptions &formattingOptions, IntType v, size_t valSize)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(valSize);
    return martyFormatSimpleConvertToString<StringType>(v);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename IntType >
StringType martyFormatValueFormatUnsigned(const FormattingOptions &formattingOptions, IntType v, size_t valSize)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(valSize);
    return martyFormatSimpleConvertToString<StringType>(v);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType, typename FloatType >
StringType martyFormatValueFormatFloat(const FormattingOptions &formattingOptions, FloatType v)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(v);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType >
StringType martyFormatValueFormatString(const FormattingOptions &formattingOptions, const std::string &str)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(str);
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
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, const marty::Decimal &d)
{
    MARTY_ARG_USED(formattingOptions);
    return martyFormatSimpleConvertToString<StringType>(d);
}

//----------------------------------------------------------------------------
template< typename WidthCalculator, typename StringType=std::string >
StringType martyFormatValueFormat(const FormattingOptions &formattingOptions, FormatValueFilter filter)
{
    MARTY_ARG_USED(formattingOptions);
    MARTY_ARG_USED(filter);
    throw filter_as_value_error("invalid filter usage: filter used as value");
}

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

    std::size_t argIdx = 0;

    // std::vector<>

// using FormatValueFilter = BasicFormatValueFilter< marty::utf::UtfInputIterator<char>
//                                                 , marty::utf::UtfOutputIterator<char>
//                                                 >;



    auto indexStringConverter = [&](const char* strB, const char* strE, FormatIndexType indexType)
    {
        // return std::size_t(0);
        std::size_t idx = MartyFormatValueIndexGetter<ArgsType>()(args, strB, strE /* argId */ , argIdx);

        if (indexType==FormatIndexType::filterRef)
        {
            // Надо проверить, является ли аргумент фильтром, и вообще, был ли он найден
            // если это фильтр, пытаемся его скопировать в вектор фильтров
        }

        return idx;
    };


    auto formatHandler = [&](marty::format::FormattingOptions formattingOptions)
    {
        using value_type = typename ContainerValueTypeDeducer<ArgsType>::value_type;

        formattingOptions.formattingFlags = formattingFlags;

        // typename ContainerType::value_type valToFormat = typename ContainerType::value_type("<ERR>");
        value_type valToFormat = value_type{"<ERR>"};

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

    return processFormatStringImpl<StringType, typename StringType::const_iterator>(fmt.begin(), fmt.end(), formatHandler, indexStringConverter, (formattingFlags&FormattingFlags::ignoreFormatStringErrors)!=0);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//#! formatMessageGeneric
template< typename StringType      = std::string
        , typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        >
StringType formatMessage( const StringType &fmt
                        , const ArgsType   &args
                        , FormattingFlags  formattingFlags=FormattingFlags::all
                        )
//#!
{
    return formatMessageImpl<StringType, ArgsType, WidthCalculator>(fmt, args, formattingFlags);
}

//----------------------------------------------------------------------------
//#! formatMessageGenericConstCharPtr
template< typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        >
std::string formatMessage( const char *fmt
                         , const ArgsType   &args
                         , FormattingFlags  formattingFlags=FormattingFlags::all
                         )
//#!
{
    return formatMessageImpl<std::string, ArgsType, WidthCalculator>(fmt, args, formattingFlags);
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//#! formatMessageInitializerList
using FormatArgumentVariantList = std::initializer_list<FormatArgumentVariant>;

template< typename StringType = std::string
        , typename WidthCalculator = DefaultUtfWidthCalculator
        >
StringType formatMessage( const StringType          &fmt
                        , FormatArgumentVariantList &&args
                        , FormattingFlags           formattingFlags=FormattingFlags::all
                        )
//#!
{
    using ArgsType = std::initializer_list<FormatArgumentVariant>;
    return formatMessageImpl<StringType, ArgsType, WidthCalculator>(fmt, args, formattingFlags);
}

//----------------------------------------------------------------------------
//#! formatMessageInitializerListConstCharPtr
template< typename WidthCalculator = DefaultUtfWidthCalculator
        >
inline
std::string formatMessage( const char                *fmt
                         , FormatArgumentVariantList &&args
                         , FormattingFlags           formattingFlags=FormattingFlags::all
                         )
//#!
{
    using ArgsType = std::initializer_list<FormatArgumentVariant>;
    return formatMessageImpl<std::string, ArgsType, WidthCalculator>(fmt, args, formattingFlags);
}


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace format
} // namespace marty
// marty::format::
// #include "marty_format/marty_format.h"






