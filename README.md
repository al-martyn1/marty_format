# Библиотека форматирования marty::format



Библиотека форматирования в стиле `Python` / `C++` `std::format`.


Библиотека предназначена для использования в самодельных скриптовых языках и в языках описания 
прикладных данных (DSL - Domain Specific Language).

Как бонус, существует возможность использования функций форматирования библиотеки вместо 
`std::format` при помощи контейнера `std::initializer_list`.


Замечания, предложения, фиксы - приветствуются.

Ограничений на использование библиотеки нет. Требуется только указывать авторство.


- [Возможности библиотеки](#user-content-возможности-библиотеки)
- [Зависимости](#user-content-зависимости)
- [Ссылки на референсные спецификации](#user-content-ссылки-на-референсные-спецификации)
- [Примеры использования](#user-content-примеры-использования)
- [API библиотеки](#user-content-api-библиотеки)
  - [Обобщённый тип фильтра BasicFormatValueFilter](#user-content-обобщённый-тип-фильтра-basicformatvaluefilter)
    - [Пример реализации методов InputIteratorType для использования совместно с BasicFormatValueFilter](#user-content-пример-реализации-методов-inputiteratortype-для-использования-совместно-с-basicformatvaluefilter)
  - [Тип фильтра FormatValueFilter](#user-content-тип-фильтра-formatvaluefilter)
  - [Стандартные фильтры](#user-content-стандартные-фильтры)
  - [Фабрика стандартных фильтров - makeStandardFormatValueFilter](#user-content-фабрика-стандартных-фильтров---makestandardformatvaluefilter)
  - [marty::format::FormatArgumentVariant - Variant-тип аргумента](#user-content-martyformatformatargumentvariant---variant-тип-аргумента)
  - [marty::format::BasicArgs](#user-content-martyformatbasicargs)
    - [Конструктор marty::format::BasicArgs](#user-content-конструктор-martyformatbasicargs)
    - [Методы marty::format::BasicArgs::arg](#user-content-методы-martyformatbasicargsarg)
  - [marty::format::Args](#user-content-martyformatargs)
  - [enum флаги marty::format::FormattingFlags](#user-content-enum-флаги-martyformatformattingflags)
  - [marty::format::formatMessageImpl](#user-content-martyformatformatmessageimpl)
  - [marty::format::formatMessage - аргументы передаются в generic-контейнере](#user-content-martyformatformatmessage---аргументы-передаются-в-generic-контейнере)
  - [marty::format::formatMessage - аргументы передаются в виде std::initializer_list](#user-content-martyformatformatmessage---аргументы-передаются-в-виде-stdinitializer_list)
  - [Базовая реализация функтора вычисления отображаемой ширины строки](#user-content-базовая-реализация-функтора-вычисления-отображаемой-ширины-строки)
- [Синтаксис форматной строки](#user-content-синтаксис-форматной-строки)
  - [Грамматика полей замены](#user-content-грамматика-полей-замены)
  - [Преобразование типа аргумента - !conversion](#user-content-преобразование-типа-аргумента---conversion)
  - [Спецификатор формата - format_spec](#user-content-спецификатор-формата---format_spec)
    - [Маркер выравнивания align](#user-content-маркер-выравнивания-align)
    - [Признак знака sign](#user-content-признак-знака-sign)
  - [Поддерживаемые спецификаторы типа](#user-content-поддерживаемые-спецификаторы-типа)
    - [Спецификаторы представления строк](#user-content-спецификаторы-представления-строк)
    - [Спецификаторы представления целочисленных типов](#user-content-спецификаторы-представления-целочисленных-типов)
    - [Спецификаторы представления типов с плавающей точкой](#user-content-спецификаторы-представления-типов-с-плавающей-точкой)
  - [Экранирование строк](#user-content-экранирование-строк)
    - [](#user-content-)
    - [](#user-content--1)
- [Список задач](#user-content-список-задач)



## Возможности библиотеки

1. Добавление возможностей `C++` `std::format` из `C++20` в `C++17` (бэкпорт новых фич в старый стандарт).
2. Совместимость форматной строки с форматной строкой языков `Python` и `C++`. 
   Некоторые возможности этих языков игнорируются, но не вызывают ошибки при разборе
   форматной строки. В библиотеке совмещены возможности форматирования из языка `Python` и возможности 
   форматирования `C++20` `std::format`.
3. Поддержка именованных аргументов для `C++` при использовании соответствующих контейнеров для аргументов.
4. Поддержка передачи аргументов через std::initializer_list для непосредственного использования из `C++`.
5. Поддержка дополнительных оригинальных фич:
  - `grouping` - добавлен символ группировки `'''` (apos), который отсутстсвует в `Python`;
  - `frac_grouping` - возможность группировки цифр дробной части чисел с плавающей точкой отдельно явно
    задаваемым символом, или таким же символом группировки, как и для целой части числа, при задании
    соответствующего флага при вызове функции форматирования, отсутствует в `Python` и `C++`;
  - инверсия регистра в альтернативной форме `'!#'`.
  - фильтры для отформатированных значений. Можно использовать цепочки фильтров длиной до 8ми.
    Фильтры могут экранировать символы для вывода в `XML`/`HTML` или в `SQL`.
6. Упор на использование библиотеки в своих скриптовых языках/DSL языках.
7. Поддержка Unicode в кодировке UTF-8.
8. Поддержка чисел с плавающей точкой произвольной размерности `marty::Decimal`.
9. Поддержка пользовательских типов строк вместо `std::string` и поддержка микроконтроллеров 
   (`STM32`, как пример). Пока не реализовано, но весьма ready, требует отключения части кода 
   условными макросами, также требуется модифицировать некоторые части, пока завязанные на `std::string`.
10. Возможность создания собственной системы/библиотеки форматирования на базе данной библиотеки с поддержкой
   вывода собственных произвольных типов данных.




## Зависимости

- [marty_cpp](https://github.com/al-martyn1/marty_cpp) - от этого никуда не деться, базовые зависимости, enum как флаги и база для де/сериализации в строки.
- [marty_decimal](https://github.com/al-martyn1/marty_decimal) - поддержка чисел с плавающей точкой и произвольной размерностью. Потенциально отключаемо.
- [marty_utf](https://github.com/al-martyn1/marty_utf) - поддержка UTF-8. Потенциально отключаемо.


## Ссылки на референсные спецификации

- `Python`: Описание форматной строки - Format String Syntax - https://docs.python.org/3/library/string.html#format-string-syntax
- `C++`: `std::format` - https://en.cppreference.com/w/cpp/utility/format/format
- `C++`: `Standard format specification` - https://en.cppreference.com/w/cpp/utility/format/spec


## Примеры использования

Используем `marty::format::Args`

```cpp
// Автоматически вычисляемый индекс аргумента
// Ширину и точность (на самом деле макс ширину строки)
// задаём также аргументами, а не в форматной строке
using namespace marty::format;
cout << formatMessage("Integer number: {:d}, string: {:{}.{}s}, Pi: {:f}\n"
                     , Args().arg(10)
                             .arg("Very long string, does not fit into "
                                  "20 characters")
                             .arg(10) // задаём ширину поля
                             .arg(20) // это точность (для чисел), но для строк
                                      // это максимальная ширина поля
                             .arg(3.14159)
                     );
```


Используем `std::initializer_list<marty::format::FormatArgumentVariant>`

```cpp
// Автоматически вычисляемый индекс аргумента, используем std::initializer_list
// Перевод строки отдельно выводим
using namespace marty::format;
cout << formatMessage("Integer number: {:d}, string: {:{}.{}s}, Pi: {:f}"
                     , { 10, "Very long string, does not fit into 20 characters"
                       , 10, 20, 3.14159
                       }
                     ) << "\n";
```


Используем `std::vector<marty::format::FormatArgumentVariant>`

```cpp
// Автоматически вычисляемый индекс аргумента, используем std::vector
// Перевод строки отдельно выводим
using namespace marty::format;
auto argsVec = std::vector<FormatArgumentVariant>{ 10, "Very long string, "
                      "does not fit into 20 characters", 10, 20, 3.14159 };
cout << formatMessage( "Integer number: {:d}, string: {:{}.{}s}, Pi: {:f}"
                     , argsVec
                     ) << "\n";
```


Используем `marty::format::Args` с именоваными параметрами

```cpp
// Используем именованные аргументы,
// перемешали порядок аргументов, по сравнению с предыдущим примером
using namespace marty::format;
cout << formatMessage(
    "Integer number: {int:d}, string: {str:{strW}.{strMaxW}s}, Pi: {Pi:f}\n"
     , Args().arg("str", "Very long string, does not fit into 20 characters")
             .arg("Pi", 3.14159)
             .arg("strMaxW", 20) // это точность (для чисел), но для строк
                                 // это максимальная ширина поля
             .arg("strW", 10) // задаём ширину поля
             .arg("int", 10)
     );
```


Используем `std::vector< std::pair<std::string, marty::format::FormatArgumentVariant> >` - аналогично использованию `marty::format::Args`,
но поиск по имени каждый раз производится перебором от начала вектора. Не слишком эффективно, но работает без лишних сущностей.

```cpp
// Используем std::vector вместо marty::format::Args
// Тут поиск по имени не такой эффективный, простым перебором, но тоже работает
// Готовим вектор заранее
using namespace marty::format;
auto argsVec = std::vector< std::pair<std::string, FormatArgumentVariant> >
               { {"str", "Very long string, does not fit into 20 characters"}
               , {"Pi", 3.14159}
               , {"strMaxW", 20}
               , {"strW", 10}
               , {"int", 10}
               };
cout << formatMessage("Integer number: {int:d}, string: {str:{strW}.{strMaxW}s}, "
                      "Pi: {Pi:f}\n", argsVec);
```

Задаём конвертацию аргумента, также символ заполнения передаём аргументом:

```cpp
// Используем именованные аргументы
// Задаём конвертацию значения (но в текущий момент она игнорируется)
// Символ заполнения передаём аргументом
using namespace marty::format;
cout << formatMessage(
    "Integer number: {int:d}, string: {str!s:{fill}<{strW}.{strMaxW}s}, Pi: {Pi:f}\n"
     , Args().arg("str", "Very long string, does not fit into 20 characters")
             .arg("Pi", 3.14159)
             .arg("fill", '*')
             .arg("strMaxW", 20) // это точность (для чисел), но для строк
                                 // это максимальная ширина поля
             .arg("strW", 10) // задаём ширину поля
             .arg("int", 10)
     );
```







## API библиотеки


### Обобщённый тип фильтра BasicFormatValueFilter

Функциональный объект - фильтр, для преобразования форматированного значения в целевую строку.

Фильтрация производится после форматирования поля.

**marty_format_types.h**
```cpp
template<typename InputIteratorType, typename OutputIteratorType>
using BasicFormatValueFilter = std::function< void( InputIteratorType  // begin
                                                  , InputIteratorType  // end
                                                  , OutputIteratorType
                                                  )
                                            >;
```

Параметры шаблона:


|Значение|Описание|
|:-------|:-------|
|**InputIteratorType**|Тип входного итератора, должен иметь конструктор, принимающий два аргумента: `const CharType* pBegin` и `const CharType* pEnd`, и конструктор без параметров, создающий `end`-итератор, при вызове оператора `operator==` для него и итератора, в котором равенство указателей `pBegin` и `pEnd` обозначает достижение конца последовательности, должен возвращать `true`.|
|**OutputIteratorType**|Тип выходного итератора, добавляет символы в результирующую строку/контейнер символов.|


**Примечание**. Попытка вывода объекта фильтра как значение вызовет ошибку (исключение), или будет 
проигнорирована, в зависимости от режима игнорирования ошибок.

#### Пример реализации методов InputIteratorType для использования совместно с BasicFormatValueFilter

```cpp
bool operator==(InputIterator other) const
{
    return isIteratorEnd()==other.isIteratorEnd();
}

bool operator!=(InputIterator other) const
{
    return isIteratorEnd()!=other.isIteratorEnd();
}

InputIterator() : m_ptr(0), m_ptrEnd(0) {}

InputIterator(const char* b, const char* e)
: m_ptr(const_pointer_type(b)), m_ptrEnd(const_pointer_type(e))
{}

bool isIteratorEnd() const
{
    return m_ptr==m_ptrEnd;
}

const char* m_ptr     = 0;
const char* m_ptrEnd  = 0;
```


### Тип фильтра FormatValueFilter

Специализация базового фильтра `BasicFormatValueFilter` для UTF-8 строк: 

**marty_format_types.h**
```cpp
using FormatValueFilter = BasicFormatValueFilter< marty::utf::UtfInputIterator<char>
                                                , marty::utf::UtfOutputIterator<char>
                                                >;
```


### Стандартные фильтры

Стандартные фильтры располагаются в пространстве имён `marty::format::filters`.

Стандартные фильтры:


|Тип|Описание|
|-------|-------|
|**XmlTextFilter**|производит преобразование символов, которые недопустимы в тексте `XML`/`HTML` тэгов в стандартные сущности (`entity`) - `&apos`, `&quot`, `&lt`, `&gt`.|
|**XmlAttrFilter**|производит преобразование символов, которые недопустимы в атрибутах `XML`/`HTML` тэгов в стандартные сущности (`entity`) - `&apos`, `&quot`, `&lt`, `&gt`.|
|**SqlValueFilter**|производит преобразование символов, которые недопустимы в значениях в тексте `SQL`-запросов.|


**Примечание**. В текущий момент стандартные фильтры не реализованы (**NOT_IMPLEMENTED**).


### Фабрика стандартных фильтров - makeStandardFormatValueFilter

Возвращает стандартный фильтр по его имени.

**marty_format_types.h**
```cpp
template<typename StringType>
FormatValueFilter makeStandardFormatValueFilter(const StringType &filterName)
```

В настоящее время фабрика не реализована и выбрасывает исключение на любое переданное имя фильтра (**NOT_IMPLEMENTED**).

Допустимые имена фильтров:


|Значение|Описание|
|:-------|:-------|
|`'xml'`|возвращает экземпляр фильтра для `XML`, как для текста, так и для атрибутов.|
|`'xml_text'`|возвращает экземпляр фильтра для текста `XML`.|
|`'xml_attr'`|возвращает экземпляр фильтра для атрибутов `XML`.|
|`'html'`|возвращает экземпляр фильтра для `HTML`, как для текста, так и для атрибутов.|
|`'html_text'`|возвращает экземпляр фильтра для текста `HTML`.|
|`'html_attr'`|возвращает экземпляр фильтра для атрибутов `HTML`.|
|`'sql'`|возвращает экземпляр фильтра для значений `SQL`-запросов.|



### marty::format::FormatArgumentVariant - Variant-тип аргумента

Стандартный тип аргумента на базе `std::variant`.

Это стандартный тип `marty::format::FormatArgumentVariant`. Пользователь библиотеки может определить
свой вариант variant'а и использовать его в своих библиотеках или прикладном коде.

**marty_format_types.h**
```cpp
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
                , marty::Decimal
                , FormatValueFilter
                >;
```


### marty::format::BasicArgs

Контейнер `BasicArgs` предоставляет возможности по поиску аргумента как по целочисленному индексу, 
так и по строковому ассоциативному индексу.

Контейнер `BasicArgs` предоставляет предоставляет функции `find` и `find_by_pos`, по которым производится
выбор стратегии работы с контейнером аргументов - детектируется наличие данных методов (также проверяются
некоторые свойства контейнера аргументов).




Контейнер типа `marty::format::BasicArgs` предоставляет как метод `find` по имени, так и метод 
`find_by_pos(std::size_t)` для "поиска" по индексу.

**marty_format.h**
```cpp
template< typename ArgumentVariantType=FormatArgumentVariant
        , typename VectorType=std::vector<ArgumentVariantType>
        , typename MapType=std::unordered_map<std::string, std::size_t>
        >
class BasicArgs
```


#### Конструктор marty::format::BasicArgs

Конструктор `BasicArgs` позволяет задать игнорирование регистра именованных аргументов. 
По умолчанию регистр имён именованных аргументов игнорируется.

Игнорирование регистра именованных аргументов производится путём приведения имён
к нижнему регистру, и работает только для имён, содержащих символы из базовой таблицы ASCII.

**marty_format.h**
```cpp
BasicArgs(bool caseIgnore=true)
: m_caseIgnore(caseIgnore)
{}
```


#### Методы marty::format::BasicArgs::arg

Семейство методов `arg()` задаёт значения аргументов для форматирования
и возвращает ссылку на объект `marty::format::BasicArgs` для того, чтобы можно было делать цепочки вызовов.

Добавляет безымянный аргумент:

**marty_format.h**
```cpp
template<typename T> BasicArgs& arg(T t)
```

Добавляет именованный аргумент, имя задаётся параметром типа `const char*`:

**marty_format.h**
```cpp
template<typename T> BasicArgs& arg(const char* k, T t)
```

Добавляет именованный аргумент, имя задаётся параметром типа ключа в `map`, хранящей индексы именованных аргументов:

**marty_format.h**
```cpp
template<typename T> BasicArgs& arg(const key_type &k, T t)
```

Добавляет безымянный аргумент типа int со значением `0`:

**marty_format.h**
```cpp
BasicArgs& arg()
```


### marty::format::Args

Данный тип является специализацией типа `BasicArgs` с использованием `marty::format::FormatArgumentVariant`.

**marty_format.h**
```cpp
using Args = BasicArgs< FormatArgumentVariant
                      , std::vector<FormatArgumentVariant>
                      , std::unordered_map<std::string, std::size_t>
                      >;
```

### enum флаги marty::format::FormattingFlags

Флаги marty::format::FormattingFlags могут принимать следующие значения:


|Значение|Описание|
|:-------|:-------|
|**none**|флаги не установлены.|
|**ignoreFormatStringErrors**|игнорировать ошибки в форматной строке.|
|**ignoreArgumentErrors**|игнорировать ошибки поиска аргументов.|
|**ignoreFormattingErrors**|игнорировать ошибки при форматировании значений - например, некорректные значения ширины или точности.|
|**ignoreFillIndirectErrors**|игнорировать ошибку поиска аргумента, содержащего символ заполнения.|
|**ignoreWidthIndirectErrors**|игнорировать ошибку поиска аргумента, содержащего ширину поля.|
|**ignorePrecisionIndirectErrors**|игнорировать ошибку поиска аргумента, содержащего точность.|
|**ignoreConversionErrors**|игнорировать ошибки спецификатора типа и автоматически конвертировать данные в требуемый тип.|
|**fractionalGroupping**|Группировка цифр также и в дробной части числа, если разделитель разрядов задан для целой части числа. Без указания данного флага для дробной части числа следует явно указывать спецификатор разделителя разрядов.|
|**ignoreOptionsIndirectErrors**|игнорировать ошибки косвенного задания опций форматирования. Групповое значение для всех типов ошибок (`ignoreFillIndirectErrors \| ignoreWidthIndirectErrors \| ignorePrecisionIndirectErrors`).|
|**ignoreErrors**|игнорировать все ошибки. Групповое значение для всех типов ошибок (`ignoreFormatStringErrors \| ignoreArgumentErrors \| ignoreFormattingErrors \| ignoreOptionsIndirectErrors \| ignoreConversionErrors`).|
|**all**|все флаги установлены (`ignoreErrors \| fractionalGroupping`).|





### marty::format::formatMessageImpl

Базовая реализация функций форматирования. 
Пользователь библиотеки может создать свой собственный variant-тип аргумента, добавив
свои собственные типы, и, используя данную функцию, сделать свою кастомизированную функцию форматирования.

**marty_format.h**
```cpp
template< typename StringType      = std::string
        , typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        >
StringType formatMessageImpl( const StringType &fmt
                            , const ArgsType   &args
                            , FormattingFlags  formattingFlags=FormattingFlags::all
                            )
```



### marty::format::formatMessage - аргументы передаются в generic-контейнере

**marty_format.h**
```cpp
template< typename StringType      = std::string
        , typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        >
StringType formatMessage( const StringType &fmt
                        , const ArgsType   &args
                        , FormattingFlags  formattingFlags=FormattingFlags::all
                        )
```

**marty_format.h**
```cpp
template< typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        >
std::string formatMessage( const char *fmt
                         , const ArgsType   &args
                         , FormattingFlags  formattingFlags=FormattingFlags::all
                         )
```


### marty::format::formatMessage - аргументы передаются в виде std::initializer_list

**marty_format.h**
```cpp
using FormatArgumentVariantList = std::initializer_list<FormatArgumentVariant>;

template< typename StringType = std::string
        , typename WidthCalculator = DefaultUtfWidthCalculator
        >
StringType formatMessage( const StringType          &fmt
                        , FormatArgumentVariantList &&args
                        , FormattingFlags           formattingFlags=FormattingFlags::all
                        )
```

**marty_format.h**
```cpp
template< typename WidthCalculator = DefaultUtfWidthCalculator
        >
inline
std::string formatMessage( const char                *fmt
                         , FormatArgumentVariantList &&args
                         , FormattingFlags           formattingFlags=FormattingFlags::all
                         )
```


### Базовая реализация функтора вычисления отображаемой ширины строки

Базовая реализация функтора вычисления отображаемой ширины строки использует функции библиотеки
`marty::utf` с префиксом `suf` - `simpleUnicodeFeature`. В текущий момент поддерживается
детект пробелов нулевой ширины по кодам символов и детект комбинируемых диакретиков (по диапазонам символов).

**utils.h**
```cpp
struct DefaultUtfWidthCalculator
{
    std::size_t operator()(const char* b, const char* e) const
    {
        auto it    = marty::utf::UtfInputIterator<char>(b, e);
        auto endIt = marty::utf::UtfInputIterator<char>();

        std::size_t size = 0;

        for(; it!=endIt; ++it)
        {
            auto ch = *it;
            // suf - simpleUnicodeFeature
            if (marty::utf::sufIsZeroWidthSpace(ch) || marty::utf::sufIsCombiningDiacretic(ch))
                continue;

            ++size;
        }

        return size;
    }

}; // struct DefaultUtfWidthCalculator
```

**marty_format.h**
```cpp
using DefaultUtfWidthCalculator = utils::DefaultUtfWidthCalculator;
```














## Синтаксис форматной строки



Форматная строка содержит "поля замены", окруженные фигурными скобками (`{}`).
Все, что не заключено в фигурные скобки, считается текстом, который копируется в вывод без изменений.
Если вам нужно включить символ скобки в текст, его можно экранировать удвоением: `{{` и `}}`.

**Примечание**. В зависимости от режима игнорирования ошибок, закрывающая фигурная скобка может быть
единственной.


### Грамматика полей замены

Грамматика для полей замены выглядит так:

<pre>
replacement_field ::= &quot;{&quot; [arg_id] [&quot;!&quot; conversion] [&quot;:&quot; format_spec] &quot;}&quot;
arg_id            ::= arg_index | arg_name
arg_index         ::= digit+
arg_name          ::= identifier
identifier        ::= identifier_char [identifier_char | digit &quot;.&quot;]+
identifier_char   ::= &quot;_&quot; | &quot;a&quot;-&quot;z&quot; | &quot;A&quot;-&quot;Z&quot;
conversion        ::= &quot;r&quot; | &quot;s&quot; | &quot;a&quot;
format_spec       ::= format_spec_expr
</pre>

<!-- -->

Описание полей грамматики полей замены:


**arg_id** - 
Идентификатор аргумента. Может отсутствовать, тогда используется автоматически
подставляемый индекс, который инкрементируется при появлении каждого "поля замены" с отсутствующим
идентификатором аргумента.

**arg_index** - 
Индекс аргумента, начиная с нуля. Для ассоциативных контейнеров
типа `std::map` и `std::unordered_map` обращение по индексу производится с использованием
`std::advance`. Индекс аргумента должен быть корректным десятичным числом.

**arg_name** - 
Имя аргумента, допустимы любые символы, кроме символов `:`, `{` и `}`.
В зависимости от типа контейнера, в котором передаются аргументы, именованное обращение к аргументам
может не поддерживаться. Так, для контейнеров типа `std::vector` или `std::initializer_list`
использование именованного аргумента приведёт к выбросу исключения, если это не запрещено режимом
игнорирования ошибок.

**identifier** - 
Идентификатор - начинается с `identifier_char`, последующие символы могут быть `identifier_char`, цифры, или символ `'.'`.

**identifier_char** - 
символ идентификатора - символ подчеркивания `'_'`, строчная или прописная латинская буква.

**conversion** - 
преобразование типа аргумента, см. далее "Преобразование типа аргумента".

**format_spec** - 
спецификатор формата, см. далее.



**Примечание**. При использовании ассоциативных контейнеров типа `std::map` и `std::unordered_map`
не существует возможности задать порядок следования аргументов, порядок следования аргументов определяется
контейнером. При использовании контейнеров типа `std::vector` или `std::initializer_list` нет возможности
использовать именованные аргументы. Для возможности использования одновременно обращений к аргументам 
по целочисленному индексу и по имени аргумента следует использовать конейнер `std::vector` с элементами
типа `std::pair<std::string, T>` (вместо `std::pair` можно использовать любую другую структуру, которая имеет
вложенные типы `first_type` и `second_type`, и, соответственно, поля `first` и `second`), либо
`marty::format::BaseArgs<...>`, специфицированный пользовательскими типами или явно специфицированный `marty::format::Args`.


### Преобразование типа аргумента - !conversion

В данный момент обрабатывается при разборе форматной строки, но игнорируется при дальнейшей обработке.
Надо разобраться, что делают режимы преобразования в оригинальной реализации форматирования в `Python`,
и продумать, что они будут делать в данной библиотеке.


|Значение|Описание|
|:-------|:-------|
|`s`|преобразует аргумент в строку (см. описание [str](https://docs.python.org/3/library/stdtypes.html#str)).|
|`r`|преобразует аргумент в строку (см. описание [repr](https://docs.python.org/3/library/functions.html#repr)).|
|`a`|преобразует аргумент в строку (см. описание [ascii](https://docs.python.org/3/library/functions.html#ascii)).|



### Спецификатор формата - format_spec


<pre>
format_spec     ::= [options][width][grouping][&quot;.&quot; precision][frac_grouping][&quot;L&quot;][type][filters]
options         ::= [[fill]align][sign][&quot;z&quot;][&quot;!&quot;][&quot;#&quot;][&quot;0&quot;]
fill            ::= &lt;any character&gt; | &quot;{&quot; argId &quot;}&quot;
align           ::= &quot;&lt;&quot; | &quot;&gt;&quot; | &quot;=&quot; | &quot;^&quot;
sign            ::= &quot;+&quot; | &quot;-&quot; | &quot; &quot;
width           ::= digit+ | &quot;{&quot; argId &quot;}&quot;
grouping        ::= &quot;,&quot; | &quot;_&quot; | &quot;&apos;&quot;
frac_grouping   ::= &quot;,&quot; | &quot;_&quot; | &quot;&apos;&quot;
precision       ::= digit+ | &quot;{&quot; argId &quot;}&quot;
type            ::= &quot;b&quot; | &quot;c&quot; | &quot;d&quot; | &quot;e&quot; | &quot;E&quot; | &quot;f&quot; | &quot;F&quot; | &quot;g&quot; | &quot;G&quot; | &quot;n&quot; | &quot;o&quot; | &quot;s&quot; | &quot;x&quot; | &quot;X&quot; | &quot;%&quot;
filters         ::= filter+
filter          ::= &quot;|&quot; argId
arg_id          ::= arg_index | arg_name
arg_index       ::= digit+
arg_name        ::= identifier
identifier      ::= identifier_char [identifier_char | digit &quot;.&quot;]+
identifier_char ::= &quot;_&quot; | &quot;a&quot;-&quot;z&quot; | &quot;A&quot;-&quot;Z&quot;
</pre>

<!-- -->

Описание полей спецификатора формата:


|Значение|Описание|
|:-------|:-------|
|**fill**, **align**|заполнение и выравнивание. Символ заполнения может быть любым символом, за исключением символов, используемых в форматной строке. Символ заполнения может быть не задан, тогда будет использоваться символ пробела `space` ([U+0020](https://unicode-explorer.com/c/0020)).|
|**sign**|знак. Данный параметр действителен только для числовых типов, для остальных типов игнорируется.|
|**z**|обрабатывается в форматной строке, но игнорируется (**IGNORED**).|
|**!**|Инверсия регистра. Если задаётся альтернативная форма опцией `'#'`, то к двоичным и шестнадцатиричным числам со спецификаторами типа формата `'b'`, `'B'`, `'x'` и `'X'` добавляется префикс `'0b'`, `'0B'`, `'0x'` или `'0X'`. Так, шестнадцатеричные числа будут отображаться как `'0xdeadbeaf'` или `'0XDEADBEAF'` (для спецификаторов типа `'x'` и `'X'`, соответственно). Такая запись читается плохо - префикс сливается с остальным числом. Для того, чтобы префикс был хорошо заметен, его регистр обычно делается отличным от регистра самого числа - `'0xDEADBEAF'`. Маркер инверсии регистра предназначен для изменения регистра префикса по сравнению с регистром числа. Так, форматная строка `'!#X'` выведет `'0xDEADBEAF'`, а `'!#x'` - `'0Xdeadbeaf'`.<br/><br/> Для систем счисления, отличных от двоичной и шестнадцатеричной, а также для спецификаторов типа формата, отличных от `'b'`, `'B'`, `'x'` и `'X'`, данная опция игнорируется.|
|**#**|Задаёт использовать альтернативную форму при форматировании. Альтернативная форма определяется по-разному для разных типов. Этот параметр действителен только для целых чисел и чисел с плавающий точкой. Для целых чисел, когда используется двоичный, восьмеричный или шестнадцатеричный вывод, этот параметр добавляет соответствующий префикс '0b', '0o', '0x' или '0X' к выходному значению. Для десятичных чисел параметр игнорируется. Для чисел с плавающий точкой альтернативная форма заставляет результат преобразования всегда содержать символ десятичной точки, даже если за ним не следует ни одна цифра. Обычно символ десятичной точки появляется в результате этих преобразований, только если за ним следует цифра. Кроме того, для преобразований 'g' и 'G' конечные нули не удаляются из результата.|
|**0**|Если явное выравнивание не указано, то предваряющий поле ширины нулевой символ включает нулевое заполнение для числовых типов. Это эквивалентно символу заполнения `'0'` с типом выравнивания `'='`.|
|**width**|Ширина — это десятичное целое число, определяющее минимальную общую ширину поля, включая любые префиксы, разделители и другие символы форматирования. Если не указано, то ширина поля будет определяться содержимым.|
|**grouping**|Параметр группировки после поля ширины указывает разделитель групп цифр для целой части числа. Для десятичных чисел разделение разрядов производится по три цифры. Для спецификаторов `'b'`, `'o'`, `'x'`, and `'X'` разделение производится на группы по четыре цифры. Разделитель групп разрядов может быть одним из следующих: `','`, `'_'`, `'''` (apos).<br/><br/> Данный параметр не используется для форматирования в библиотеке языка `C++`.<br/><br/> В языке `Python` символ `'''` (apos) не используется. Также, в языке `Python` разделители `','` и `'_'` используются при отображении десятичных чисел и отображении `'b'`, `'o'`, `'x'`, and `'X'` чисел соответственно. Данная библиотека позволяет использовать все допустимые разделители для любых чисел.|
|**precision**|Точность - десятичное целое число, указывающее, сколько цифр должно отображаться после десятичной точки для типов представления 'f' и 'F', или до и после десятичной точки для типов представления 'g' или 'G'. Для строковых типов представления поле указывает максимальный размер поля — другими словами, сколько символов будет использовано из содержимого поля. Точность не допускается для целочисленных типов представления.|
|**frac_grouping**|Группировка цифр числа в дробной части (fractional part groupping). По умолчанию, если при вызове функции форматирования не задан флаг форматирования `marty::format::FormattingFlags::fractionalGroupping`, группировка цифр в дробной части чисел с плавающей точкой не производится.|
|**L**|Locale-зависимое форматирование. Аналогично спецификатору типа `'n'`. В текущий момент игнорируется (**IGNORED**).|
|**type**|спецификатор типа. Определяет, как должны быть отображены данные.|
|**filters**|последовательность фильтров, до восьми в цепочке.|
|**filter**|идентификатор аргумента, предварённый символом пайпа `'\|'` (broken vertical bar)|
|**arg_id**, **arg_index**, **arg_name**, **identifier**, **identifier_char**|См. предыдущий раздел "Грамматика полей замены"|


**Примечание**. Поддержка фильтров в форматной строке в данный момент не реализавана (**NOT_IMPLEMENTED**). Попытка 
использовать фильтры вызовет ошибку на этапе разбора форматной строки.


#### Маркер выравнивания align


|Значение|Описание|
|:-------|:-------|
|`'<'`|Задаёт выравнивание по левому краю поля вывода.|
|`'>'`|Задаёт выравнивание по правому краю поля вывода. Используется по умолчанию.|
|`'='`|Задаёт вставлять заполнение после знака числа, но перед цифрами. Позволяет печатать числа в виде  `'+  120'`. Работает только для числовых типов. Если значению ширины поля (`width`) предшествует символ `'0'`, то данный тип выравнивания становится значением по умолчанию и вывод примет вид `'+00120'`.|
|`'^'`|Задаёт выравнивание по центру|


Обратите внимание, что если не определена минимальная ширина поля (`width`), ширина поля всегда будет 
такой же, как и размер заполняющих его данных, поэтому параметр выравнивания в этом случае не имеет смысла.


#### Признак знака sign


|Значение|Описание|
|:-------|:-------|
|`'+'`|Указывает, что знак следует использовать как для положительных, так и для отрицательных чисел.\||
|`'-'`|Указывает, что знак следует использовать только для отрицательных чисел (это поведение используется по умолчанию).|
|`space`|Указывает, что для положительных чисел следует использовать пробел, а для отрицательных чисел — знак `'-'`.|


**Примечание**. Поддержка `space` в форматной строке реализована, но игнорируется при форматированиии (**IGNORED**).



Тут надо написать про конвертацию - есть, нет, что-то ещё.


### Поддерживаемые спецификаторы типа

Символами `Py` обозначаются спецификаторы типа `Python`, `C++` - `C++`, соответственно, спецификаторы, которые 
поддерживаются обоими языками, будут обозначены как `Py`/`C++`.

Тестируем таблицу без заголовка



#### Спецификаторы представления строк



|Спецификатор|Язык|Описание|
|:-------|:-------|:-------|
|`none`|`Py`/`C++`|Аналогично `'s'`.|
|`'s'`|`Py`/`C++`|Выводит строку.|
|`'?'`|`C++`|Выводит экранированную строку, см. далее "Экранирование строк".|



#### Спецификаторы представления целочисленных типов


|Спецификатор|Язык|Описание|
|:-------|:-------|:-------|
|`none`|`Py`/`C++`|Аналогично `'d'`.|
|`'b'`|`Py`/`C++`|Форматирует число в двоичном виде. Префикс основания системы счисления - `'0b'` и `'0B'` (с инверсией регистра).|
|`'B'`|`C++`|Форматирует число в двоичном виде. Префикс основания системы счисления - `'0B'` и `'0b'` (с инверсией регистра).|
|`'c'`|`Py`/`C++`|Символ. Отображает целое число в виде символа.|
|`'d'`|`Py`/`C++`|Десятичное число (в обычной нотации).|
|`'o'`|`Py`/`C++`|Форматирует число в восьмеричном виде. Префикс основания системы счисления - `'0'`.|
|`'x'`|`Py`/`C++`|Форматирует число в шестнадцатеричном виде. Префикс основания системы счисления - `'0x'` и `'0X'` (с инверсией регистра).|
|`'X'`|`Py`/`C++`|Форматирует число в шестнадцатеричном виде. Префикс основания системы счисления - `'0X'` и `'0x'` (с инверсией регистра).|
|`'n'`|`Py`|Десятичное число. Аналогично форматному символу `'d'` с `C++` опцией `'L'`. Форматирует число с использованием символа разделителя разрядов, заданного системной локалью. В данный момент используется фиксированный символ `'''` (apos).|


#### Спецификаторы представления типов с плавающей точкой


|Спецификатор|Язык|Описание|
|:-------|:-------|:-------|
|`'a'`|`Py`/`C++`||
|`'A'`|`Py`/`C++`||
|`'e'`|`Py`/`C++`||
|`'E'`|`Py`/`C++`||
|`'f'`|`Py`/`C++`||
|`'F'`|`Py`/`C++`||
|`'g'`|`Py`/`C++`||
|`'G'`|`Py`/`C++`||



**Примечание**. Десятичные числа с произвольной размерностью `marty::Decimal`
в режиме форматирования обычного числа с плавающей точкой (`'f'`/`'F'`) форматируются как есть,
в других режимах форматирование производится стандартными средствами `C++` через преобразование
в `long double`.


### Экранирование строк

См. https://en.cppreference.com/w/cpp/utility/format/spec,
раздел "Formatting escaped characters and strings".


#### 


#### 









## Список задач

Список задач по библиотеке - [TODO](todo.md_).


Тестим ссылки - [Возможности библиотеки](#возможности-библиотеки),
[Возможности библиотеки](#Возможности библиотеки)
