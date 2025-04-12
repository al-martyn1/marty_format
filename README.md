# Библиотека форматирования marty::format



Библиотека форматирования в стиле `Python` / `C++` `std::format`.

Библиотека не претендует на высокое качество кода, также не претендует на высокую производительность.

Библиотека предназначена для использования в самодельных скриптовых языках и в языках описания 
прикладных данных (DSL - Domain Specific Language).

Как бонус, существует возможность использования функций форматирования библиотеки вместо 
`std::format` при помощи контейнера `std::initializer_list`.

Не бейте ~~пианиста~~программиста, он ~~играет~~пишет, как умеет.

Замечания, предложения, фиксы - приветствуются.

Ограничений на использование библиотеки нет. Требуется только указывать авторство.


- [Возможности библиотеки](#user-content-возможности-библиотеки)
- [Зависимости](#user-content-зависимости)
- [Ссылки на референсные спецификации](#user-content-ссылки-на-референсные-спецификации)
- [Примеры использования](#user-content-примеры-использования)
- [API библиотеки](#user-content-api-библиотеки)
  - [marty::format::FormatArgumentVariant - Variant-тип аргумента](#user-content-martyformatformatargumentvariant---variant-тип-аргумента)
  - [marty::format::BasicArgs](#user-content-martyformatbasicargs)
    - [Конструктор marty::format::BasicArgs](#user-content-конструктор-martyformatbasicargs)
    - [Методы marty::format::BasicArgs::arg](#user-content-методы-martyformatbasicargsarg)
  - [marty::format::Args](#user-content-martyformatargs)
  - [enum флаги marty::format::FormattingFlags](#user-content-enum-флаги-martyformatformattingflags)
  - [marty::format::formatMessageImpl](#user-content-martyformatformatmessageimpl)
  - [marty::format::formatMessage - аргументы передаются в generic-контейнере](#user-content-martyformatformatmessage---аргументы-передаются-в-generic-контейнере)
  - [marty::format::formatMessage - аргументы передаются в виде std::initializer_list](#user-content-martyformatformatmessage---аргументы-передаются-в-виде-stdinitializer_list)
- [Синтаксис форматной строки](#user-content-синтаксис-форматной-строки)
  - [Преобразование типа аргумента](#user-content-преобразование-типа-аргумента)
  - [Спецификатор формата](#user-content-спецификатор-формата)
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
5. Упор на использование библиотеки в своих скриптовых языках/DSL языках.
6. Поддержка Unicode в кодировке UTF-8.
7. Поддержка чисел с плавающей точкой произвольной размерности `marty::Decimal`.
8. Поддержка пользовательских типов строк вместо `std::string` и поддержка микроконтроллеров 
   (`STM32`, как пример). Пока не реализовано, но весьма ready, требует отключения части кода 
   условными макросами, также требуется модифицировать некоторые части, пока завязанные на `std::string`.
9. Возможность создания собственной системы/библиотеки форматирования на базе данной библиотеки с поддержкой
   вывода собственных произвольных типов данных.



## Зависимости

- [marty_decimal](https://github.com/al-martyn1/marty_decimal)
- [marty_utf](https://github.com/al-martyn1/marty_utf)


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
// перемешали порядок, по сравнению с предыдущим примером
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
// Задаём конвертацию (игнорируем)
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

### marty::format::FormatArgumentVariant - Variant-тип аргумента

Стандартный тип аргумента на базе `std::variant`.

Это стандартный тип `marty::format::FormatArgumentVariant`. Пользователь библиотеки может определить
свой вариант variant'а и использовать его в своих библиотеках или прикладном коде.

```cpp
using FormatArgumentVariant =
    std::variant< bool
                , char
                , std::int8_t
                , std::uint8_t
                , std::int16_t
                , std::uint16_t
                , std::int32_t
                , std::uint32_t
                , std::int64_t
                , std::uint64_t
                , float
                , double
                , long double
                , const char*
                , const wchar_t*
                , std::string
                , std::wstring
                , marty::Decimal
                >;
```


### marty::format::BasicArgs

Контейнер `BasicArgs` предоставляет возможности по поиску аргумента как по целочисленному индексу, 
так и по строковому ассоциативному индексу.

Контейнер `BasicArgs` предоставляет предоставляет функции `find` и `find_by_pos`, по которым производится
выбор стратегии работы с контейнером аргументов - детектируется наличие данных методов (также проверяются
некоторые свойства контейнера аргументов).

**Ремарка**. Я не настоящий `C++` программист, и я не осилил, как отличить наличие метода `find(std::string)`
для поиска по строковому значению от
наличия метода `find(std::size_t)`
для поиска по индексу.
При этом разные компиляторы на мои проверки срабатывали по-разному.
Поэтому для определения факта использования своего контейнера я решил завести отдельный новый метод
`find_by_pos`.

К слову, код вообще так себе.



Контейнер типа `marty::format::BasicArgs` предоставляет как метод `find` по имени, так и метод 
`find_by_pos(std::size_t)` для "поиска" по индексу.

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

```cpp
BasicArgs(bool caseIgnore=true)
: m_caseIgnore(caseIgnore)
{}
```


#### Методы marty::format::BasicArgs::arg

Семейство методов `arg()` задаёт значения аргументов для форматирования
и возвращает ссылку на объект `marty::format::BasicArgs` для того, чтобы можно было делать цепочки вызовов.

Добавляет безымянный аргумент:

```cpp
template<typename T> BasicArgs& arg(T t)
```

Добавляет именованный аргумент, имя задаётся параметром типа `const char*`:

```cpp
template<typename T> BasicArgs& arg(const char* k, T t)
```

Добавляет именованный аргумент, имя задаётся параметром типа ключа в `map`, хранящей индексы именованных аргументов:

```cpp
template<typename T> BasicArgs& arg(const key_type &k, T t)
```

Добавляет безымянный аргумент типа int со значением `0`:

```cpp
BasicArgs& arg()
```


### marty::format::Args

Данный тип является специализацией типа `BasicArgs` с использованием `marty::format::FormatArgumentVariant`.

```cpp
using Args = BasicArgs< FormatArgumentVariant
                      , std::vector<FormatArgumentVariant>
                      , std::unordered_map<std::string, std::size_t>
                      >;
```

### enum флаги marty::format::FormattingFlags

**none** - флаги не установлены.

**ignoreFormatStringErrors** - игнорировать ршибки в форматной строке.

**ignoreArgumentErrors** - игнорировать ошибки поиска аргументов.

**ignoreFormattingErrors** - игнорировать ошибки при форматировании значений - например, некорректные значения ширины или точности.

**ignoreFillIndirectErrors** - игнорировать ошибку поиска аргумента, содержащего символ заполнения.

**ignoreWidthIndirectErrors** - игнорировать ошибку поиска аргумента, содержащего ширину поля.

**ignorePrecisionIndirectErrors** - игнорировать ошибку поиска аргумента, содержащего точность.

**ignoreConversionErrors** - игнорировать ошибки спецификатора типа и автоматически конвертировать данные в требуемый тип.


**ignoreOptionsIndirectErrors** - игнорировать ошибки косвенного задания опций форматирования (`ignoreFillIndirectErrors | ignoreWidthIndirectErrors | ignorePrecisionIndirectErrors`).

**ignoreErrors**                - игнорировать все ошибки (`ignoreFormatStringErrors | ignoreArgumentErrors | ignoreFormattingErrors | ignoreOptionsIndirectErrors | ignoreConversionErrors`)

**all** - всё флаги установлены.



### marty::format::formatMessageImpl

Базовая реализация функций форматирования. 
Пользователь библиотеки может создать свой собственный variant-тип аргумента, добавив
свои собственные типы, и, используя данную функцию, сделать свою кастомизированную функцию форматирования.

```cpp
template< typename StringType = std::string
        , typename ArgsType   = Args
        >
StringType formatMessageImpl( const StringType &fmt
                            , const ArgsType   &args
                            , FormattingFlags  formattingFlags=FormattingFlags::all
                            )
```


### marty::format::formatMessage - аргументы передаются в generic-контейнере

```cpp
template< typename StringType = std::string
        , typename ArgsType   = Args
        >
StringType formatMessage( const StringType &fmt
                        , const ArgsType   &args
                        , FormattingFlags  formattingFlags=FormattingFlags::all
                        )
```

```cpp
template< typename ArgsType = Args >
std::string formatMessage( const char *fmt
                         , const ArgsType   &args
                         , FormattingFlags  formattingFlags=FormattingFlags::all
                         )
```


### marty::format::formatMessage - аргументы передаются в виде std::initializer_list

```cpp
using FormatArgumentVariantList = std::initializer_list<FormatArgumentVariant>;

template< typename StringType = std::string >
StringType formatMessage( const StringType          &fmt
                        , FormatArgumentVariantList &&args
                        , FormattingFlags           formattingFlags=FormattingFlags::all
                        )
```

```cpp
inline
std::string formatMessage( const char                *fmt
                         , FormatArgumentVariantList &&args
                         , FormattingFlags           formattingFlags=FormattingFlags::all
                         )
```
















## Синтаксис форматной строки



Форматная строка содержит "поля замены", окруженные фигурными скобками (`{}`).
Все, что не заключено в фигурные скобки, считается текстом, который копируется в вывод без изменений.
Если вам нужно включить символ скобки в текст, его можно экранировать удвоением: `{{` и `}}`.

**Примечание**. В зависимости от режима игнорирования ошибок, закрывающая фигурная скобка может быть
единственной.


Грамматика для полей замены выглядит так:

<pre>
replacement_field ::= &quot;{&quot; [arg_id] [&quot;!&quot; conversion] [&quot;:&quot; format_spec] &quot;}&quot;
arg_id            ::= arg_index | arg_name
arg_index         ::= digit+
arg_name          ::= identifier
conversion        ::= &quot;r&quot; | &quot;s&quot; | &quot;a&quot;
format_spec       ::= format_spec_expr
</pre>

<!-- -->

**arg_id** - идентификатор аргумента. Может отсутствовать, тогда используется автоматически 
подставляемый индекс, который инкрементируется при появлении каждого "поля замены" с отсутствующим 
идентификатором аргумента.

**arg_index** - индекс аргумента, начиная с нуля. Для ассоциативных контейнеров 
типа `std::map` и `std::unordered_map` обращение по индексу производится с использованием 
`std::advance`. Индекс аргумента должен быть корректным десятичным числом.

**arg_name** - имя аргумента, допустимы любые символы, кроме символов `:`, `{` и `}`.
В зависимости от типа контейнера, в котором передаются аргументы, именованное обращение к аргументам
может не поддерживаться. Так, для контейнеров типа `std::vector` или `std::initializer_list`
использование именованного аргумента приведёт к выбросу исключения, если это не запрещено режимом 
игнорирования ошибок.

**conversion** - преобразование типа аргумента, см. далее "Преобразование типа аргумента".

**format_spec** - спецификатор формата, см. далее.

**Примечание**. При использовании ассоциативных контейнеров типа `std::map` и `std::unordered_map`
не существует возможности задать порядок следования аргументов, порядок следования аргументов определяется
контейнером. При использовании контейнеров типа `std::vector` или `std::initializer_list` нет возможности
использовать именованные аргументы. Для возможности использования одновременно обращений к аргументам 
по целочисленному индексу и по имени аргумента следует использовать конейнер `std::vector` с элементами
типа `std::pair<std::string, T>` (вместо `std::pair` можно использовать любую другую структуру, которая имеет
вложенные типы `first_type` и `second_type`, и, соответственно, поля `first` и `second`), либо
`marty::format::BaseArgs<...>`, специфицированный пользовательскими типами или явно специфицированный `marty::format::Args`.


### Преобразование типа аргумента

В данный момент не реализовано (**NOT IMPLEMENTED**), планируется к реализации. Использование в поле замены вызовет ошибку.
Надо разобраться, что делают режимы преобразования в оригинальной реализации форматирования в `Python`,
и продумать, что они будут делать в данной библиотеке.

`s` - преобразует аргумент в строку (см. описание [str](https://docs.python.org/3/library/stdtypes.html#str)).

`r` - преобразует аргумент в строку (см. описание [repr](https://docs.python.org/3/library/functions.html#repr)).

`a` - преобразует аргумент в строку (см. описание [ascii](https://docs.python.org/3/library/functions.html#ascii)).


### Спецификатор формата


<pre>
format_spec ::= [options][width][grouping][&quot;.&quot; precision][&quot;L&quot;][type]
options     ::= [[fill]align][sign][&quot;z&quot;][&quot;#&quot;][&quot;0&quot;]
fill        ::= &lt;any character&gt;
align       ::= &quot;&lt;&quot; | &quot;&gt;&quot; | &quot;=&quot; | &quot;^&quot;
sign        ::= &quot;+&quot; | &quot;-&quot; | &quot; &quot;
width       ::= digit+
grouping    ::= &quot;,&quot; | &quot;_&quot; | &quot;&apos;&quot;
precision   ::= digit+
type        ::= &quot;b&quot; | &quot;c&quot; | &quot;d&quot; | &quot;e&quot; | &quot;E&quot; | &quot;f&quot; | &quot;F&quot; | &quot;g&quot; | &quot;G&quot; | &quot;n&quot; | &quot;o&quot; | &quot;s&quot; | &quot;x&quot; | &quot;X&quot; | &quot;%&quot;
</pre>

<!-- -->

**fill**, **align** - заполнение и выравнивание. Символ заполнения может быть любым символом,
за исключением символов, используемых в форматной строке. Символ заполнения может быть не задан,
тогда будет использоваться символ пробела `space` ([U+0020](https://unicode-explorer.com/c/0020)).

|Символ|Значение|
|:-----|:-------|
|`'<'`|Задаёт выравнивание по левому краю поля вывода.|
|`'>'`|Задаёт выравнивание по правому краю поля вывода. Используется по умолчанию.|
|`'='`|Задаёт вставлять заполнение после знака числа, но перед цифрами. Позволяет печатать числа в виде  `'+  120'`. Работает только для числовых типов. Если значению ширины поля (`width`) предшествует символ `'0'`, то данный тип выравнивания становится значением по умолчанию и вывод примет вид `'+00120'`.|
|`'^'`|Задаёт выравнивание по центру.|

Обратите внимание, что если не определена минимальная ширина поля (`width`), ширина поля всегда будет 
такой же, как и размер заполняющих его данных, поэтому параметр выравнивания в этом случае не имеет смысла.

**sign** - знак. Данный параметр действителен только для числовых типов, и может быть следующим:

|Символ|Значение|
|:-----|:-------|
|`'+'`|Указывает, что знак следует использовать как для положительных, так и для отрицательных чисел.|
|`'-'`|Указывает, что знак следует использовать только для отрицательных чисел (это поведение используется по умолчанию).|
|`space`|Указывает, что для положительных чисел следует использовать пробел, а для отрицательных чисел — знак `'-'`.|

**Примечание**. Поддержка `space` в форматной строке пока не реализована (**NOT IMPLEMENTED**).

**z** - (**NOT IMPLEMENTED**)

**#** - Задаёт использовать альтернативную форму при форматировании. 
Альтернативная форма определяется по-разному для разных типов.
Этот параметр действителен только для целых чисел и чисел с плавающий точкой.
Для целых чисел, когда используется двоичный, восьмеричный или шестнадцатеричный вывод, 
этот параметр добавляет соответствующий префикс '0b', '0o', '0x' или '0X' к выходному значению.
Для десятичных чисел параметр игнорируется.
Для чисел с плавающий точкой альтернативная форма заставляет результат преобразования 
всегда содержать символ десятичной точки, даже если за ним не следует ни одна цифра. 
Обычно символ десятичной точки появляется в результате этих преобразований, 
только если за ним следует цифра. Кроме того, для преобразований 'g' и 'G' конечные нули не удаляются из результата.

**0** - Если явное выравнивание не указано, то предваряющий поле ширины нулевой символ
включает нулевое заполнение для числовых типов. Это эквивалентно символу заполнения `'0'` с типом выравнивания `'='`.


**width** - ширина — это десятичное целое число, определяющее минимальную общую ширину поля, 
включая любые префиксы, разделители и другие символы форматирования. Если не указано, 
то ширина поля будет определяться содержимым.

**grouping** - параметр группировки после поля ширины указывает разделитель групп цифр для целой части числа.
Для десятичных чисел разделение разрядов производится по три цифры. Для спецификаторов `'b'`, `'o'`, `'x'`, and `'X'`
разделение производится на группы по четыре цифры.
Разделитель групп разрядов может быть одним из следующих: `','`, `'_'`, `'''` (apos).

Данный параметр не используется для форматирования в библиотеке языка `C++`.

В языке `Python` символ `'''` (apos) не используется. Также, в языке `Python` разделители `','` и `'_'`
используются при отображении десятичных чисел и отображении `'b'`, `'o'`, `'x'`, and `'X'` чисел соответственно.
Данная библиотека позволяет использовать все допустимые разделители для любых чисел.

**precision** - точность - десятичное целое число, указывающее, сколько цифр должно отображаться 
после десятичной точки для типов представления 'f' и 'F', или до и после десятичной точки для 
типов представления 'g' или 'G'. Для строковых типов представления поле указывает максимальный 
размер поля — другими словами, сколько символов будет использовано из содержимого поля. 
Точность не допускается для целочисленных типов представления.

**L** - locale-зависимое форматирование. Аналогично спецификатору типа `'n'`. В текущий момент игнорируется.


**type** - спецификатор типа. Определяет, как должны быть отображены данные.

Тут надо написать про конвертацию - есть, нет, что-то ещё.


### Поддерживаемые спецификаторы типа

Символом `P` обозначаются спецификаторы типа `Python`, `C` - `C++`, соответственно, спецификаторы, которые 
поддерживаются обоими языками, будут обозначены как `P`/`C`.

Тестируем таблицу без заголовка



#### Спецификаторы представления строк

Тестируем виды представления списков аргументов


|Спецификатор|Язык|Описание|
|:-------:|-------:|:-------|
|`none`|`P`/`C`|Аналогично `'s'`.|
|`'s'`|`P`/`C`|Выводит строку|
|`'?'`|`C`|Выводит экранированную строку.<br/><br/> См. далее "Экранирование строк"|
|**some**|||
|**other**|a|b c d e<br/><br/> f|




**`none`** - `P`/`C` - 
Аналогично `'s'`.

**`'s'`** - `P`/`C` - 
Выводит строку

**`'?'`** - `C` - 
Выводит экранированную строку.

См. далее "Экранирование строк"

**some** -  - 


**other** - a - 
b
c
d
e

f





|Спецификатор|Язык|Описание|
|:-----------|:---|:-------|
|`none`|`P`/`C`|Аналогично `'s'`.|
|`'s'` |`P`/`C`|Выводит строку|
|`'?'` |    `C`|Выводит экранированную строку, см. далее "Экранирование строк"|



|Спецификатор|Язык|Описание|
|:-----------|:---|:-------|
|`none`|`P`/`C`|Аналогично `'s'`.|
|`'s'` |`P`/`C`|Выводит строку|
|`'?'` |    `C`|Выводит экранированную строку, см. далее "Экранирование строк"|


#### Спецификаторы представления целочисленных типов

|Спецификатор|Язык|Описание|
|:-----------|:---|:-------|
|`none`|`P`/`C`|Аналогично `'d'`.|
|`'b'`|`P`/`C`||
|`'B'`|    `C`||
|`'c'`|`P`/`C`||
|`'d'`|`P`/`C`||
|`'o'`|`P`/`C`||
|`'x'`|`P`/`C`||
|`'X'`|`P`/`C`||
|`'n'`|`P`/`C`||

#### Спецификаторы представления типов с плавающей точкой

|Спецификатор|Язык|Описание|
|:-----------|:---|:-------|
|`'a'`|`P`/`C`||
|`'A'`|`P`/`C`||
|`'e'`|`P`/`C`||
|`'E'`|`P`/`C`||
|`'f'`|`P`/`C`||
|`'F'`|`P`/`C`||
|`'g'`|`P`/`C`||
|`'G'`|`P`/`C`||
|`''`|`P`/`C`||
|`''`|`P`/`C`||
|`''`|`P`/`C`||


### Экранирование строк


#### 


#### 









## Список задач

Список задач по библиотеке - [TODO](todo.md_).



