# Библиотека форматирования marty::format

- [Библиотека форматирования marty::format](#user-content-библиотека-форматирования-martyformat)
  - [Возможности библиотеки](#user-content-возможности-библиотеки)
  - [Ссылки на референсные спецификации](#user-content-ссылки-на-референсные-спецификации)
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

1. Совместимость форматной строки с форматной строкой языков `Python` и `C++`. 
   Некоторые возможности этих языков игнорируются, но не вызывают ошибки при разборе
   форматной строки.
2. Поддержка именованных аргументов для `C++` при использовании соответствующих контейнеров для аргументов.
3. Поддержка передачи аргументов через std::initializer_list.
4. Поддержка Unicode в кодировке UTF-8.


## Ссылки на референсные спецификации

- `Python`: Описание форматной строки - Format String Syntax - https://docs.python.org/3/library/string.html#format-string-syntax
- `C++`: `std::format` - https://en.cppreference.com/w/cpp/utility/format/format
- `C++`: `Standard format specification` - https://en.cppreference.com/w/cpp/utility/format/spec


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
|`'='`|Задаёт вставлять заполнение после знака числа, но перед цифрами. Позволяет печатать числа в виде  `'+      120'`. Работает только для числовых типов. Если значению ширины поля (`width`) предшествует символ `'0'`, то данный тип выравнивания|
|`'^'`|Задаёт выравнивание по центру.|

Обратите внимание, что если не определена минимальная ширина поля (`idth`), ширина поля всегда будет 
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


#### Спецификаторы представления строк

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



