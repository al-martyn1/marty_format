// Форматирование в 2ую СС
cout << formatMessage("|{:0b}|",    {-420 }) << "\n"; // |-00000000000000000000000110100100|
cout << formatMessage("|{:0'b}|",   {-420 }) << "\n"; // |-0000'0000'0000'0000'0000'0001'1010'0100|
cout << formatMessage("|{:12b}|",   {-420 }) << "\n"; // |-110100100|
cout << formatMessage("|{:12'b}|",  {-420 }) << "\n"; // |-1'1010'0100|
cout << formatMessage("|{:#0b}|",   {-420 }) << "\n"; // |-0b00000000000000000000000110100100|
cout << formatMessage("|{:~0'b}|",  {-1420}) << "\n"; // |1111'1111'1111'1111'1111'1010'0111'0100|
cout << formatMessage("|{:#0'b}|",  {-420 }) << "\n"; // |-0b0000'0000'0000'0000'0000'0001'1010'0100|
cout << formatMessage("|{:#12b}|",  {-420 }) << "\n"; // |-0b110100100|
cout << formatMessage("|{:#12'b}|", {-420 }) << "\n"; // |-0b1'1010'0100|

// Фрматирование в 16ую СС
cout << formatMessage("|{:0x}|",        { -420  }) << "\n"; // |-000001a4|
cout << formatMessage("|{:0X}|",        { -420  }) << "\n"; // |-000001A4|
cout << formatMessage("|{:0'x}|",       { -420  }) << "\n"; // |-0000'01a4|
cout << formatMessage("|{:12X}|",       { -420  }) << "\n"; // |-1A4|
cout << formatMessage("|{:#0h}|",       { -420  }) << "\n"; // |-000001a4h|
cout << formatMessage("|{:#12x}|",      { -420  }) << "\n"; // |-0x1a4|
cout << formatMessage("|{:!#0X}|",      { -420  }) << "\n"; // |-0x000001A4|
cout << formatMessage("|{:~!#0X}|",     { -420  }) << "\n"; // |0xFFFFFE5C|
cout << formatMessage("|{:~!#0'X}|",    { -420  }) << "\n"; // |0xFFFF'FE5C|
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
cout << formatMessage("|{:=12}|",       { -420  }) << "\n"; // |-        420|
cout << formatMessage("|{:=12b}|",      { -420  }) << "\n"; // |-  110100100|
cout << formatMessage("|{:=12x}|",      { -420  }) << "\n"; // |-        1a4|
cout << formatMessage("|{:=12X}|",      { -420  }) << "\n"; // |-        1A4|
cout << formatMessage("|{:=12%}|",      { -420  }) << "\n"; // |-       420%|
cout << formatMessage("|{:*=12}|",      { -420  }) << "\n"; // |-********420|
cout << formatMessage("|{:*=12b}|",     { -420  }) << "\n"; // |-**110100100|
cout << formatMessage("|{:*=+12b}|",    { 420   }) << "\n"; // |+**110100100|
cout << formatMessage("|{:*=+!#12X}|",  { 1420  }) << "\n"; // |+******0x58C|
cout << formatMessage("|{:= ~!#012X}|", { -1420 }) << "\n"; // | 0x0FFFFFA74|
