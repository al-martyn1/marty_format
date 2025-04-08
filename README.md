- [Библиотека форматирования](#user-content-библиотека-форматирования)
  - [Синтаксис форматной строки](#user-content-синтаксис-форматной-строки)

# Библиотека форматирования

## Синтаксис форматной строки



Форматная строка содержит "поля замены", укруженные фигурными скобками ("{}").
Все, что не заключено в фигурные скобки, считается текстом, который копируется в вывод без изменений.
Если вам нужно включить символ скобки в текст, его можно экранировать удвоением: "{{" и "}}".

*Примечание*. В зависимости от режима игнорирования ошибок, закрывающая фигурная скобка может быть
единственной.


Грамматика для полей замены выглядит так:

<pre>
*replacement_field* ::= &quot;{&quot; [field_name] [&quot;!&quot; conversion] [&quot;:&quot; format_spec] &quot;}&quot;
*field_name*        ::= arg_name (&quot;.&quot; attribute_name | &quot;[&quot; element_index &quot;]&quot;)*
*arg_name*          ::= [identifier | digit+]
*attribute_name*    ::= identifier
*element_index*     ::= digit+ | index_string
*index_string*      ::= &lt;any source character except &quot;]&quot;&gt; +
*conversion*        ::= &quot;r&quot; | &quot;s&quot; | &quot;a&quot;
*format_spec*       ::= format-spec:format_spec
</pre>

<!-- -->




