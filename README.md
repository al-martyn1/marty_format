# Библиотека форматирования marty::format



Библиотека форматирования в стиле `Python` / `C++` `std::format`.


Библиотека предназначена для использования в самодельных скриптовых языках и в языках описания 
прикладных данных (DSL - Domain Specific Language).

Как бонус, существует возможность использования функций форматирования библиотеки вместо 
`std::format` при помощи контейнера `std::initializer_list`.


Замечания, предложения, фиксы - приветствуются.

Ограничений на использование библиотеки нет. Требуется только указывать авторство.


- [Возможности библиотеки](#возможности-библиотеки)
- [Поддержка стандартов и компиляторов](#поддержка-стандартов-и-компиляторов)
- [Зависимости](#зависимости)
- [Ссылки на референсные спецификации](#ссылки-на-референсные-спецификации)
- [Примеры использования](#примеры-использования)
  - [Вызов функций форматирования с разными типами списков аргументов](#вызов-функций-форматирования-с-разными-типами-списков-аргументов)
    - [Используем std::initializer_list](#используем-stdinitializer_list)
    - [Используем std::vector](#используем-stdvector)
    - [Используем marty::format::Args](#используем-martyformatargs)
    - [Используем marty::format::Args с именоваными параметрами](#используем-martyformatargs-с-именоваными-параметрами)
    - [Используем std::vector с парами std::pair<std::string, FormatArgumentVariant>](#используем-stdvector-с-парами-stdpairstdstring-formatargumentvariant)
  - [Конвертация и фильтры](#конвертация-и-фильтры)
    - [Задаём конвертацию аргумента, также символ заполнения передаём аргументом](#задаём-конвертацию-аргумента-также-символ-заполнения-передаём-аргументом)
    - [Используем фильтры для вывода в HTML](#используем-фильтры-для-вывода-в-html)
  - [Примеры использования форматной строки](#примеры-использования-форматной-строки)
    - [Форматирование строк с различной шириной и точностью](#форматирование-строк-с-различной-шириной-и-точностью)
    - [Форматирование булевых значений](#форматирование-булевых-значений)
    - [Форматирование указателей (и целых чисел, как указателей)](#форматирование-указателей-и-целых-чисел-как-указателей)
    - [Форматирование целых чисел](#форматирование-целых-чисел)
    - [Форматирование чисел с плавающей точкой](#форматирование-чисел-с-плавающей-точкой)
      - [Форматирование с использованием спецификаторов a/A/e/E/f/F/g/G](#форматирование-с-использованием-спецификаторов-aaeeffgg)
      - [Форматирование чисел, процентов, валют](#форматирование-чисел-процентов-валют)
- [API библиотеки](#api-библиотеки)
  - [Поддержка локализации форматируемых значений](#поддержка-локализации-форматируемых-значений)
    - [Класс marty::format::LocaleInfo](#класс-martyformatlocaleinfo)
    - [Функция marty::format::getLocaleInfo - получение стандартной локали](#функция-martyformatgetlocaleinfo---получение-стандартной-локали)
      - [marty::format::LocaleInfoType перечисление](#martyformatlocaleinfotype-перечисление)
    - [Создание/формирование собственной локали](#созданиеформирование-собственной-локали)
      - [Модификация предопределённой локали](#модификация-предопределённой-локали)
      - [Переопределение виртуальных методов класса marty::format::LocaleInfo](#переопределение-виртуальных-методов-класса-martyformatlocaleinfo)
    - [Форматная строка для чисел - marty::format::LocaleInfo::format*](#форматная-строка-для-чисел---martyformatlocaleinfoformat)
  - [Обобщённый тип фильтра BasicFormatValueFilter](#обобщённый-тип-фильтра-basicformatvaluefilter)
    - [Пример реализации методов InputIteratorType для использования совместно с BasicFormatValueFilter](#пример-реализации-методов-inputiteratortype-для-использования-совместно-с-basicformatvaluefilter)
  - [Тип фильтра FormatValueFilter](#тип-фильтра-formatvaluefilter)
  - [Мейкер стандартных фильтров по типу StdFilterType - makeStandardFormatValueFilter](#мейкер-стандартных-фильтров-по-типу-stdfiltertype---makestandardformatvaluefilter)
    - [marty::format::StdFilterType перечисление](#martyformatstdfiltertype-перечисление)
  - [Мейкер стандартных фильтров по строке - makeStandardFormatValueFilter](#мейкер-стандартных-фильтров-по-строке---makestandardformatvaluefilter)
  - [Фабрика стандартных фильтров - StdFilterFactory](#фабрика-стандартных-фильтров---stdfilterfactory)
  - [marty::format::FormatArgumentVariant - Variant-тип аргумента](#martyformatformatargumentvariant---variant-тип-аргумента)
  - [marty::format::BasicArgs](#martyformatbasicargs)
    - [Конструктор marty::format::BasicArgs](#конструктор-martyformatbasicargs)
    - [Методы marty::format::BasicArgs::arg](#методы-martyformatbasicargsarg)
  - [marty::format::Args](#martyformatargs)
  - [marty::format::formatMessageImpl](#martyformatformatmessageimpl)
  - [marty::format::FormattingFlags флаги](#martyformatformattingflags-флаги)
  - [marty::format::formatMessage - аргументы передаются в generic-контейнере](#martyformatformatmessage---аргументы-передаются-в-generic-контейнере)
  - [marty::format::formatMessage - аргументы передаются в виде std::initializer_list](#martyformatformatmessage---аргументы-передаются-в-виде-stdinitializer_list)
  - [Базовая реализация функтора вычисления отображаемой ширины строки](#базовая-реализация-функтора-вычисления-отображаемой-ширины-строки)
- [Синтаксис форматной строки](#синтаксис-форматной-строки)
  - [Грамматика полей замены](#грамматика-полей-замены)
  - [Преобразование типа аргумента - !conversion](#преобразование-типа-аргумента---conversion)
  - [Спецификатор формата - format_spec](#спецификатор-формата---format_spec)
    - [Символ заполнения fill](#символ-заполнения-fill)
    - [Маркер выравнивания align](#маркер-выравнивания-align)
    - [Признак знака sign](#признак-знака-sign)
  - [Поддерживаемые спецификаторы типа](#поддерживаемые-спецификаторы-типа)
    - [Спецификаторы представления строк](#спецификаторы-представления-строк)
    - [Спецификаторы представления типа bool](#спецификаторы-представления-типа-bool)
      - [Опции форматирования, влияющие на отображение bool в строковом виде](#опции-форматирования-влияющие-на-отображение-bool-в-строковом-виде)
      - [Спецификаторы](#спецификаторы)
      - [Спецификаторы целых чисел для представления типа bool](#спецификаторы-целых-чисел-для-представления-типа-bool)
    - [Спецификаторы представления целочисленных типов](#спецификаторы-представления-целочисленных-типов)
    - [Спецификаторы представления типов с плавающей точкой](#спецификаторы-представления-типов-с-плавающей-точкой)
    - [Спецификаторы представления указателей](#спецификаторы-представления-указателей)
  - [Встроенные фильтры (поле спецификатора формата filter)](#встроенные-фильтры-поле-спецификатора-формата-filter)
  - [Экранирование строк](#экранирование-строк)



## Возможности библиотеки

1. Добавление возможностей `C++` `std::format` из `C++20` в `C++17` (бэкпорт новых фич в старый стандарт).

2. Совместимость форматной строки с форматной строкой языков `Python` и `C++`. 
   Некоторые возможности этих языков игнорируются, но не вызывают ошибки при разборе
   форматной строки. В библиотеке совмещены возможности форматирования из языка `Python` и возможности 
   форматирования `C++20` `std::format`, в том числе:

   - `'%'` - форматный символ (`Python`) - форматирование процентов. Числа с плавающей точкой умножаются на `100.0`, целые
     сохранают своё значение, после числа добавляется символ `'%'` (или его локализованный вариант).
     Форматирование производится как для спецификатора `'d'`;
   - `'n'` - форматный символ (`Python`) - форматирование с учетом системной локали.
     Форматирование производится как для спецификатора `'d'`;
   - `'L'` - опция форматирования (`C++`) - форматирование с учетом системной локали.
     Форматирование производится с учетом заданного спецификатора типа;
   - `'='` - маркер выравнивания по ширине (`Python`). При использовании данного типа выравнивания знак числа 
     отделяется от числа `'fill'`-символом, если не указан ведущий `'0'` при задании ширины, во втором случае 
     заполнение производится нулями;
   - **Разделители разрядов** в форматной строке (`Python`);

3. Поддержка именованных аргументов для `C++` при использовании соответствующих контейнеров для аргументов.

4. Поддержка передачи аргументов через `std::initializer_list` для непосредственного использования из `C++`.

5. Поддержка дополнительных оригинальных фич:
  
  - `'$'` - форматный символ - форматирование валюты. В отформатированную строку добавляется символ `'$'`
     (или символ локальной валюты пользователя). Форматирование производится как для спецификатора `'d'`
     с использованием символа валюты (по умолчанию - `'$#'`);
  - добавлены форматные символы `'S'` - для отображения булевых значений (`C++`) в верхнем регистре в виде строки;
    `'y'`, `'Y'` - для отображения `bool` значений (`C++`) в виде строк `'yes'`/`'no'` (см. далее),
    а также `'t'`, `'T'` (см. далее);
  - добавлены форматные символы `'h'` и `'H'` - форматирование чисел в шестнадцатиричном виде,
    при этом в альтернативной форме добавляется суффикс `'h'` и `'H'` (как в ассемблере).
  - `grouping` - добавлен символ группировки `'''` (`apos`), который отсутстсвует в `Python`;
  - `frac_grouping` (отсутствует в `Python` и `C++`) - возможность группировки цифр дробной части чисел 
    с плавающей точкой отдельно явно задаваемым символом, или таким же символом группировки, как и для 
    целой части числа, при задании соответствующего флага при вызове функции форматирования;
  - `'~'` - битовый каст отрицательных знаковых чисел в беззнаковые такого же размера;
  - `'!'` - инверсия регистра:
    - при форматировании `bool` изменяется регистр первой буквы;
    - инверсия регистра в альтернативной форме при форматировании целых чисел `'!#'` - меняет регистр 
      префикса системы счисления (например, `'0x'`) на противоположный, по сравнению с регистром символов числа;
  - **фильтры** для отформатированных значений. Можно использовать цепочки фильтров длиной до 4х.
    Фильтры могут экранировать символы для вывода в `XML`/`HTML` или в `SQL`.
  - **форматирование двоичных и шестнадцатиричных чисел по ширине типа**: если использован префикс ширины `'0'`,
    но ширина не задана или явно задана нулевой (`'{:0b}'`/`'{:00b}'`/`'{:0X}'`), то количество выводимых цифр 
    будет зависеть от размера выводимого типа -  `'00000000000000000000000110100100'`/`'000001A4'` для 32ух-разрядных
    целых.

6. Упор на использование библиотеки в своих скриптовых языках/DSL языках,
   в том числе:

  - спецификаторы формата `'t'` и `'T'` - для отображения целых чисел, как `bool` в виде строки `'true'`/`'false'`
    для использования в собственных ЯП/DSL, в которых нет типа `bool`;
  - спецификаторы формата `'y'`, `'Y'`  - для отображения целых чисел, как `bool` в виде строки `'yes'`/`'no'`
    для использования в собственных ЯП/DSL, в которых нет типа `bool`.

7. Поддержка Unicode в кодировке UTF-8.

8. Поддержка чисел с плавающей точкой произвольной размерности `marty::Decimal`.

9. Поддержка пользовательских типов строк вместо `std::string` и поддержка микроконтроллеров 
   (`STM32`, как пример). **Пока не реализовано, но весьма ready**, требует отключения части кода 
   условными макросами, также требуется модифицировать некоторые части, пока завязанные на `std::string`.

10. Возможность создания собственной системы/библиотеки форматирования на базе данной библиотеки с поддержкой
   вывода собственных произвольных типов данных.




## Поддержка стандартов и компиляторов

Поддерживаются компиляторы, соответсвующие стандарту языка `C++` - `C++17`.

Для компиляторов `MSVC` и `GCC` есть исключения.
Данные компиляторы языка `C++` не всегда соответсвуют заявленному ими стандарту `C++`,
и для них сделаны исключения и обходные реализации некоторых возможностей.

Поддерживаемые версии `MSVC` и `GCC`:

- `MSVC` - MS Visual Studio 2019 и соответсвующий данной версии MSVS компилятор `C++` от Microsoft.
- `GCC` - Поддерживается `GCC` вплоть до версии 7.3. Это сделано для возможности работы с библиотекой Qt старых версий (до 5.12 включительно)

Всё прочие компиляторы должны безусловно поддерживать стандарт языка `C++` - `C++17` для использования данной библиотеки.



## Зависимости

- [marty_cpp](https://github.com/al-martyn1/marty_cpp) - от этого никуда не деться, базовые зависимости, enum как флаги и база для (де)сериализации в строки.
- [marty_decimal](https://github.com/al-martyn1/marty_decimal) - поддержка чисел с плавающей точкой и произвольной размерностью. Потенциально отключаемо.
- [marty_utf](https://github.com/al-martyn1/marty_utf) - поддержка UTF-8. Потенциально отключаемо.


## Ссылки на референсные спецификации

- `Python`: Описание форматной строки - Format String Syntax - https://docs.python.org/3/library/string.html#format-string-syntax
- `C++`: `std::format` - https://en.cppreference.com/w/cpp/utility/format/format
- `C++`: `Standard format specification` - https://en.cppreference.com/w/cpp/utility/format/spec


## Примеры использования


### Вызов функций форматирования с разными типами списков аргументов

Все примеры используют стандартный тип аргумента `marty::format::FormatArgumentVariant`.
При необходимости пользователь может создать свой аналогичный тип и использовать его.

#### Используем std::initializer_list

```cpp
using std::cout;
using namespace marty::format;
cout << formatMessage("Integer number: {:d}, string: {:{}.{}s}, Pi: {:f}"
                     , { 10, "Very long string, does not fit into 20 characters"
                       , 10, 20, 3.14159
                       }
                     ) << "\n";
```

**Вывод:**

```
Integer number: 10, string: Very long string, do, Pi: 3.142
```


#### Используем std::vector

```cpp
using std::cout;
using namespace marty::format;
auto argsVec = std::vector<FormatArgumentVariant>{ 10, "Very long string, "
                      "does not fit into 20 characters", 10, 20, 3.14159 };
cout << formatMessage( "Integer number: {:d}, string: {:{}.{}s}, Pi: {:f}"
                     , argsVec
                     ) << "\n";
```

**Вывод:**

```
Integer number: 10, string: Very long string, do, Pi: 3.142
```


#### Используем marty::format::Args

- Автоматически вычисляемый индекс аргумента
- Ширину и точность (на самом деле макс ширину строки)
  задаём также аргументами, а не в форматной строке


```cpp
using std::cout;
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

**Вывод:**

```
Integer number: 10, string: Very long string, do, Pi: 3.142
```


#### Используем marty::format::Args с именоваными параметрами

```cpp
using std::cout;
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

**Вывод:**

```
Integer number: 10, string: Very long string, do, Pi: 3.142
```


#### Используем std::vector с парами std::pair<std::string, FormatArgumentVariant>

Использование вектора с парами, где первый элемент строка, а второй - `marty::format::FormatArgumentVariant` 
аналогично использованию `marty::format::Args`,
но поиск по имени каждый раз производится перебором от начала вектора. Не слишком эффективно, но работает без лишних сущностей.

```cpp
using std::cout;
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

**Вывод:**

```
Integer number: 10, string: Very long string, do, Pi: 3.142
```



### Конвертация и фильтры

#### Задаём конвертацию аргумента, также символ заполнения передаём аргументом

```cpp
using std::cout;
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

**Вывод:**

```
Integer number: 10, string: Very long string, do, Pi: 3.142
```


#### Используем фильтры для вывода в HTML

```cpp
using std::cout;
using namespace marty::format;
cout << formatMessage( "<a href=\"{url|html-attr}\">{text|html-text}</a>\n"
                     , Args().arg("url" , "http://site.com/&q=/'")
                             .arg("text", "<Some & text>")
                     );
```

**Вывод:**

```
<a href="http://site.com/&amp;q=/&apos;">&lt;Some &amp; text&gt;</a>
```



### Примеры использования форматной строки

#### Форматирование строк с различной шириной и точностью

При выводе используем как непосредственное задание ширины и точности, так и косвенное.

```cpp
using std::cout;
using namespace marty::format;
cout << formatMessage( //---
                       "Alignment names explicitly taken\n"
                       "Width: 20, precision: 13, explicit fill chars\n"
                       "str: |{strL20}|\n"
                       "Default  aligned: |{strL20:*20.13}|\n"
                       "Left     aligned: |{strL20:*<20.13}|\n"
                       "Right    aligned: |{strL20:*>20.13}|\n"
                       "Center   aligned: |{strL20:*^20.13}|\n"
                       "//---\n"
                       "Alignment names indirect taken\n"
                       "Width: 20, precision: 13, explicit fill chars\n"
                       "str: |{strL20}|\n"
                       "{adname:{anamew}} aligned: |{strL20:$20.13}|\n"
                       "{alname:{anamew}} aligned: |{strL20:$<20.13}|\n"
                       "{arname:{anamew}} aligned: |{strL20:$>20.13}|\n"
                       "{acname:{anamew}} aligned: |{strL20:$^20.13}|\n"
                       "//---\n"
                       "Width: 18, precision: 9, default fill char\n"
                       "str: |{strL20}|\n"
                       "{adname:{anamew}} aligned: |{strL20:20.9}|\n"
                       "{alname:{anamew}} aligned: |{strL20:<20.9}|\n"
                       "{arname:{anamew}} aligned: |{strL20:>20.9}|\n"
                       "{acname:{anamew}} aligned: |{strL20:^20.9}|\n"
                       "//---\n"
                       "Width (I): {w1}, precision (I): {p1}\n"
                       "str: |{strL20}|\n"
                       "{adname:{anamew}} aligned: |{strL20:@{w1}.{p1}}|\n"
                       "{alname:{anamew}} aligned: |{strL20:@<{w1}.{p1}}|\n"
                       "{arname:{anamew}} aligned: |{strL20:@>{w1}.{p1}}|\n"
                       "{acname:{anamew}} aligned: |{strL20:@^{w1}.{p1}}|\n"
                       // "//---\n"
                     , Args().arg("anamew", 8)            // alignment name width
                             .arg("adname", "Default")    // name for default alignment
                             .arg("alname", "Left")       // name for left alignment
                             .arg("arname", "Right")      // name for right alignment
                             .arg("acname", "Center")     // name for center alignment
                             //---
                             .arg("w1", 16).arg("p1", 13) // indirect width & precision #1
                             //---
                             .arg("strL20", "String larger than 20")
                             .arg("strS19", "Str smaler than 19")
                             .arg("strS4" , "S<4")
                             .arg("strL5" , "Str > 5")
                     );
```

**Вывод:**

```
Alignment names explicitly taken
Width: 20, precision: 13, explicit fill chars
str: |String larger than 20|
Default  aligned: |String larger*******|
Left     aligned: |String larger*******|
Right    aligned: |*******String larger|
Center   aligned: |***String larger****|
//---
Alignment names indirect taken
Width: 20, precision: 13, explicit fill chars
str: |String larger than 20|
Default  aligned: |String larger|
Left     aligned: |String larger       |
Right    aligned: |String larger       |
Center   aligned: |String larger       |
//---
Width: 18, precision: 9, default fill char
str: |String larger than 20|
Default  aligned: |String la           |
Left     aligned: |String la           |
Right    aligned: |           String la|
Center   aligned: |     String la      |
//---
Width (I): 16, precision (I): 13
str: |String larger than 20|
Default  aligned: |String larger@@@|
Left     aligned: |String larger@@@|
Right    aligned: |@@@String larger|
Center   aligned: |@String larger@@|
```


#### Форматирование булевых значений

Также выводим целые числа как булевы значения.

```cpp
using std::cout;
using namespace marty::format;
cout<<formatMessage
        ( "Bool as string, true : {bt:s}, false: {bf:s}\n"
          "Bool as string, upper case: {bt:S}, {bf:S}\n"
          "Bool as string, mixed case (first char upper): {bt:!s}, {bf:!}\n"
          "Bool as string, mixed case (first char lower): {bt:!S}, {bf:!S}\n"
          "Bool as string, single char (using precision): {bt:.1S}, {bf:.1S}\n"
          "Bool as string, (using spec-t): {bt:t}, {bf:t}\n"
          "Bool as string, (using spec-T): {bt:T}, {bf:T}\n"
          "Bool as string, (using spec-y): {bt:y}, {bf:y}\n"
          "Bool as string, (using spec-Y): {bt:Y}, {bf:Y}\n"
          "Bool as string, (using spec-t#): {bt:#t}, {bf:#t}\n"
          "Bool as string, (using spec-T#): {bt:#T}, {bf:#T}\n"
          "Bool as string, (using spec-y#): {bt:#y}, {bf:#y}\n"
          "Bool as string, (using spec-Y#): {bt:#Y}, {bf:#Y}\n"
          "Unsigned as bool string, (using spec-y#): {ut:#y}, {uf:#y}, as native: {ut:d}, {uf:d}\n"
          "Unsigned as bool string, (using spec-Y#): {ut:#Y}, {uf:#Y}, as native: {ut:d}, {uf:d}\n"
          "Unsigned as bool string, (using spec-t#): {ut:#t}, {uf:#t}, as native: {ut:d}, {uf:d}\n"
          "Unsigned as bool string, (using spec-T#): {ut:#T}, {uf:#T}, as native: {ut:d}, {uf:d}\n"
          "Int as bool string, (using spec-y): {st:y}, {sf:y}, as native: {st:d}, {sf:d}\n"
          "Int as bool string, (using spec-Y): {st:Y}, {sf:Y}, as native: {st:d}, {sf:d}\n"
          "Int as bool string, (using spec-t): {st:t}, {sf:t}, as native: {st:d}, {sf:d}\n"
          "Int as bool string, (using spec-T): {st:T}, {sf:T}, as native: {st:d}, {sf:d}\n"
        , Args().arg("bt", true).arg("bf", false)
                .arg("ut", 42u) .arg("uf", 0u)
                .arg("st", -42) .arg("sf", 0)
        );
```

**Вывод:**

```
Bool as string, true : true, false: false
Bool as string, upper case: TRUE, FALSE
Bool as string, mixed case (first char upper): True, False
Bool as string, mixed case (first char lower): tRUE, fALSE
Bool as string, single char (using precision): T, F
Bool as string, (using spec-t): true, false
Bool as string, (using spec-T): TRUE, FALSE
Bool as string, (using spec-y): yes, no
Bool as string, (using spec-Y): YES, NO
Bool as string, (using spec-t#): t, f
Bool as string, (using spec-T#): T, F
Bool as string, (using spec-y#): y, n
Bool as string, (using spec-Y#): Y, N
Unsigned as bool string, (using spec-y#): y, n, as native: 42, 0
Unsigned as bool string, (using spec-Y#): Y, N, as native: 42, 0
Unsigned as bool string, (using spec-t#): t, f, as native: 42, 0
Unsigned as bool string, (using spec-T#): T, F, as native: 42, 0
Int as bool string, (using spec-y): yes, no, as native: -42, 0
Int as bool string, (using spec-Y): YES, NO, as native: -42, 0
Int as bool string, (using spec-t): true, false, as native: -42, 0
Int as bool string, (using spec-T): TRUE, FALSE, as native: -42, 0
```


#### Форматирование указателей (и целых чисел, как указателей)

```cpp
using std::cout;
using namespace marty::format;
cout << formatMessage( "Argv Pointer: {argv:P}\n"
                       "Short as ptr: {ptrShort:P}\n"
                     , Args().arg("argv", (void*)argv)
                             .arg("ptrShort", short(-1234))
                     );
```

**Вывод:**

```
Argv Pointer: 000001D9EF03EA30
Short as ptr: FB2E
```


#### Форматирование целых чисел

```cpp
// Форматирование в 2ую СС
cout << formatMessage("|{:0b}|",   {-1420})<<"\n"; // |-00000000000000000000010110001100|
cout << formatMessage("|{:0'b}|",  {-1420})<<"\n"; // |-0000'0000'0000'0000'0000'0101'1000'1100|
cout << formatMessage("|{:12b}|",  {-1420})<<"\n"; // |-10110001100|
cout << formatMessage("|{:12'b}|", {-1420})<<"\n"; // |-101'1000'1100|
cout << formatMessage("|{:#0b}|",  {-1420})<<"\n"; // |-0b00000000000000000000010110001100|
cout << formatMessage("|{:~0'b}|", {-1420})<<"\n"; // |1111'1111'1111'1111'1111'1010'0111'0100|
cout << formatMessage("|{:#0'b}|", {-1420})<<"\n"; // |-0b0000'0000'0000'0000'0000'0101'1000'1100|
cout << formatMessage("|{:#12b}|", {-1420})<<"\n"; // |-0b10110001100|
cout << formatMessage("|{:#12'b}|",{-1420})<<"\n"; // |-0b101'1000'1100|

// Фрматирование в 16ую СС
cout << formatMessage("|{:0x}|",        { -1420 }) << "\n"; // |-0000058c|
cout << formatMessage("|{:0X}|",        { -1420 }) << "\n"; // |-0000058C|
cout << formatMessage("|{:0'x}|",       { -1420 }) << "\n"; // |-0000'058c|
cout << formatMessage("|{:12X}|",       { -1420 }) << "\n"; // |-58C        |
cout << formatMessage("|{:#0h}|",       { -1420 }) << "\n"; // |-0000058ch|
cout << formatMessage("|{:#12x}|",      { -1420 }) << "\n"; // |-0x58c      |
cout << formatMessage("|{:!#0X}|",      { -1420 }) << "\n"; // |-0x0000058C|
cout << formatMessage("|{:~!#0X}|",     { -1420 }) << "\n"; // |0xFFFFFA74|
cout << formatMessage("|{:~!#0'X}|",    { -1420 }) << "\n"; // |0xFFFF'FA74|
cout << formatMessage("|{:^12x}|",      { -1420 }) << "\n"; // |    -58c    |
cout << formatMessage("|{:^12X}|",      { -1420 }) << "\n"; // |    -58C    |
cout << formatMessage("|{:^~12X}|",     { -1420 }) << "\n"; // |  FFFFFA74  |
cout << formatMessage("|{:^+~12x}|",    { -1420 }) << "\n"; // | +fffffa74  |

// Форматирование в 10ую СС (по умолчанию спецификатор 'd' можно не указывать)
cout << formatMessage("|{:12}|",        { -1420 }) << "\n"; // |-1420       |
cout << formatMessage("|{:12%}|",       { -1420 }) << "\n"; // |-1420%      |
cout << formatMessage("|{:*=12}|",      { -1420 }) << "\n"; // |-*******1420|
cout << formatMessage("|{:*<12}|",      { -1420 }) << "\n"; // |-1420*******|
cout << formatMessage("|{:*<12'}|",     { -1420 }) << "\n"; // |-1'420******|
cout << formatMessage("|{:*<12'%}|",    { -1420 }) << "\n"; // |-1'420%*****|
cout << formatMessage("|{:>12%}|",      { -1420 }) << "\n"; // |      -1420%|
cout << formatMessage("|{:>12'}|",      { -1420 }) << "\n"; // |      -1'420|
cout << formatMessage("|{:>12'%}|",     { -1420 }) << "\n"; // |     -1'420%|
cout << formatMessage("|{:^12}|",       { -1420 }) << "\n"; // |   -1420    |
cout << formatMessage("|{:^12%}|",      { -1420 }) << "\n"; // |   -1420%   |
cout << formatMessage("|{:^12'}|",      { -1420 }) << "\n"; // |   -1'420   |

// Форматирование по ширине с заполнением fill-символом
// (без ведущего нуля в поле ширины)
cout << formatMessage("|{:=12}|",       { -1420 }) << "\n"; // |-       1420|
cout << formatMessage("|{:=12b}|",      {  1420 }) << "\n"; // | 10110001100|
cout << formatMessage("|{:=12x}|",      { -1420 }) << "\n"; // |-        58c|
cout << formatMessage("|{:=12X}|",      { -1420 }) << "\n"; // |-        58C|
cout << formatMessage("|{:=12%}|",      { -1420 }) << "\n"; // |-      1420%|
cout << formatMessage("|{:*=12}|",      { -1420 }) << "\n"; // |-*******1420|
cout << formatMessage("|{:*=12b}|",     {  1420 }) << "\n"; // |*10110001100|
cout << formatMessage("|{:*=+!#12X}|",  {  1420 }) << "\n"; // |+******0x58C|
cout << formatMessage("|{:= ~!#012X}|", { -1420 }) << "\n"; // | 0x0FFFFFA74|
```


#### Форматирование чисел с плавающей точкой

Для краткости приводится только форматная строка, использованное при форматировании число (возможно, 
с отбрасыванием точности), и результат форматирования. обрамлённый символами `'|'` (для более
наглядного отображения опций выравнивания).


##### Форматирование с использованием спецификаторов a/A/e/E/f/F/g/G

Также продемонстрированно использование различного заполнения и выравнивания, и разделения на разряды.


```
{:.05f}         with  3.141593       : |3.14159|
{:.020f}        with  3.141593       : |3.14159265358979311600|
{:.05f}         with  -3.141593      : |-3.14159|
{:.020f}        with  -3.141593      : |-3.14159265358979311600|
{:.5f}          with  3.141593       : |3.14159|
{:.20f}         with  3.141593       : |3.14159265358979311600|

{:09.05f}       with  3.141593       : |003.14159|
{:010.05f}      with  -3.141593      : |-003.14159|
{:010.05f}      with  -3.141593      : |-003.14159|
{:+010.05f}     with  3.141593       : |+003.14159|

{:= 010.05f}    with  3.141593       : | 003.14159|
{:=+010.05f}    with  3.141593       : |+003.14159|
{:=-010.05f}    with  3.141593       : |0003.14159|
{:= 010.05f}    with  -3.141593      : |-003.14159|
{:=+010.05f}    with  -3.141593      : |-003.14159|
{:=-010.05f}    with  -3.141593      : |-003.14159|

{:= 10.05f}     with  3.141593       : |   3.14159|
{:=+10.05f}     with  3.141593       : |+  3.14159|
{:=-10.05f}     with  3.141593       : |   3.14159|
{:= 10.05f}     with  -3.141593      : |-  3.14159|
{:=+10.05f}     with  -3.141593      : |-  3.14159|
{:=-10.05f}     with  -3.141593      : |-  3.14159|

{:= 10.05f}     with  3.141593       : |   3.14159|
{:=+10.05f}     with  3.141593       : |+  3.14159|
{:=-10.05f}     with  3.141593       : |   3.14159|
{:= 10.05f}     with  -3.141593      : |-  3.14159|
{:=+10.05f}     with  -3.141593      : |-  3.14159|
{:=-10.05f}     with  -3.141593      : |-  3.14159|

{:< 10.05f}     with  3.141593       : | 3.14159  |
{:<+10.05f}     with  3.141593       : |+3.14159  |
{:<-10.05f}     with  3.141593       : |3.14159   |
{:< 10.05f}     with  -3.141593      : |-3.14159  |
{:<+10.05f}     with  -3.141593      : |-3.14159  |
{:<-10.05f}     with  -3.141593      : |-3.14159  |

{:> 10.05f}     with  3.141593       : |   3.14159|
{:>+10.05f}     with  3.141593       : |  +3.14159|
{:>-10.05f}     with  3.141593       : |   3.14159|
{:> 10.05f}     with  -3.141593      : |  -3.14159|
{:>+10.05f}     with  -3.141593      : |  -3.14159|
{:>-10.05f}     with  -3.141593      : |  -3.14159|

{:^ 010.05f}    with  3.141593       : |  3.14159 |
{:^+010.05f}    with  3.141593       : | +3.14159 |
{:^-010.05f}    with  3.141593       : | 3.14159  |
{:^ 010.05f}    with  -3.141593      : | -3.14159 |
{:^+010.05f}    with  -3.141593      : | -3.14159 |
{:^-010.05f}    with  -3.141593      : | -3.14159 |

{:.05f}         with  2.718282       : |2.71828|
{:.020f}        with  2.718282       : |2.71828182845904509080|

{:012'.05f}     with  2.718282       : |00'002.71828|
{:012'.018f}    with  2.718282       : |2.718281828459045091|
{:012'.05'f}    with  2.718282       : |0'002.718'28|
{:012'.018'f}   with  2.718282       : |2.718'281'828'459'045'091|
{:012'.5'f}     with  2.718282       : |0'002.718'28|
{:012'.18'f}    with  2.718282       : |2.718'281'828'459'045'091|
```

##### Форматирование чисел, процентов, валют

Также продемонстрированно использование пользовательской локали, получаемой из системы.

```
//---------------------
// n - десятичное число, форматирование с использованием локали.
// Точность не задана, берётся из текущей локали пользователя
// (русская под Win - 2 десятичных знака)
{:n}            with  3.141593       : |3.14|

// n - десятичное число, форматирование с использованием локали.
// Точность задана явно, .4.
{:.4n}          with  3.141593       : |3.1416|

// n - десятичное число, форматирование с использованием локали.
// Точность задана явно, .6.
// Указано разделение на разряды дробной части.
// Разделитель используется из локали пользователя в ОС.
{:.6_n}         with  3.141593       : |3.141 593|

//---------------------
// d - число. Точность не задана, берётся из локали
// (дефолтная "C"/invariant локаль - 3 символа)
{:d}            with  3.141593       : |3.142|

// d - число. Точность не задана, берётся из пользовательской локали
// (русская под Win - 2 десятичных знака)
// Маркер локали 'L' указывает использовать локаль из ОС
{:Ld}           with  3.141593       : |3.14|

// d - число. Точность не задана, альтер маркер '#' указывает
// выбрать точность автоматически
{:#d}           with  3.141593       : |3.141593|

//---------------------
// % - проценты - число умножается на 100.
// Точность не задана, берётся из invariant локали - 3 символа
{:%}            with  3.141593       : |314.159%|

// % - проценты - число умножается на 100.
// Точность не задана, берётся из пользовательской локали - 2 символа
{:L%}           with  3.141593       : |314.16%|

// % - проценты, альтернативная форма, знак '%' опускается, invariant локаль, .3.
{:#%}           with  3.141593       : |314.159|

// % - проценты, альтернативная форма, знак '%' опускается, пользовательская локаль, .2.
{:#L%}          with  3.141593       : |314.16|

// % - проценты, точность задаётся явно.
{:.4%}          with  3.141593       : |314.1593%|

//---------------------
// $ - валюта.
// Точность не задана, берётся из invariant локали - 2 символа
{:$}            with  3.141593       : |$3.14|

// $ - валюта.
// Точность не задана, берётся из пользовательской локали - 2 символа
{:L$}           with  3.141593       : |3.14 ₽|

// $ - валюта, альтернативная форма, знак валюты опускается, invariant локаль, .2.
{:#$}           with  3.141593       : |3.14|

// $ - валюта, альтернативная форма, знак '$' опускается, пользовательская локаль, .2.
{:#L$}          with  3.141593       : |3.14|

// $ - валюта, точность задаётся явно.
{:.4$}          with  3.141593       : |$3.1416|


//---------------------
// Краткий сводный пример форматирования чисел, процентов, валюты.

{:n}            with  3.141593       : |3.14|

// 9 знаков после запятой, разбиение дробной части по разрядам.
// Используется локаль пользователя системы.
// Разделитель - неразрывный пробел (&nbsp;) - локаль RU-ru.
{:.9'n}         with  3.141593       : |3.141 592 654|

{:d}            with  3.141593       : |3.142|
{:.9'd}         with  3.141593       : |3.141'592'654|
{:f}            with  3.141593       : |3.142|
{:.9'f}         with  3.141593       : |3.141'592'654|
{:%}            with  3.141593       : |314.159%|
{:L%}           with  3.141593       : |314.16%|
{:#L%}          with  3.141593       : |314.16|
{:$}            with  3.141593       : |$3.14|
{:L$}           with  3.141593       : |3.14 ₽|
{:#L$}          with  3.141593       : |3.14|
```









## API библиотеки

### Поддержка локализации форматируемых значений

#### Класс marty::format::LocaleInfo

Класс `LocaleInfo` является интерфейсом, 
через который библиотека форматирования получает locale-зависимую информацию.

Всё получение locale-зависимой информации функциями форматирования производится 
через виртуальные методы класса `LocaleInfo`.

Функции форматирования принимают указатель на класс `LocaleInfo` для
переопределения локали, получаемой из системы. Данный указатель
позволяет передать собственную настроенную локаль в функции форматирования.

Если в функции форматирования передаётся нулевой указатель на `LocaleInfo`,
то используется либо пользовательская, либо системная локаль текущего пользовательского окружения.
По умолчанию используется пользовательская локаль текущего окружения пользователя.
Для использования системной локали текущего окружения пользователя следует указывать флаг 
`FormattingFlags::localeUseSystem`.

Если в форматной строке не задано использование локали, то для форматирования 
используется `invariant`/`"C"`-локаль.

```cpp
class LocaleInfo
{

public: // constructors

    LocaleInfo();

    LocaleInfo(const LocaleInfo &) = default;
    LocaleInfo& operator=(const LocaleInfo &) = default;
    LocaleInfo(LocaleInfo &&) = default;
    LocaleInfo& operator=(LocaleInfo &&) = default;


public: // types

// Для отладки удобнее std::vector<std::uint8_t>,
// а в релизе std::basic_string<std::uint8_t> гораздо быстрее
#if defined(DEBUG) || defined(_DEBUG)
    using group_info_t = std::vector<std::uint8_t>;
    static void groupInfoAppend(group_info_t &gi, std::size_t n, std::uint8_t u)
    {
        gi.resize(gi.size()+n, u);
    }
#else
    using group_info_t = std::basic_string<std::uint8_t>;
    static void groupInfoAppend(group_info_t &gi, std::size_t n, std::uint8_t u)
    {
        gi.append(n, u);
    }
#endif


public: // virtual methods

    virtual ~LocaleInfo() {}

    virtual std::string getGroupSeparator( LocaleInfoSeparatorType vt //!
                                         , NumeralSystem ns       //!
                                         ) const;

    //! Обрабатывает LocaleInfoValueType::sign* и LocaleInfoValueType::format*
    virtual std::string getLocaleInfoValue(LocaleInfoValueType vt) const;

    virtual group_info_t getGroupInfo(NumeralSystem ns, bool bFractionalPart) const;

    virtual std::string substFormatString( const std::string &fmt
                                         , const std::string &numStr
                                         , PositiveNumbersMode pm
                                         ) const;

    // digitsDecimal / digitsCurrency
    virtual unsigned getNumberOfDigits(LocaleInfoDigitsType vt) const;


public: // members

    // Integer part/Fractional part
    group_info_t groupInfoCur[2]; // Currency
    group_info_t groupInfoBin[2];
    group_info_t groupInfoOct[2];
    group_info_t groupInfoDec[2];
    group_info_t groupInfoHex[2];

    // Integer part/Fractional part
    std::string groupSeparatorCur[2];
    std::string groupSeparatorBin[2];
    std::string groupSeparatorOct[2];
    std::string groupSeparatorDec[2];
    std::string groupSeparatorHex[2];

    std::string signDecimal            = ".";
    std::string signPlus               = "+";
    std::string signMinus              = "-";
    std::string signPercent            = "%";
    std::string signCurrency           = "$";

    std::string formatPercentNegative       = "-#%";
    std::string formatPercentPositive       = "+#%";
    std::string formatPercentShortNegative  = "-#" ;
    std::string formatPercentShortPositive  = "+#" ;
    std::string formatCurrencyNegative      = "-$#";
    std::string formatCurrencyPositive      = "+$#";
    std::string formatCurrencyShortNegative = "-#" ;
    std::string formatCurrencyShortPositive = "+#" ;

    // Числа любой СС форматируем по тем же правилам.
    // Только перед подстановкой в форматную строку строки числа могут быть дополнены
    // или пробелами или нулями, в зависимости от режима выравнивания
    std::string formatNumberNegative   = "-#";
    std::string formatNumberPositive   = "+#";

    unsigned digitsNumber              = 3;
    unsigned digitsCurrency            = 2;

public: // static helper methods

    //! Подстановка локализованных символов в шаблон форматной строки
    static
    std::string substFormatString( const std::string &fmt
                                 , const std::string &numStr
                                 , const std::string &symbolPlus
                                 , const std::string &symbolMinus
                                 , const std::string &symbolPercent
                                 , const std::string &symbolCurrency
                                 , PositiveNumbersMode pm
                                 );

    //! Нулем может быть только последний элемент.
    /*! Данная функция фиксит разобранное из строки описание групп
        При дальнейшей работе с группами элемент с нулевым размером
        будет считаться последним
     */
    static group_info_t fixGroupInfo(group_info_t &&grpInfo);

    //! Разбор строк вида "3;2;0" в group_info_t
    static group_info_t parseGroupingString(const std::string &str);

    //! Восстановление group_info_t в строку
    static std::string composeGroupingString(const group_info_t &grpInfo);


    // 3;0     3,000,000,000,000
    // 3;2;0   30,00,00,00,00,000
    // 3       3000000000,000
    // 3;2     30000000,00,000
    static bool testAppendGroupSeparator(std::size_t curDigitIndex, const group_info_t &grpInfo);

    static std::string insertGroupSeparators( std::string numStr
                                            , std::string sep
                                            , const group_info_t &grpInfo
                                            , bool bFractionalPart
                                            );

    // полная длина дополняемой строки, включает разделители разрядов
    static
    std::string expandWithGroupSeparator
            ( std::string numStr, std::string sep, const group_info_t &grpInfo
            , bool bFractionalPart
            , std::size_t sepCalculatedLen // Посчитанная снаружи длина разделителя
            , std::size_t &numStrLen       // Посчитанная снаружи полная длина дополняемой строки
            , std::size_t &digitsCount
            , std::size_t maxLen
            );

    // maxLen - Макс длина в цифрах
    static
    std::string expandWithGroupSeparatorToNumDigits( std::string numStr
                                                   , std::string sep
                                                   , const group_info_t &grpInfo
                                                   , bool bFractionalPart
                                                   , std::size_t &digitsCount
                                                   , std::size_t maxLen
                                                   );

protected: // static helper methods

    static
    std::string insertGroupSeparatorsImplHelper( const std::string &numStr
                                               , const std::string &sep
                                               , const group_info_t &grpInfo
                                               );

    /*!
        @param sepCalculatedLen Посчитанная снаружи длина разделителя
        @param numStrLen Посчитанная снаружи полная длина строки, которую дополняем,
        включая сепараторы
    */
    static
    std::string expandWithGroupSeparatorImplHelper( std::string numStr
                                                  , const std::string &sep
                                                  , const group_info_t &grpInfo
                                                  , std::size_t sepCalculatedLen
                                                  , std::size_t &numStrLen
                                                  , std::size_t &digitsCount
                                                  , std::size_t maxLen
                                                  );

    static
    std::string expandWithGroupSeparatorToNumDigitsImplHelper( std::string numStr
                                                             , const std::string &sep
                                                             , const group_info_t &grpInfo
                                                             , std::size_t &digitsCount
                                                             , std::size_t maxLen
                                                             );

}; // class LocaleInfo
```


#### Функция marty::format::getLocaleInfo - получение стандартной локали

Функция для получения одной из стандартных локалей.

```cpp
const LocaleInfo* getLocaleInfo(LocaleInfoType lt);
```

Запрашиваемые локали имеют тип `marty::format::LocaleInfoType`.

При получении неизвестного типа локали данная функция выбросит исключение.


##### marty::format::LocaleInfoType перечисление

Базовый тип: `std::uint32_t`.


|Имя|Значение|Описание|
|:-------|:-------|:-------|
|**invalid**, **unknown**|(std::uint32_t)(-1)|Недопустимое/неизвестное значение.|
|**invariant**, **c**|0x00|Инвариантная (неизменная) локаль ("C"-локаль).|
|**user**|0x01|Пользовательская локаль в операционной системе.|
|**system**|0x02|Системная локаль (локаль операционной системы).|





#### Создание/формирование собственной локали

##### Модификация предопределённой локали

В базовом классе локали `LocaleInfo` есть следующие поля:

```cpp
// Integer part/Fractional part
group_info_t groupInfoCur[2]; // Currency
group_info_t groupInfoBin[2];
group_info_t groupInfoOct[2];
group_info_t groupInfoDec[2];
group_info_t groupInfoHex[2];

// Integer part/Fractional part
std::string groupSeparatorCur[2];
std::string groupSeparatorBin[2];
std::string groupSeparatorOct[2];
std::string groupSeparatorDec[2];
std::string groupSeparatorHex[2];

std::string signDecimal            = ".";
std::string signPlus               = "+";
std::string signMinus              = "-";
std::string signPercent            = "%";
std::string signCurrency           = "$";

std::string formatPercentNegative       = "-#%";
std::string formatPercentPositive       = "+#%";
std::string formatPercentShortNegative  = "-#" ;
std::string formatPercentShortPositive  = "+#" ;
std::string formatCurrencyNegative      = "-$#";
std::string formatCurrencyPositive      = "+$#";
std::string formatCurrencyShortNegative = "-#" ;
std::string formatCurrencyShortPositive = "+#" ;

// Числа любой СС форматируем по тем же правилам.
// Только перед подстановкой в форматную строку строки числа могут быть дополнены
// или пробелами или нулями, в зависимости от режима выравнивания
std::string formatNumberNegative   = "-#";
std::string formatNumberPositive   = "+#";

unsigned digitsNumber              = 3;
unsigned digitsCurrency            = 2;
```

Поля, являющиеся массивами из двух элементов, задают значения для целой и дробной частей соответсвующих
форматов, где нулевой индекс отвечает за целую часть, а единичный - за дробную.

Для создания собственной локали достаточно получить указатель на одну из
предопределённых локалей при помощи функции `marty::format::getLocaleInfo`,
скопировать `LocaleInfo` по полученному указателю, модифицировать по необходимости
поля класса, и передать уже непосредственно в функцию форматирования.


##### Переопределение виртуальных методов класса marty::format::LocaleInfo

Если перезадания значений полей базового класса 
`marty::format::LocaleInfo` недостаточно для реализации собственного класса локали,
можно переопределить виртуальные методы базового класса `marty::format::LocaleInfo`
для реализации необходимой функциональности.

Виртуальные методы класса `marty::format::LocaleInfo`, доступные для переопределения:

```cpp
virtual std::string getGroupSeparator( LocaleInfoSeparatorType vt //!
                                     , NumeralSystem ns       //!
                                     ) const;

//! Обрабатывает LocaleInfoValueType::sign* и LocaleInfoValueType::format*
virtual std::string getLocaleInfoValue(LocaleInfoValueType vt) const;

virtual group_info_t getGroupInfo(NumeralSystem ns, bool bFractionalPart) const;

virtual std::string substFormatString( const std::string &fmt
                                     , const std::string &numStr
                                     , PositiveNumbersMode pm
                                     ) const;

// digitsDecimal / digitsCurrency
virtual unsigned getNumberOfDigits(LocaleInfoDigitsType vt) const;
```


#### Форматная строка для чисел - marty::format::LocaleInfo::format*

!!! Написать






### Обобщённый тип фильтра BasicFormatValueFilter

Функциональный объект - фильтр, для преобразования форматированного значения в целевую строку.

Фильтрация производится после форматирования поля.

```cpp
template<typename InputIteratorType, typename OutputIteratorType>
using BasicFormatValueFilter = std::function< OutputIteratorType( InputIteratorType  // begin
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

```cpp
using FormatValueFilter = BasicFormatValueFilter< marty::utf::UtfInputIterator<char>
                                                , marty::utf::UtfOutputIterator<char>
                                                >;
```


### Мейкер стандартных фильтров по типу StdFilterType - makeStandardFormatValueFilter

Возвращает стандартный фильтр по его StdFilterType-идентификатору.

```cpp
FormatValueFilter makeStandardFormatValueFilter( StdFilterType filterType
                                               , bool *pNoneReturned=0
                                               );
```


|Значение|Описание|
|:-------|:-------|
|**filterType**|Тип фильтра (`marty::format::StdFilterType`).|
|**pNoneReturned**|Опциональный указатель на `bool` переменную, принимающую признак, был ли возвращён `none` фильтр. Если данный указатель задан нулевым, то вместо возвращения `none` фильтра будет выброшено исключение.|



#### marty::format::StdFilterType перечисление

Базовый тип: `std::uint32_t`.

Перечисление для стандартных фильтров.


|Имя|Значение|Описание|
|:-------|:-------|:-------|
|**unknown**, **invalid**|(std::uint32_t)(-1)|Недопустимое/неизвестное значение.|
|**none**|0x00|Фильтрация не производится.|
|**xml**|0x01|XML фильтр для текста тэгов или значений атрибутов.|
|**xmlTag**, **xmlText**|0x02|XML фильтр для текста тэгов.|
|**xmlAttr**|0x03|XML фильтр для значений атрибутов.|
|**html**|0x04|HTML фильтр для текста тэгов или значений атрибутов.|
|**htmlTag**, **htmlText**|0x05|HTML фильтр для текста тэгов.|
|**htmlAttr**|0x06|HTML фильтр для значений атрибутов.|
|**sql**|0x07|Фильтр для значений SQL.|




### Мейкер стандартных фильтров по строке - makeStandardFormatValueFilter

Возвращает стандартный фильтр по его имени.

```cpp
template<typename StringType>
FormatValueFilter makeStandardFormatValueFilter( const StringType &filterName
                                               , bool *pNoneReturned=0
                                               );
```


|Значение|Описание|
|:-------|:-------|
|**filterName**|Имя фильтра (строка).|
|**pNoneReturned**|Опциональный указатель на `bool` переменную, принимающую признак, был ли возвращён `none` фильтр. Если данный указатель задан нулевым, то вместо возвращения `none` фильтра будет выброшено исключение.|



Допустимые фильтры:



|Значение|Описание|
|:-------|:-------|
|`'xml'`|XML фильтр для текста тэгов или значений атрибутов.|
|`'xml-tag'`, `'xml-text'`, `'xml_tag'`, `'xml_text'`, `'xmltag'`, `'xmltext'`|XML фильтр для текста тэгов.|
|`'xml-attr'`, `'xml_attr'`, `'xmlattr'`|XML фильтр для значений атрибутов.|
|`'html'`|HTML фильтр для текста тэгов или значений атрибутов.|
|`'html-tag'`, `'html-text'`, `'html_tag'`, `'html_text'`, `'htmltag'`, `'htmltext'`|HTML фильтр для текста тэгов.|
|`'html-attr'`, `'html_attr'`, `'htmlattr'`|HTML фильтр для значений атрибутов.|
|`'sql'`|Фильтр для значений SQL.|


Значения являются независимыми от регистра символов.



### Фабрика стандартных фильтров - StdFilterFactory

Используется как значение по умолчанию для шаблонного параметра `FilterFactory`
в базовой реализации функции форматирования `formatMessageImpl` и в стандартных 
реализациях `formatMessage`.

```cpp
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
```


### marty::format::FormatArgumentVariant - Variant-тип аргумента

Стандартный тип аргумента на базе `std::variant`.

Это стандартный тип `marty::format::FormatArgumentVariant`. Пользователь библиотеки может определить
свой вариант variant'а и использовать его в своих библиотеках или прикладном коде.

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
                , const void*
                // , std::nullptr_t
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

```cpp
template< typename ArgumentVariantType=FormatArgumentVariant
        , typename VectorType=std::vector<ArgumentVariantType>
        , typename MapType=std::unordered_map<std::string, std::size_t>
        , typename WidthCalculator = utils::DefaultUtfWidthCalculator
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
                      , DefaultUtfWidthCalculator
                      >;
```


### marty::format::formatMessageImpl

Базовая реализация функций форматирования. 
Пользователь библиотеки может создать свой собственный variant-тип аргумента, добавив
свои собственные типы, и, используя данную функцию, сделать свою кастомизированную функцию форматирования.

```cpp
template< typename StringType      = std::string
        , typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
StringType formatMessageImpl( const StringType &fmt
                            , const ArgsType   &args
                            , const LocaleInfo *pLocaleInfo = 0
                            , FormattingFlags   formattingFlags=FormattingFlags::allBase
                            )
```


### marty::format::FormattingFlags флаги

Базовый тип: `std::uint32_t`.

Флаги опций для семейства функций `marty::format::formatMessage`.


|Имя|Значение|Описание|
|:-------|:-------|:-------|
|**unknown**, **invalid**|(std::uint32_t)(-1)|Недопустимое/неизвестное значение.|
|**none**|0x00|Пустое/отсутствующее значение.|
|**ignoreFormatStringErrors**|0x01|Игнорировать ошибки в форматной строке.|
|**ignoreArgumentErrors**|0x02|Игнорировать ошибки поиска аргументов.|
|**ignoreFormattingErrors**|0x04|Игнорировать ошибки при форматировании значений - например, некорректные значения ширины или точности.|
|**ignoreFillIndirectErrors**|0x08|Игнорировать ошибку поиска аргумента, содержащего символ заполнения.|
|**ignoreWidthIndirectErrors**|0x10|Игнорировать ошибку поиска аргумента, содержащего ширину поля.|
|**ignorePrecisionIndirectErrors**|0x20|Игнорировать ошибку поиска аргумента, содержащего точность.|
|**ignoreConversionErrors**|0x40|Игнорировать ошибки спецификатора типа и автоматически конвертировать данные в требуемый тип..|
|**ignoreTypeMismatchErrors**|0x80|Ignore formatting type specification char mismatch errors.|
|**ignoreNotFilterErrors**|0x100|Игнорировать ошибки - попытка использовать как фильтр значения других (примитивных) типов.|
|**considerZeroWidthSpaces**|0x200|Consider zero-width spaces.|
|**considerCombiningChars**|0x400|Consider combining Unicode chars.|
|**fractionalGroupping**|0x800|Группировка цифр также и в дробной части числа, если разделитель разрядов задан для целой части числа. Без указания данного флага для дробной части числа следует явно указывать спецификатор разделителя разрядов.|
|**ignoreOptionsIndirectErrors**|ignoreFillIndirectErrors \| ignoreWidthIndirectErrors \| ignorePrecisionIndirectErrors|Игнорировать ошибки косвенного задания опций форматирования. Групповое значение для всех типов ошибок (`ignoreFillIndirectErrors \| ignoreWidthIndirectErrors \| ignorePrecisionIndirectErrors`).|
|**ignoreErrors**|ignoreFormatStringErrors \| ignoreArgumentErrors \| ignoreFormattingErrors \| ignoreOptionsIndirectErrors \| ignoreConversionErrors \| ignoreNotFilterErrors \| ignoreTypeMismatchErrors|Игнорировать все ошибки. Групповое значение для всех типов ошибок (`ignoreFormatStringErrors \| ignoreArgumentErrors \| ignoreFormattingErrors \| ignoreOptionsIndirectErrors \| ignoreConversionErrors`).|
|**considerUnicodeFeatures**|considerZeroWidthSpaces  \| considerCombiningChars|Consider options.|
|**allBase**|ignoreErrors \| considerUnicodeFeatures|Все флаги установлены, кроме флага группировки разрядов дробной части.|
|**all**|allBase \| fractionalGroupping|Все флаги установлены (`ignoreErrors \| fractionalGroupping`).|
|**localeForceCustom**|0x1000|Принудительно использовать переданную локаль.|
|**localeUseSystem**|0x2000|Использовать локаль системы вместо локали текущего пользователя.|




### marty::format::formatMessage - аргументы передаются в generic-контейнере

```cpp
template< typename StringType      = std::string
        , typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
StringType formatMessage( const StringType &fmt
                        , const ArgsType   &args
                        , const LocaleInfo *pLocaleInfo = 0
                        , FormattingFlags  formattingFlags=FormattingFlags::allBase
                        )
```

```cpp
template< typename ArgsType        = Args
        , typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
std::string formatMessage( const char       *fmt
                         , const ArgsType   &args
                         , const LocaleInfo *pLocaleInfo = 0
                         , FormattingFlags  formattingFlags=FormattingFlags::allBase
                         )
```


### marty::format::formatMessage - аргументы передаются в виде std::initializer_list

```cpp
using FormatArgumentVariantList = std::initializer_list<FormatArgumentVariant>;

template< typename StringType = std::string
        , typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
StringType formatMessage( const StringType          &fmt
                        , FormatArgumentVariantList &&args
                        , const LocaleInfo          *pLocaleInfo = 0
                        , FormattingFlags           formattingFlags=FormattingFlags::allBase
                        )
```

```cpp
template< typename WidthCalculator = DefaultUtfWidthCalculator
        , typename FilterFactory   = StdFilterFactory
        >
inline
std::string formatMessage( const char                *fmt
                         , FormatArgumentVariantList &&args
                         , const LocaleInfo          *pLocaleInfo = 0
                         , FormattingFlags           formattingFlags=FormattingFlags::allBase
                         )
```


### Базовая реализация функтора вычисления отображаемой ширины строки

Базовая реализация функтора вычисления отображаемой ширины строки использует функции библиотеки
`marty::utf` с префиксом `suf` - `simpleUnicodeFeature`. В текущий момент поддерживается
детект пробелов нулевой ширины по кодам символов и детект комбинируемых диакретиков (по диапазонам символов).

```cpp
struct DefaultUtfWidthCalculator
{

    std::size_t operator()(marty::utf::unicode_char_t ch) const
    {
        // suf - simpleUnicodeFeature
        if (marty::utf::sufIsZeroWidthSpace(ch) || marty::utf::sufIsCombiningDiacretic(ch))
            return 0;

        return 1;
    }

    std::size_t operator()(const char* b, const char* e) const
    {
        auto it    = marty::utf::UtfInputIterator<char>(b, e);
        auto endIt = marty::utf::UtfInputIterator<char>();

        std::size_t size = 0;

        for(; it!=endIt; ++it)
        {
            size += operator()(*it);
        }

        return size;
    }

    std::size_t operator()(const char* b, std::size_t strLen) const
    {
        return operator()(b, b+std::ptrdiff_t(strLen));
    }

}; // struct DefaultUtfWidthCalculator
```

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


|Значение|Описание|
|:-------|:-------|
|**arg_id**|Идентификатор аргумента. Может отсутствовать, тогда используется автоматически подставляемый индекс, который инкрементируется при появлении каждого "поля замены" с отсутствующим идентификатором аргумента.|
|**arg_index**|Индекс аргумента, начиная с нуля. Для ассоциативных контейнеров типа `std::map` и `std::unordered_map` обращение по индексу производится с использованием `std::advance`. Индекс аргумента должен быть корректным десятичным числом.|
|**arg_name**|Имя аргумента, допустимы любые символы, кроме символов `:`, `{` и `}`. В зависимости от типа контейнера, в котором передаются аргументы, именованное обращение к аргументам может не поддерживаться. Так, для контейнеров типа `std::vector` или `std::initializer_list` использование именованного аргумента приведёт к выбросу исключения, если это не запрещено режимом игнорирования ошибок.|
|**identifier**|Идентификатор - начинается с `identifier_char`, последующие символы могут быть `identifier_char`, цифры, или символ `'.'`.|
|**identifier_char**|символ идентификатора - символ подчеркивания `'_'`, строчная или прописная латинская буква.|
|**conversion**|преобразование типа аргумента, см. далее "Преобразование типа аргумента".|
|**format_spec**|спецификатор формата, см. далее.|


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

**NOT_IMPLEMENTED** - в данный момент не реализовано, но обрабатывается в форматной строке.


|Значение|Описание|
|:-------|:-------|
|`s`|преобразует аргумент в строку (см. описание [str](https://docs.python.org/3/library/stdtypes.html#str)).|
|`r`|преобразует аргумент в строку (см. описание [repr](https://docs.python.org/3/library/functions.html#repr)).|
|`a`|преобразует аргумент в строку (см. описание [ascii](https://docs.python.org/3/library/functions.html#ascii)).|



### Спецификатор формата - format_spec


<pre>
format_spec     ::= [options][width][grouping][&quot;.&quot; precision][frac_grouping][&quot;L&quot;][type][filters]
options         ::= [[fill]align][sign][&quot;z&quot;][&quot;~&quot;][&quot;!&quot;][&quot;#&quot;][&quot;0&quot;]
fill            ::= &lt;any character&gt; | &quot;{&quot; argId &quot;}&quot;
align           ::= &quot;&lt;&quot; | &quot;&gt;&quot; | &quot;^&quot; | &quot;=&quot;
sign            ::= &quot;+&quot; | &quot;-&quot; | &quot; &quot;
width           ::= digit+ | &quot;{&quot; argId &quot;}&quot;
grouping        ::= &quot;,&quot; | &quot;_&quot; | &quot;&apos;&quot;
frac_grouping   ::= &quot;,&quot; | &quot;_&quot; | &quot;&apos;&quot;
precision       ::= digit+ | &quot;{&quot; argId &quot;}&quot;
type            ::= &quot;a&quot; | &quot;A&quot; | &quot;b&quot; | &quot;B&quot; | &quot;c&quot; | &quot;d&quot; | &quot;e&quot; | &quot;E&quot; | &quot;f&quot; | &quot;F&quot; | &quot;g&quot; | &quot;G&quot; | &quot;h&quot; | &quot;H&quot; |
                    &quot;n&quot; | &quot;o&quot; | &quot;p&quot; | &quot;s&quot; | &quot;S&quot; | &quot;t&quot; | &quot;T&quot; | &quot;x&quot; | &quot;X&quot; | &quot;y&quot; | &quot;Y&quot; | &quot;%&quot; | &quot;$&quot;
filters         ::= filter+
filter          ::= &quot;|&quot; argId
arg_id          ::= arg_index | arg_name
arg_index       ::= digit+
arg_name        ::= identifier
identifier      ::= identifier_char [identifier_char | digit | &quot;.&quot;]+
identifier_char ::= &quot;_&quot; | &quot;a&quot;-&quot;z&quot; | &quot;A&quot;-&quot;Z&quot;
</pre>

<!-- -->

Описание полей спецификатора формата:


|Значение|Описание|
|:-------|:-------|
|**format_spec**|полный спецификатор формата.|
|**options**|опции форматирования в составе спецификатора формата.|
|**fill**, **align**|заполнение и выравнивание. Символ заполнения может быть любым символом, за исключением символов, используемых в форматной строке. Символ заполнения может быть не задан, тогда будет использоваться символ пробела `space` ([U+0020](https://unicode-explorer.com/c/0020)).|
|**sign**|знак. Данный параметр действителен только для числовых типов, для остальных типов игнорируется.|
|**z**|обрабатывается в форматной строке, но игнорируется (**IGNORED**).|
|**~**|Битовый каст знаковых целых. По умолчанию, знаковые числа отображаются, как значение его модуля без знака или со знаком, в зависимости от `'sign'`, при этом отрицательные числа всегда отображаются со знаком. Иногда требуется отобразить битовое представление числа. Можно при передаче аргумента явно привести его к безнаковому, но иногда проще задать в форматной строке единственную опцию битового каста. Данная опция работает с форматными символами `'b'`, `'B'`, `'x'`, `'X'`, `'h'` и `'H'`.|
|**!**|Инверсия регистра. Если задаётся альтернативная форма опцией `'#'`, то к двоичным и шестнадцатиричным числам со спецификаторами типа формата `'b'`, `'B'`, `'x'` и `'X'` добавляется префикс `'0b'`, `'0B'`, `'0x'` или `'0X'`. Так, шестнадцатеричные числа будут отображаться как `'0xdeadbeaf'` или `'0XDEADBEAF'` (для спецификаторов типа `'x'` и `'X'`, соответственно). Такая запись читается плохо - префикс сливается с остальным числом. Для того, чтобы префикс был хорошо заметен, его регистр обычно делается отличным от регистра самого числа - `'0xDEADBEAF'`. Маркер инверсии регистра предназначен для изменения регистра префикса по сравнению с регистром числа. Так, форматная строка `'!#X'` выведет `'0xDEADBEAF'`, а `'!#x'` - `'0Xdeadbeaf'`.<br/><br/> Для систем счисления, отличных от двоичной и шестнадцатеричной, а также для спецификаторов типа формата, отличных от `'b'`, `'B'`, `'x'`, `'X'`, `'h'` и `'H'`, данная опция игнорируется.|
|**#**|Задаёт использовать альтернативную форму при форматировании. Альтернативная форма определяется по-разному для разных типов. Этот параметр действителен только для целых чисел и чисел с плавающий точкой. Для целых чисел, когда используется двоичный, восьмеричный или шестнадцатеричный вывод, этот параметр добавляет соответствующий префикс '0b', '0o', '0x' или '0X' к выходному значению. Для десятичных чисел параметр игнорируется. Для чисел с плавающий точкой альтернативная форма заставляет результат преобразования всегда содержать символ десятичной точки, даже если за ним не следует ни одна цифра. Обычно символ десятичной точки появляется в результате этих преобразований, только если за ним следует цифра. Кроме того, для преобразований 'g' и 'G' конечные нули не удаляются из результата.|
|**0**|Задаёт заполение нулями до нужной ширины поля (между знаком числа и его значением). Также устанавливает выравниваение в `'='`, если выравнивание не задано. Если выравнивание задано и не равно `'='`, то `'0'` игнорируется.|
|**width**|Ширина — это десятичное целое число, определяющее минимальную общую ширину поля, включая любые префиксы, разделители и другие символы форматирования. Если не указано, то ширина поля будет определяться содержимым.|
|**grouping**|Параметр группировки после поля ширины указывает разделитель групп цифр для целой части числа. Для десятичных чисел разделение разрядов производится по три цифры. Для спецификаторов `'b'`, `'o'`, `'x'`, and `'X'` разделение производится на группы по четыре цифры. Разделитель групп разрядов может быть одним из следующих: `','`, `'_'`, `'''` (apos).<br/><br/> Данный параметр не используется для форматирования в библиотеке языка `C++`.<br/><br/> В языке `Python` символ `'''` (apos) не используется. Также, в языке `Python` разделители `','` и `'_'` используются при отображении десятичных чисел и отображении `'b'`, `'o'`, `'x'`, and `'X'` чисел соответственно. Данная библиотека позволяет использовать все допустимые разделители для любых чисел.<br/><br/> При использовании локали для разделения разрядов используется символ (строка), заданная в локали.|
|**precision**|Точность - десятичное целое число, указывающее, сколько цифр должно отображаться после десятичной точки для типов представления 'f' и 'F', или до и после десятичной точки для типов представления 'g' или 'G'. Для строковых типов представления поле указывает максимальный размер поля — другими словами, сколько символов будет использовано из содержимого поля. Точность не допускается для целочисленных типов представления.|
|**frac_grouping**|Группировка цифр числа в дробной части (fractional part groupping). По умолчанию, если при вызове функции форматирования не задан флаг форматирования `marty::format::FormattingFlags::fractionalGroupping`, группировка цифр в дробной части чисел с плавающей точкой не производится.<br/><br/> При использовании локали для разделения разрядов используется символ (строка), заданна яв локали.|
|**L**|Locale-зависимое форматирование. Аналогично спецификатору типа `'n'`.|
|**type**|спецификатор типа. Определяет, как должны быть отображены данные.|
|**filters**|последовательность фильтров, до восьми в цепочке.|
|**filter**|идентификатор аргумента, предварённый символом пайпа `'\|'` (broken vertical bar), если в аргументах задан пользовательский фильтр, или один из встроенных фильтров. При поиске фильтра сначала проверяются встроенные фильтры.|
|**arg_id**, **arg_index**, **arg_name**, **identifier**, **identifier_char**|См. предыдущий раздел "Грамматика полей замены"|



#### Символ заполнения fill

В качестве символов заполнения (`fill`) запрещены символы `'a'`-`'z'` и `'A'`-`'Z'`, а также цифровые символы `'0'`-`'9'`.

Прочие запрещённые символы:


|Запрещённый символ|Описание/причина запрещения использования|
|-------|-------|
|`'<'`|Выравнивание по левому краю.|
|`'>'`|Выравнивание по правому краю.|
|`'^'`|Выравнивание по середине.|
|`'='`|Выравнивание чисел по ширине.|
|`'+'`|Символ знака `'+'` для отображения числовых значений.|
|`'-'`|Символ знака `'-'` для отображения числовых значений.|
|`space`|Символ `' '` (`space`/`пробел`) для отображения числовых значений. Символ `' '` (`space`/`пробел`) для выравнивания используется по умолчанию, явное указание данного символа не требуется.|
|`'''`|Символ группировки разрядов чисел - апостроф (`apos`).|
|`'_'`|Символ группировки разрядов чисел - подчеркивание (`underscore`).|
|`','`|Символ группировки разрядов чисел запятая (`','`).|
|`'\\|'`|Символ последовательности фильтров  (`pipe`/`broken vertical bar`/`BVB`), используется при задании последовательности фильтров.|
|`'%'`|Символ вывода числа в виде процентов.|
|`'$'`|Символ вывода числа в виде валюты.|
|`'?'`|Символ вывода строки в виде `escape`-последовательности.|
|`'!'`|Символ инверсии регистра.|
|`'#'`|Символ `альтернативного` режима.|
|`'~'`|Символ битового каста.|


В текущий момент допустимыми `ASCII`-символами заполнения являются: 
а так же символы 
`'&'`,
`'*'`,
`';'`,
`'@'`
.
Также можно использовать любые `UNICODE`-символы.


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
|`'-'`|Указывает, что знак следует использовать только для отрицательных чисел. Для положительных чисел не производится вставка знака `'+'` или пробела. Данное поведение используется по умолчанию.|
|`space`|Указывает, что для положительных чисел следует использовать пробел, а для отрицательных чисел — знак `'-'`.|


**Примечание** Тут надо написать про конвертацию - есть, нет, что-то ещё.

### Поддерживаемые спецификаторы типа

Строкой `Py` обозначаются спецификаторы типа из языка `Python`, строкой `C++` - спецификаторы типа из языка `C++`, соответственно, спецификаторы, которые 
поддерживаются обоими языками, будут обозначены как `Py`/`C++`.

Спецификаторы, не поддерживаемые ни языком `Python`, ни языком `C++`, обозначаются как `-`.


#### Спецификаторы представления строк


|Спецификатор|Язык|Описание|
|:-------|:-------|:-------|
|`none`|`Py`/`C++`|Аналогично `'s'`.|
|`'s'`|`Py`/`C++`|Выводит строку.|
|`'?'`|`C++`|Выводит экранированную строку, см. далее "Экранирование строк".|





#### Спецификаторы представления типа bool

При форматировании значений типа `bool` в строковом представлении
форматирование осуществляется в соответствии с правилами форматирования строк.

При форматировании значений типа `bool` в целочисленном представлении
форматирование осуществляется в соответствии с правилами форматирования целых чисел.


##### Опции форматирования, влияющие на отображение bool в строковом виде


|Значение|Описание|
|:-------|:-------|
|`'!'`|Инверсия регистра - меняет регистр первого символа строкового представления булевой переменной на противоположный: 'True', 'fALSE'.|
|`'#'`|Альтернативное отображение - отображение `bool` в виде одного символа, соответствует заданию точности в 1 символ.|



##### Спецификаторы


|Спецификатор|Язык|Описание|
|:-------|:-------|:-------|
|`none`|`C++`|Аналогично `'s'`.|
|`'s'`|`C++`|Отображает значение в виде строки `'true'`/`'false'` в нижнем регистре.|
|`'S'`|`-`|Отображает значение в виде строки `'TRUE'`/`'FALSE'` в верхнем регистре.|
|`'t'`|`-`|Для булевых значений работает аналогично `'s'`. Для целочисленных значений принудительно отображает целое число как `bool` в формате `'s'`.|
|`'T'`|`-`|Для булевых значений работает аналогично `'S'`. Для целочисленных значений принудительно отображает целое число как `bool` в формате `'S'`.|
|`'y'`|`-`|Для булевых значений работает аналогично `'s'`, но используются строковые значения `'yes'`/`'no'`. Для целочисленных значений принудительно отображает целое число как `bool` в формате `'y'`.|
|`'Y'`|`-`|Для булевых значений работает аналогично `'S'`, но используются строковые значения `'YES'`/`'NO'`. Для целочисленных значений принудительно отображает целое число как `bool` в формате `'Y'`.|



##### Спецификаторы целых чисел для представления типа bool

Для отображения `bool` значений в виде целого числа можно использовать любой и спецификаторов
форматирования целых чисел, при этом целочисленное значение `0` будет трактоваться, как `false`,
а любое другое значение будет трактоваться, как `true`.



#### Спецификаторы представления целочисленных типов


|Спецификатор|Язык|Описание|
|:-------|:-------|:-------|
|`none`|`Py`/`C++`|Аналогично `'d'`.|
|`'b'`|`Py`/`C++`|Форматирует число в двоичном виде. Префикс основания системы счисления - `'0b'` и `'0B'` (с инверсией регистра).|
|`'B'`|`C++`|Форматирует число в двоичном виде. Префикс основания системы счисления - `'0B'` и `'0b'` (с инверсией регистра).|
|`'c'`|`Py`/`C++`|Символ. Отображает целое число в виде символа.|
|`'d'`|`Py`/`C++`|Десятичное число (в обычной нотации).|
|`'o'`|`Py`/`C++`|Форматирует число в восьмеричном виде. Префикс основания системы счисления - `'0'`.|
|`'x'`|`Py`/`C++`|Форматирует число в шестнадцатеричном виде. Префикс основания системы счисления - `'0x'` или `'0X'` (с инверсией регистра).|
|`'X'`|`Py`/`C++`|Форматирует число в шестнадцатеричном виде. Префикс основания системы счисления - `'0X'` или `'0x'` (с инверсией регистра).|
|`'h'`|`Py`/`C++`|Форматирует число в шестнадцатеричном виде. Для данного формата в альтернативной форме к отформатированномому числу добавляется суффикс `'h'` или `'H'` (с инверсией регистра).|
|`'H'`|`Py`/`C++`|Форматирует число в шестнадцатеричном виде. Для данного формата в альтернативной форме к отформатированномому числу добавляется суффикс `'H'` или `'h'` (с инверсией регистра).|
|`'n'`|`Py`|Десятичное число. Аналогично форматному символу `'d'` с `C++` опцией `'L'`. Форматирует число с использованием символа разделителя разрядов, заданного системной локалью.|
|`'t'`|`-`|Принудительно отображает целое число как `bool` в формате `'s'`, при этом значение `0` будет трактоваться, как `false`, а любое другое значение будет трактоваться, как `true`.|
|`'T'`|`-`|Принудительно отображает целое число как `bool` в формате `'S'`, при этом значение `0` будет трактоваться, как `false`, а любое другое значение будет трактоваться, как `true`.|
|`'y'`|`-`|Принудительно отображает целое число как `bool` в формате `'y'`, при этом значение `0` будет трактоваться, как `false`, а любое другое значение будет трактоваться, как `true`.|
|`'Y'`|`-`|Принудительно отображает целое число как `bool` в формате `'Y'`, при этом значение `0` будет трактоваться, как `false`, а любое другое значение будет трактоваться, как `true`.|
|`'p'`|`C++`|Форматирует целое число как указатель в шестнадцатиричном виде, как с использованием спецификатора `'x'`, ширина поля вычисляется автоматически, в зависимости от размера целого. При этом число форматируется как указатель, используя размер целого типа, а не размер указателя на текущей платформе.|
|`'P'`|`C++`|Форматирует целое число как указатель в шестнадцатиричном виде, как с использованием спецификатора `'X'`, ширина поля вычисляется автоматически, в зависимости от размера целого. При этом число форматируется как указатель, используя размер целого типа, а не размер указателя на текущей платформе.|
|`'%'`|`Py`|Производит вывод в формате процентов, как для числа с плавающей точкой, но умножения на 100 не производится (предполагается, что оно уже было сделано).|
|`'$'`|`-`|Производит вывод в формате валюты. Для обозначения валюты используется символ `'$'` или локальный символ валюты.|




#### Спецификаторы представления типов с плавающей точкой


|Спецификатор|Язык|Описание|
|:-------|:-------|:-------|
|`none`|`Py`/`C++`|Аналогично `'f'`.|
|`'a'`|`C++`|Шестнадцатиричное представление чисел с плавающей точкой.|
|`'A'`|`C++`|Шестнадцатиричное представление чисел с плавающей точкой.|
|`'d'`|`Py`|Десятичное число (аналогично `'f'`)|
|`'e'`|`Py`/`C++`|Научная запись. Для заданной точности `precision` форматирует число в научной записи с буквой `'e'`, разделяющей коэффициент от показателя степени. Коэффициент имеет одну цифру до и `precision` цифр после десятичной точки, всего `precision` + 1 значащих цифр. Если точность не указана, использует точность 6 цифр после десятичной точки для float и показывает все цифры коэффициента для Decimal. Если `precision`=0, десятичная точка опускается, если не используется опция `'#'`.|
|`'E'`|`Py`/`C++`|Научная запись. То же, что и `'e'`, но в качестве разделительного символа используется заглавная `'E'`.|
|`'f'`|`Py`/`C++`|Нотация с фиксированной точкой. Для заданной точности p форматирует число как десятичное число с ровно `precision` цифрами после десятичной точки. Если точность не указана, использует точность 6 цифр после десятичной точки для float и использует точность, достаточную для отображения всех цифр коэффициентов для Decimal. Если p=0, десятичная точка опускается, если не используется опция `'#'`.|
|`'F'`|`Py`/`C++`|Нотация с фиксированной точкой. То же, что и `'f'`, но преобразует `nan` в `'NAN'` и `inf` в `'INF'`.|
|`'g'`|`Py`/`C++`|Общий формат. Для заданной точности p >= 1 это округляет число до p значащих цифр, а затем форматирует результат либо в формате с фиксированной точкой, либо в экспоненциальном формате, в зависимости от его величины. Точность 0 рассматривается как эквивалент точности 1.<br/><br/> Точные правила следующие: предположим, что результат, отформатированный с типом представления 'e' и точностью p-1, будет иметь показатель степени exp. Тогда, если m <= exp < p, где m равно -4 для чисел с плавающей точкой и -6 для десятичных чисел, число форматируется с типом представления 'f' и точностью p-1-exp. В противном случае число форматируется с типом представления 'e' и точностью p-1. В обоих случаях незначащие конечные нули удаляются из значащей части, а десятичная точка также удаляется, если после нее не осталось цифр, если только не используется опция '#'.<br/><br/> Если точность не указана, используется точность 6 значащих цифр для чисел с плавающей точкой. Для десятичных чисел коэффициент результата формируется из цифр коэффициента значения; для значений, меньших 1e-6 по абсолютной величине, и значений, где разрядное значение младшей значащей цифры больше 1, используется экспоненциальное обозначение, в противном случае используется обозначение с фиксированной точкой.<br/><br/> Положительная и отрицательная бесконечность, положительный и отрицательный ноль и nan форматируются как inf, -inf, 0, -0 и nan соответственно, независимо от точности.|
|`'G'`|`Py`/`C++`|Общий формат. То же, что и «g», за исключением переключения на «E», если число становится слишком большим. Представления бесконечности и NaN также записываются заглавными буквами.|
|`'n'`|`Py`|Десятичное число. Аналогично форматному символу `'d'` с `C++` опцией `'L'`. Форматирует число с использованием символа разделителя разрядов, заданного системной локалью.|
|`'%'`|`Py`|Производит вывод в формате процентов, умножая исходное число на `100.0`.|
|`'$'`|`-`|Производит вывод в формате валюты. Для обозначения валюты используется символ `'$'` или локальный символ валюты.|



**Примечание**. Десятичные числа с произвольной размерностью `marty::Decimal`
в режиме форматирования обычного числа с плавающей точкой (`'f'`/`'F'`) форматируются как есть,
в других режимах форматирование производится стандартными средствами `C++` через преобразование
в `long double`.


#### Спецификаторы представления указателей

Для отображения указателя его надо преобразовать к типу `void*` при передаче в функцию форматирования.


|Спецификатор|Язык|Описание|
|:-------|:-------|:-------|
|`none`|По умолчанию указатели форматируются со спецификатором `'P'`.||
|`'p'`|`C++`|Форматирует указатель в шестнадцатиричном виде, как с использованием спецификатора `'x'`, ширина поля вычисляется автоматически.|
|`'P'`|`C++`|Форматирует указатель в шестнадцатиричном виде, как с использованием  спецификатора `'X'`, ширина поля вычисляется автоматически.|






### Встроенные фильтры (поле спецификатора формата filter)



|Значение|Описание|
|:-------|:-------|
|`'xml'`|XML фильтр для текста тэгов или значений атрибутов.|
|`'xml-tag'`, `'xml-text'`, `'xml_tag'`, `'xml_text'`, `'xmltag'`, `'xmltext'`|XML фильтр для текста тэгов.|
|`'xml-attr'`, `'xml_attr'`, `'xmlattr'`|XML фильтр для значений атрибутов.|
|`'html'`|HTML фильтр для текста тэгов или значений атрибутов.|
|`'html-tag'`, `'html-text'`, `'html_tag'`, `'html_text'`, `'htmltag'`, `'htmltext'`|HTML фильтр для текста тэгов.|
|`'html-attr'`, `'html_attr'`, `'htmlattr'`|HTML фильтр для значений атрибутов.|
|`'sql'`|Фильтр для значений SQL.|


Значения являются независимыми от регистра символов.




### Экранирование строк

**NOT_IMPLEMENTED** - в данный момент не реализовано, но обрабатывается в форматной строке.

















