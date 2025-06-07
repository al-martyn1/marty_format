/*!
    \file
    \brief Типы для marty::format
 */
#pragma once

#include "defs.h"
#include "enums.h"
#include "exceptions.h"
#include "locale_info.h"

// 
#include "marty_utf/utf.h"
//
#if defined(USE_MARTY_BIGINT) && USE_MARTY_BIGINT!=0
    #include "marty_bigint/marty_bigint.h"
#endif
#if defined(USE_MARTY_DECIMAL) && USE_MARTY_DECIMAL!=0
    #include "marty_decimal/marty_decimal.h"
#endif

//
#include "utils.h"
//namespace marty_utf {

#include <cstddef>
#include <functional>
#include <string>
#include <type_traits>
#include <tuple>
#include <variant>
#include <vector>
#include <memory>

//
#include "undef_min_max.h"


// #include "marty_format/marty_format.h"
// marty::format::
namespace marty{
namespace format{


using utf32_char_t = marty::utf::utf32_char_t;

// Немножко оптимизируем, и делаем индексы поменьше размером
// чтобы структура FormattingOptions была не такая жирная.
// Даже для больших компов может быть польза,
// а уж во встройке - однозначно

using arg_idx_t = std::uint16_t;
constexpr static const inline arg_idx_t arg_idx_npos = arg_idx_t(-1);

// У нас максимально до восьми фильтров
// constexpr static const arg_idx_t filter_index_flag_predefined = 0x8000u;

// А не может ли поле быть шириной больше 65535?
// Усекаем молча или генерируем исключение?
using width_t   = std::uint16_t ; // For fieldWidth and precision


//! Параметры форматирования
struct FormattingOptions
{
    constexpr static const inline arg_idx_t MaxFilters = 4u;

    arg_idx_t               argIdx         = arg_idx_t(-1);
    arg_idx_t               fillIdx        = arg_idx_t(-1);
    // arg_idx_t               widthIdx       = arg_idx_t(-1);
    // arg_idx_t               precisionIdx   = arg_idx_t(-1);

    FormattingOptionsFlags  optionsFlags   = FormattingOptionsFlags::none;
    width_t                 width          = 0; // auto
    width_t                 precision      = 6; // default precision

    char                    convertChar    = 0;
    char                    alignment      = 0; // '>'; // По левому краю ('<'), по правому краю ('>'), по по центру ('^'). По умолчанию - по правому краю
    utf32_char_t            fillChar       = 0; // utf32_char_t(' '); // Символ заполнения, по умолчанию - пробел.
    utf32_char_t            grouppingChar  = 0; // разделитель разрядов, по умолчанию - 0 (без разделения).
    utf32_char_t            fractionalGrouppingChar = 0; // разделитель разрядов после десятичной точки, по умолчанию - 0 (без разделения).
    char                    typeChar       = 0;   // Символ типа, 0 - auto

    std::size_t             numFilters     = 0;
    arg_idx_t               filters[MaxFilters];

    std::size_t             forceGroupSize = 0; // Для внутреннего использования

    FormattingFlags         formattingFlags = FormattingFlags::none; // Копируем сюда аргумент функции форматирования, чтобы не таскать отдельно

}; // struct FormattingOptions


template<typename StramType>
StramType& operator<<(StramType& oss, const FormattingOptions opts)
{
    oss << "Options  : " << enum_serialize_flags(opts.optionsFlags) << "\n";


    if ((opts.optionsFlags&FormattingOptionsFlags::argIdTaken)==0)
        oss << "ArgId    : " << "auto" << "\n";
    else
        oss << "ArgId    : " << opts.argIdx << "\n";


    if ((opts.optionsFlags&FormattingOptionsFlags::fieldWidthTaken)==0)
        oss << "Width    : " << "auto" << "\n";
    else
    {
        if ((opts.optionsFlags&FormattingOptionsFlags::fieldWidthIndirect)==0)
            oss << "Width    : " << opts.width << "\n";
        else 
            oss << "WidthId  : " << opts.width << "\n";
    }


    if ((opts.optionsFlags&FormattingOptionsFlags::fieldWidthTaken)==0)
        oss << "Precision: " << "auto" << "\n";
    else
    {
        if ((opts.optionsFlags&FormattingOptionsFlags::precisionIndirect)==0)
            oss << "Precision: " << opts.precision << "\n";
        else
            oss << "PrecisionId: " << opts.precision << "\n";
    }


    // if ((opts.optionsFlags&FormattingOptionsFlags::alignTaken)==0)
    //     oss << "Align    : " << "auto" << "\n";
    // else
    {
        oss << "Align    : ";
        switch(opts.alignment)
        {
            case '<': oss << "left"; break;
            case '>': oss << "right"; break;
            case '^': oss << "width"; break;
            default : oss << "unknown"; break;
        }
        oss << "\n";
    }

    oss << "Fill Char: " << opts.fillChar << "\n";

    oss << "Grp Char : " << opts.grouppingChar << "\n";

    oss << "Type Char: " << std::string(1, opts.typeChar) << "\n";


    return oss;
}

//----------------------------------------------------------------------------


// https://ru.wikipedia.org/wiki/Functional_(C%2B%2B)

//#! BasicFormatValueFilter
template<typename InputIteratorType, typename OutputIteratorType>
using BasicFormatValueFilter = std::function< OutputIteratorType( InputIteratorType  // begin
                                                                , InputIteratorType  // end
                                                                , OutputIteratorType
                                                                )
                                            >;
//#!

//#! FormatValueFilter
using FormatValueFilter = BasicFormatValueFilter< marty::utf::UtfInputIterator<char>
                                                , marty::utf::UtfOutputIterator<char>
                                                >;
//#!

//----------------------------------------------------------------------------
struct StdNoneFilter
{
    marty::utf::UtfOutputIterator<char>
    operator()( marty::utf::UtfInputIterator<char>  b
              , marty::utf::UtfInputIterator<char>  e
              , marty::utf::UtfOutputIterator<char> it
              ) const
    {
        for(; b!=e; ++b)
            *it++ = *b;
        return it;
    }

};

//----------------------------------------------------------------------------
struct StdXmlHtmlFilter
{
    marty::utf::UtfOutputIterator<char> copyStr(marty::utf::UtfOutputIterator<char> it, const char *str) const
    {
        while(*str)
            *it++ = (marty::utf::utf32_char_t)*str++;

        return it;
    }

    marty::utf::UtfOutputIterator<char>
    operator()( marty::utf::UtfInputIterator<char>  b
              , marty::utf::UtfInputIterator<char>  e
              , marty::utf::UtfOutputIterator<char> it
              ) const
    {
        using utfch_t = marty::utf::utf32_char_t;

        for(; b!=e; ++b)
        {
            auto ch = *b;

            // &amp;, &lt;, &gt;, &apos;, and &quot;
            switch(ch)
            {
                case utfch_t('&') : it = copyStr(it, "&amp;" ); break;
                case utfch_t('<') : it = copyStr(it, "&lt;"  ); break;
                case utfch_t('>') : it = copyStr(it, "&gt;"  ); break;
                case utfch_t('\''): it = copyStr(it, "&apos;"); break;
                case utfch_t('\"'): it = copyStr(it, "&quot;"); break;
                default:
                    *it++ = ch;
            }
        }

        return it;
    }

};

//----------------------------------------------------------------------------
struct StdSqlFilter
{
    marty::utf::UtfOutputIterator<char>
    operator()( marty::utf::UtfInputIterator<char>  b
              , marty::utf::UtfInputIterator<char>  e
              , marty::utf::UtfOutputIterator<char> it
              ) const
    {
        using utfch_t = marty::utf::utf32_char_t;

        for(; b!=e; ++b)
        {
            auto ch = *b;
            if (ch==utfch_t('\''))
                *it++ = ch;
            *it++ = ch;
        }

        return it;
    }

};

//----------------------------------------------------------------------------
//#! makeStandardFormatValueFilterId
FormatValueFilter makeStandardFormatValueFilter( StdFilterType filterType
                                               , bool *pNoneReturned=0
                                               );
//#!

//#! makeStandardFormatValueFilterStr
template<typename StringType>
FormatValueFilter makeStandardFormatValueFilter( const StringType &filterName
                                               , bool *pNoneReturned=0
                                               );
//#!

FormatValueFilter makeStandardFormatValueFilter(StdFilterType filterType, bool *pNoneReturned)
{
    if (pNoneReturned)
        *pNoneReturned = false;

    switch(filterType)
    {
        case StdFilterType::none    : break;

        case StdFilterType::xml     : [[fallthrough]];
        case StdFilterType::xmlText : [[fallthrough]];
        case StdFilterType::xmlAttr : [[fallthrough]];
        case StdFilterType::html    : [[fallthrough]];
        case StdFilterType::htmlText: [[fallthrough]];
        case StdFilterType::htmlAttr: return StdXmlHtmlFilter();

        case StdFilterType::sql     : return StdSqlFilter();

        case StdFilterType::invalid : [[fallthrough]];
        default: {}
    }

    if (!pNoneReturned)
        throw unknown_value_filter("unknown value filter");

    *pNoneReturned = true;

    return StdNoneFilter();
   
}

template<typename StringType>
FormatValueFilter makeStandardFormatValueFilter(const StringType &filterName, bool *pNoneReturned)
{
    auto e = enum_deserialize(filterName, StdFilterType::unknown);
    return makeStandardFormatValueFilter(e, pNoneReturned);
}

//----------------------------------------------------------------------------
//#! StdFilterFactory
struct StdFilterFactory
{
    template<typename StringType>
    FormatValueFilter operator()(StdFilterType filterType) const
    {
        return makeStandardFormatValueFilter(filterType);
    }

    template<typename StringType>
    FormatValueFilter operator()(StdFilterType filterType, bool *pNoneReturned) const
    {
        return makeStandardFormatValueFilter(filterType, pNoneReturned);
    }

    template<typename StringType>
    FormatValueFilter operator()(const StringType &filterName) const
    {
        return makeStandardFormatValueFilter(filterName);
    }

    template<typename StringType>
    FormatValueFilter operator()(const StringType &filterName, bool *pNoneReturned) const
    {
        return makeStandardFormatValueFilter(filterName, pNoneReturned);
    }

}; // struct StdFilterFactory
//#!

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
/*
    Что я хочу.

    Я хочу при помощи стирания типа хранить в "вариенте" произвольный тип,
    и печатать его.

    Я хочу хранить значения произвольного типа как по значению, так и по ссылке -
    тип может быть некопируемый и неперемещаемый, но печатать его как-то надо.

    Объект-обёртка должен хранить объект-холдер по указателю (shared_ptr/unique_ptr).

    Объект-холдер и осуществляет всю магию при помощи виртуального метода convertToString.

    Объект-холдер - это т, что хранит выводимый объект по значению или по ссылке.

    Метод convertToString объект-холдера принимает неконстантную ссылку на formattingOptions:
    virtual std::string convertToString(FormattingOptions &formattingOptions) const = 0;

    Результат, возвращаемый методом convertToString, потом передаётся в функцию форматирования строк,
    и работают правила по ширине и точности для строк.
    Метод convertToString может модифицировать ширину и точность, а также удалить/установить флаги
    их наличия, а также модифицировать прочие опции форматирования.

    Для форматируемого типа проверяется наличие подходящей свободной шаблонной функции:
    template< typename WidthCalculator, typename StringType > StringType martyFormatValueFormat(marty::format::FormattingOptions &formattingOptions, T t)

    Если такой функции нет, то проверяется наличие подходящей свободной шаблонной функции:
    template<typename StringType> inline StringType martyFormatSimpleConvertToString(T t)

    Если такой функции нет, то возвращается строка "Object is non-pritable"


#define MARTY_CPP_MAKE_ENUM_FLAGS(TEnum)                                                       \
    inline                                                                                     \
    bool enum_is_flags(TEnum)                                                                  \


std::string enum_serialize_flags( enumTypeName enumVal, const char *seps = "|+,")
std::string enum_serialize_flags( enumTypeName enumVal, const std::string &seps)                                 \
std::string enum_serialize( enumTypeName v )


*/
#if 0
struct ConvertibleToStringBase // Interface
{
    virtual ~ConvertibleToStringImplBase() {}
    virtual std::string convertToString(FormattingOptions &formattingOptions) const = 0;

}; // struct ConvertibleToStringImplBase

//----------------------------------------------------------------------------

template <typename T>
struct ConvertibleToStringImpl : ConvertibleToStringImplBase
{
    T func;
    explicit ConvertibleToStringImpl(T f) : func(std::move(f)) {}
    void operator()() const override { func(); }
    std::unique_ptr<CallableBase> clone() const override {
        return std::make_unique<CallableImpl>(*this);
    }
};
#endif


//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//#! FormatArgumentVariant
using FormatArgumentVariant = 
    std::variant< bool
                , char
                , unsigned char        // std::uint8_t
                , signed char          // std::int8_t
                , unsigned short       // std::uint16_t
                , signed short         // std::int16_t
                , unsigned int         // std::uint32_t
                , int                  // std::int32_t
                , unsigned long        // 32/64, it's depends
                , long                 // 32/64, it's depends
                , unsigned long long   // std::uint64_t
                , long long            // std::int64_t
                , float
                , double
                , long double
                , const char*
                , const wchar_t*
                , std::string
                , std::wstring
#if defined(USE_MARTY_BIGINT) && USE_MARTY_BIGINT!=0
                , marty::BigInt
#endif
#if defined(USE_MARTY_DECIMAL) && USE_MARTY_DECIMAL!=0
                , marty::Decimal
#endif
                , const void*
                // , std::nullptr_t
                , FormatValueFilter
                >;
//#!
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
#if 0
// Шаблон для проверки, является ли тип специализацией BasicFormatValueFilter
template<typename>
struct IsBasicFormatValueFilter : std::false_type {};

template<typename InputIt, typename OutputIt>
struct IsBasicFormatValueFilter
    < 
        std::function<OutputIt(InputIt, InputIt, OutputIt)>
    > : std::true_type {};

// Удобная обёртка для проверки
template<typename T>
inline constexpr bool IsBasicFormatValueFilter_v = 
    IsBasicFormatValueFilter<T>::value;

//----------------------------------------------------------------------------
// Определение параметров InputIteratorType и OutputIteratorType
template<typename>
struct BasicFormatValueFilterTraits;

template<typename InputIt, typename OutputIt>
struct BasicFormatValueFilterTraits<
    std::function<OutputIt(InputIt, InputIt, OutputIt)>
> {
    using input_iterator_type  = InputIt;
    using output_iterator_type = OutputIt;
};

//----------------------------------------------------------------------------
// Проверка, содержит ли variant хотя бы один такой тип

template<typename Variant>
struct VariantContainsBasicFormatValueFilter;

template<typename... Ts>
struct VariantContainsBasicFormatValueFilter< std::variant<Ts...> >
    : std::disjunction<IsBasicFormatValueFilter<Ts>...> {};

template<typename Variant>
inline constexpr bool VariantContainsBasicFormatValueFilter_v =
    VariantContainsBasicFormatValueFilter<Variant>::value;

//----------------------------------------------------------------------------
// Получение списка всех BasicFormatValueFilter в variant

template<typename... Ts>
struct GetBasicFormatValueFilters {
    using type = decltype(std::tuple_cat(
        std::conditional_t<
            IsBasicFormatValueFilter_v<Ts>,
            std::tuple<Ts>,
            std::tuple<>
        >{}...
    ));
};

template<typename... Ts>
using GetBasicFormatValueFilters_t = 
    typename GetBasicFormatValueFilters<Ts...>::type;

#endif
//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
#if 0
template <typename T>
struct is_basic_format_value_filter : std::false_type {};

template <typename InputIt, typename OutputIt>
struct is_basic_format_value_filter<
    std::function<OutputIt(InputIt, InputIt, OutputIt)>
> : std::true_type {
    using input_type = InputIt;
    using output_type = OutputIt;
};

template <typename T>
inline constexpr bool is_basic_format_value_filter_v = 
    is_basic_format_value_filter<T>::value;



template <typename Variant>
struct variant_filter_traits;

template <typename... Ts>
struct variant_filter_traits<std::variant<Ts...>> {
    // Проверяем, есть ли хотя бы одна специализация
    static constexpr bool has_filter = 
        (is_basic_format_value_filter_v<Ts> || ...);

    // Список параметров (InputIt, OutputIt) для всех специализаций
    using filters = std::tuple<
        typename is_basic_format_value_filter<Ts>::input_type,
        typename is_basic_format_value_filter<Ts>::output_type
    >...; // Но так нельзя, нужна фильтрация

    // Правильный способ: фильтрация и сбор параметров
    template <typename T>
    struct filter_params {
        using type = std::conditional_t<
            is_basic_format_value_filter_v<T>,
            std::tuple<
                typename is_basic_format_value_filter<T>::input_type,
                typename is_basic_format_value_filter<T>::output_type
            >,
            std::tuple<>
        >;
    };

    using filtered_params = decltype(std::tuple_cat(
        typename filter_params<Ts>::type{}...
    ));
};


template <typename Variant>
inline constexpr bool variant_has_filter_v = 
    variant_filter_traits<Variant>::has_filter;

template <typename Variant>
using variant_filter_params_t = 
    typename variant_filter_traits<Variant>::filtered_params;
#endif

#if 0
int main() {
    using MyVariant = std::variant<
        int,
        BasicFormatValueFilter<const int*, int*>,
        double,
        BasicFormatValueFilter<char*, std::string::iterator>
    >;

    // Проверка наличия фильтров
    static_assert(variant_has_filter_v<MyVariant>, "Should have filters");

    // Получение списка параметров
    using Params = variant_filter_params_t<MyVariant>;
    static_assert(
        std::tuple_size_v<Params> == 2, 
        "Should have two sets of parameters"
    );

    // Первый фильтр: const int* -> int*
    using FirstFilterParams = std::tuple_element_t<0, Params>;
    static_assert(
        std::is_same_v<std::tuple_element_t<0, FirstFilterParams>, const int*>,
        "Input type mismatch"
    );
    static_assert(
        std::is_same_v<std::tuple_element_t<1, FirstFilterParams>, int*>,
        "Output type mismatch"
    );

    return 0;
}
#endif


//----------------------------------------------------------------------------
// (с) deepseek
// Метафункция для проверки специализации
template <typename T>
struct is_basic_format_value_filter : std::false_type {};

template <typename InputIt, typename OutputIt>
struct is_basic_format_value_filter<BasicFormatValueFilter<InputIt, OutputIt>> 
    : std::true_type {
    using input_type = InputIt;
    using output_type = OutputIt;
};

template <typename T>
constexpr bool is_basic_format_value_filter_v = 
    is_basic_format_value_filter<T>::value;

// Основная метафункция для variant
template <typename Variant>
struct variant_filter_traits;

template <typename... Ts>
struct variant_filter_traits<std::variant<Ts...>> {
private:
    // Поиск первого фильтра в списке типов
    template <typename... Us>
    struct find_first_filter {
        using type = void; // Базовый случай для пустого списка
    };

    template <typename U, typename... Us>
    struct find_first_filter<U, Us...> {
        using type = std::conditional_t<
            is_basic_format_value_filter_v<U>,
            U,
            typename find_first_filter<Us...>::type
        >;
    };
    
    // Получение типов аргументов фильтра
    template <typename Filter>
    struct filter_args;
    
    template <typename InputIt, typename OutputIt>
    struct filter_args<BasicFormatValueFilter<InputIt, OutputIt>> {
        using input_type = InputIt;
        using output_type = OutputIt;
        using filter_type = BasicFormatValueFilter<InputIt, OutputIt>;
    };

public:
    // Тип первого найденного фильтра
    using first_filter = typename find_first_filter<Ts...>::type;
    
    // Типы аргументов первого фильтра
    using first_filter_args = filter_args<first_filter>;
    
    static constexpr bool has_filter = 
        !std::is_same_v<first_filter, void>;
    
    static void print_info() {
        if constexpr (has_filter) {
            std::cout << "First filter type: " 
                      << typeid(first_filter).name() << "\n";
            std::cout << "Input type:  " 
                      << typeid(typename first_filter_args::input_type).name() << "\n";
            std::cout << "Output type: " 
                      << typeid(typename first_filter_args::output_type).name() << "\n";
        } else {
            std::cout << "No filters found in variant\n";
        }
    }
};

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//#! BasicArgs
template< typename ArgumentVariantType=FormatArgumentVariant
        , typename VectorType=std::vector<ArgumentVariantType>
        , typename MapType=std::unordered_map<std::string, std::size_t>
        , typename WidthCalculator = utils::DefaultUtfWidthCalculator
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
        return m_values.empty();
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
namespace utils
{

//----------------------------------------------------------------------------
inline
const LocaleInfo* findLocaleInfo(const FormattingOptions &formattingOptions, const LocaleInfo *pUserLocaleInfo, bool useFormatStringLocale)
{
    // Для начала 
    const LocaleInfo *pLocaleInfo = getLocaleInfo(LocaleInfoType::invariant);

    if ((formattingOptions.formattingFlags&FormattingFlags::localeForceCustom)!=0)
    {
        // Вне зависимости от форматной строки используем кастомную локаль, которую нам передал пользователь
        // Если пользователь ничего не передал, то уровнем выше берётся юзер/систем локаль
        if (pUserLocaleInfo)
            return pUserLocaleInfo;
    }

    if (!useFormatStringLocale)
        return pLocaleInfo; // в форматной строке не задано использование локали, возвращаем "сишную" invariant локаль

    return pUserLocaleInfo ? pUserLocaleInfo : pLocaleInfo;
}

//----------------------------------------------------------------------------
inline
utf32_char_t getBaseFractionalSeparator(const FormattingOptions &formattingOptions)
{
    // Если есть явно заданный разделитель групп для дробной части - возвращаем его
    if (formattingOptions.fractionalGrouppingChar!=0)
        return formattingOptions.fractionalGrouppingChar;

    // Если нет явно заданного разделителя групп для целой части - возвращаем ноль
    if (formattingOptions.grouppingChar==0)
        return formattingOptions.grouppingChar;

    // Есть явно заданный разделитель групп разрядов для целой части

    // Флагами явно задано, что если задан разделитель групп разрядов для целой части,
    // то и дробную часть надо группировать
    if ((formattingOptions.formattingFlags&FormattingFlags::fractionalGroupping)!=0)
        return formattingOptions.grouppingChar;

    return 0;
}

//----------------------------------------------------------------------------
inline
std::string getFractionalSeparator(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, NumeralSystem numeralSystem, bool useLocale)
{
    std::string fracSepStr;
    
    auto fracGrpChar = getBaseFractionalSeparator(formattingOptions);
    if (fracGrpChar!=0) // Задан какой-то из разделителей - '\'', '_', ','
    {
        if (useLocale) // используем разделитель разрядов из локали
        {
            fracSepStr = pLocaleInfo->getGroupSeparator(LocaleInfoSeparatorType::fractional, numeralSystem);
        }
        else // используем явно заданный разделитель разрядов
        {
            auto outIt = marty::utf::UtfOutputIterator<char>(fracSepStr);
            *outIt++ = marty::utf::utf32_char_t(fracGrpChar);
        }
    }
    
    return fracSepStr;
}

//----------------------------------------------------------------------------
inline
std::string getThousandsSeparator(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, NumeralSystem numeralSystem, bool useLocale)
{
    MARTY_ARG_USED(formattingOptions);

    std::string grpSepStr;
    
    auto grpChar = formattingOptions.grouppingChar;
    if (grpChar!=0) // Задан какой-то из разделителей - '\'', '_', ','
    {
        if (useLocale) // используем разделитель разрядов из локали
        {
            grpSepStr = pLocaleInfo->getGroupSeparator(LocaleInfoSeparatorType::thousands, numeralSystem);
        }
        else // используем явно заданный разделитель разрядов
        {
            auto outIt = marty::utf::UtfOutputIterator<char>(grpSepStr);
            *outIt++ = marty::utf::utf32_char_t(grpChar);
        }
    }
    
    return grpSepStr;
}

//----------------------------------------------------------------------------
inline
std::string getFormatStringHelper(const LocaleInfo *pLocaleInfo, LocaleInfoValueType formatType, bool bNegative, bool bAlter)
{
    if (formatType==LocaleInfoValueType::formatCurrencyPositive)
    {
        return pLocaleInfo->getLocaleInfoValue( bAlter
                                              ? bNegative ? LocaleInfoValueType::formatCurrencyShortNegative : LocaleInfoValueType::formatCurrencyShortPositive
                                              : bNegative ? LocaleInfoValueType::formatCurrencyNegative      : LocaleInfoValueType::formatCurrencyPositive
                                              );
    }
    else if (formatType==LocaleInfoValueType::formatPercentPositive)
    {
        return pLocaleInfo->getLocaleInfoValue( bAlter
                                              ? bNegative ? LocaleInfoValueType::formatPercentShortNegative : LocaleInfoValueType::formatPercentShortPositive
                                              : bNegative ? LocaleInfoValueType::formatPercentNegative      : LocaleInfoValueType::formatPercentPositive
                                              );
    }
    else
    {
        return pLocaleInfo->getLocaleInfoValue(bNegative ? LocaleInfoValueType::formatNumberNegative : LocaleInfoValueType::formatNumberPositive);
    }
}

//----------------------------------------------------------------------------
inline
std::string getFormatString(const LocaleInfo *pLocaleInfo, LocaleInfoValueType formatType, bool bNegative, bool bAlter)
{
    auto formatString = getFormatStringHelper(pLocaleInfo, formatType, bNegative, bAlter);
    if (formatString.empty() || formatString.find('#')==formatString.npos)
    {
        formatString = bNegative ? "-#" : "+#";
    }

    return formatString;
}

//----------------------------------------------------------------------------
/*
Если точность у нас не указана, то:
1) Можно взять её из настроек локали
2) Можно оставить на усмотрение библиотеки (альтер форма)
   Но это только для чисел. Для валюты и процента всегда четкое число из локали или явно заданное

*/

//----------------------------------------------------------------------------
inline
void checkUpdateNumberPrecision(FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, LocaleInfoValueType formatType)
{
    // Если не задана точность, то берём её из локали
    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::precisionTaken)==0)
    {
        formattingOptions.optionsFlags |= FormattingOptionsFlags::precisionTaken;
        formattingOptions.precision     = width_t(pLocaleInfo->getNumberOfDigits(formatType==LocaleInfoValueType::formatCurrencyPositive ? LocaleInfoDigitsType::currency : LocaleInfoDigitsType::number));
    }
}

//----------------------------------------------------------------------------
inline
int getPrecisionForFormatFloat(const FormattingOptions &formattingOptions, const LocaleInfo *pLocaleInfo, LocaleInfoValueType formatType)
{
    // Если не задана точность, то берём её из локали
    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::precisionTaken)==0)
    {
        // У нас просто число, а не валюта и не процент
        if (formatType==LocaleInfoValueType::formatNumberPositive)
        {
            // Альт форма использует автоматическую точность
            if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signAlterForm)!=0)
            {
                return -1;
            }
        }

        // У нас валюта или процент, или обычное число без альтер признака
        // Берём точность из локали
        return (int)width_t(pLocaleInfo->getNumberOfDigits(formatType==LocaleInfoValueType::formatCurrencyPositive ? LocaleInfoDigitsType::currency : LocaleInfoDigitsType::number));
    }

    // Есть явно заданная точность, используем её
    return int(formattingOptions.precision);
}

//----------------------------------------------------------------------------
template<typename WidthCalculator>
std::string getFillCharString(const FormattingOptions &formattingOptions)
{
    auto fillChar = formattingOptions.fillChar;
    if (fillChar<32 || WidthCalculator()(fillChar)<1)
        fillChar = utf32_char_t(' ');
    return charToStringUtf8(fillChar);
}

//----------------------------------------------------------------------------
inline
PositiveNumbersMode getPositiveNumbersMode(const FormattingOptions &formattingOptions)
{
    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signPlus)!=0)
        return PositiveNumbersMode::sign;

    else if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signSpace)!=0)
        return PositiveNumbersMode::space;

    return PositiveNumbersMode::none;
}

//----------------------------------------------------------------------------
inline
char getNumbersAlignment(const FormattingOptions &formattingOptions)
{
    if (formattingOptions.alignment!=0)
        return formattingOptions.alignment;
    
    if ((formattingOptions.optionsFlags&FormattingOptionsFlags::signZero)==0)
        return '<';
    else
        return '=';
}    

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
} // namespace utils



//----------------------------------------------------------------------------

} // namespace format
} // namespace marty
// marty::format::
// #include "marty_format/marty_format.h"

