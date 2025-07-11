
#include <cassert>
#include <string>
#include <iostream>
#include <array>


#include "marty_format/marty_format.h"


using std::cout;
using namespace marty::format;

int main()
{
    cout << formatMessage("|{:0}|",         { -1420 }) << "\n"; // |-1420|
    cout << formatMessage("|{:0b}|",        { -1420 }) << "\n"; // |-00000000000000000000010110001100|
    cout << formatMessage("|{:0x}|",        { -1420 }) << "\n"; // |-0000058c|
    cout << formatMessage("|{:0X}|",        { -1420 }) << "\n"; // |-0000058C|
    cout << formatMessage("|{:0%}|",        { -1420 }) << "\n"; // |-1420%|
    cout << formatMessage("|{:0'}|",        { -1420 }) << "\n"; // |-1'420|
    cout << formatMessage("|{:0'b}|",       { -1420 }) << "\n"; // |-0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:0'x}|",       { -1420 }) << "\n"; // |-0000'058c|
    cout << formatMessage("|{:0'X}|",       { -1420 }) << "\n"; // |-0000'058C|
    cout << formatMessage("|{:0'%}|",       { -1420 }) << "\n"; // |-1'420%|
    cout << formatMessage("|{:0'n}|",       { -1420 }) << "\n"; // |-1 420|
    cout << formatMessage("|{:12}|",        { -1420 }) << "\n"; // |-1420       |
    cout << formatMessage("|{:12b}|",       { -1420 }) << "\n"; // |-10110001100|
    cout << formatMessage("|{:12x}|",       { -1420 }) << "\n"; // |-58c        |
    cout << formatMessage("|{:12X}|",       { -1420 }) << "\n"; // |-58C        |
    cout << formatMessage("|{:12%}|",       { -1420 }) << "\n"; // |-1420%      |
    cout << formatMessage("|{:12'}|",       { -1420 }) << "\n"; // |-1'420      |
    cout << formatMessage("|{:12'b}|",      { -1420 }) << "\n"; // |-101'1000'1100|
    cout << formatMessage("|{:12'%}|",      { -1420 }) << "\n"; // |-1'420%     |
    cout << formatMessage("|{:12'n}|",      { -1420 }) << "\n"; // |-1 420      |
    cout << formatMessage("|{:#0b}|",       { -1420 }) << "\n"; // |-0b00000000000000000000010110001100|
    cout << formatMessage("|{:#0x}|",       { -1420 }) << "\n"; // |-0x0000058c|
    cout << formatMessage("|{:#0X}|",       { -1420 }) << "\n"; // |-0X0000058C|
    cout << formatMessage("|{:#0h}|",       { -1420 }) << "\n"; // |-0000058ch|
    cout << formatMessage("|{:#0'b}|",      { -1420 }) << "\n"; // |-0b0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:#0'x}|",      { -1420 }) << "\n"; // |-0x0000'058c|
    cout << formatMessage("|{:#0'X}|",      { -1420 }) << "\n"; // |-0X0000'058C|
    cout << formatMessage("|{:#0'h}|",      { -1420 }) << "\n"; // |-0000'058ch|
    cout << formatMessage("|{:#12b}|",      { -1420 }) << "\n"; // |-0b10110001100|
    cout << formatMessage("|{:#12x}|",      { -1420 }) << "\n"; // |-0x58c      |
    cout << formatMessage("|{:#12X}|",      { -1420 }) << "\n"; // |-0X58C      |
    cout << formatMessage("|{:#12h}|",      { -1420 }) << "\n"; // |-58ch       |
    cout << formatMessage("|{:#12'b}|",     { -1420 }) << "\n"; // |-0b101'1000'1100|
    cout << formatMessage("|{:012x}|",      { -1420 }) << "\n"; // |-58c|
    cout << formatMessage("|{:012X}|",      { -1420 }) << "\n"; // |-58C|
    cout << formatMessage("|{:#012x}|",     { -1420 }) << "\n"; // |-0x58c|
    cout << formatMessage("|{:#012X}|",     { -1420 }) << "\n"; // |-0X58C|
    cout << formatMessage("|{:#012h}|",     { -1420 }) << "\n"; // |-58ch|
    cout << formatMessage("|{:!#0b}|",      { -1420 }) << "\n"; // |-0B00000000000000000000010110001100|
    cout << formatMessage("|{:!#0x}|",      { -1420 }) << "\n"; // |-0X0000058c|
    cout << formatMessage("|{:!#0X}|",      { -1420 }) << "\n"; // |-0x0000058C|
    cout << formatMessage("|{:!#0h}|",      { -1420 }) << "\n"; // |-0000058cH|
    cout << formatMessage("|{:!#0'b}|",     { -1420 }) << "\n"; // |-0B0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:!#0'x}|",     { -1420 }) << "\n"; // |-0X0000'058c|
    cout << formatMessage("|{:!#0'X}|",     { -1420 }) << "\n"; // |-0x0000'058C|
    cout << formatMessage("|{:!#0'h}|",     { -1420 }) << "\n"; // |-0000'058cH|
    cout << formatMessage("|{:!#12b}|",     { -1420 }) << "\n"; // |-0B10110001100|
    cout << formatMessage("|{:!#12x}|",     { -1420 }) << "\n"; // |-0X58c      |
    cout << formatMessage("|{:!#12X}|",     { -1420 }) << "\n"; // |-0x58C      |
    cout << formatMessage("|{:!#12h}|",     { -1420 }) << "\n"; // |-58cH       |
    cout << formatMessage("|{:!#12'b}|",    { -1420 }) << "\n"; // |-0B101'1000'1100|
    cout << formatMessage("|{:!#012x}|",    { -1420 }) << "\n"; // |-0X58c|
    cout << formatMessage("|{:!#012X}|",    { -1420 }) << "\n"; // |-0x58C|
    cout << formatMessage("|{:!#012h}|",    { -1420 }) << "\n"; // |-58cH|
    cout << formatMessage("|{:~0b}|",       { -1420 }) << "\n"; // |11111111111111111111101001110100|
    cout << formatMessage("|{:~0x}|",       { -1420 }) << "\n"; // |fffffa74|
    cout << formatMessage("|{:~0X}|",       { -1420 }) << "\n"; // |FFFFFA74|
    cout << formatMessage("|{:~0'b}|",      { -1420 }) << "\n"; // |1111'1111'1111'1111'1111'1010'0111'0100|
    cout << formatMessage("|{:~0'x}|",      { -1420 }) << "\n"; // |ffff'fa74|
    cout << formatMessage("|{:~0'X}|",      { -1420 }) << "\n"; // |FFFF'FA74|
    cout << formatMessage("|{:~12x}|",      { -1420 }) << "\n"; // |fffffa74    |
    cout << formatMessage("|{:~12X}|",      { -1420 }) << "\n"; // |FFFFFA74    |
    cout << formatMessage("|{:~12'x}|",     { -1420 }) << "\n"; // |ffff'fa74   |
    cout << formatMessage("|{:~12'X}|",     { -1420 }) << "\n"; // |FFFF'FA74   |
    cout << formatMessage("|{:~#0b}|",      { -1420 }) << "\n"; // |0b11111111111111111111101001110100|
    cout << formatMessage("|{:~#0x}|",      { -1420 }) << "\n"; // |0xfffffa74|
    cout << formatMessage("|{:~#0X}|",      { -1420 }) << "\n"; // |0XFFFFFA74|
    cout << formatMessage("|{:~#0h}|",      { -1420 }) << "\n"; // |fffffa74h|
    cout << formatMessage("|{:~#0'b}|",     { -1420 }) << "\n"; // |0b1111'1111'1111'1111'1111'1010'0111'0100|
    cout << formatMessage("|{:~#0'x}|",     { -1420 }) << "\n"; // |0xffff'fa74|
    cout << formatMessage("|{:~#0'X}|",     { -1420 }) << "\n"; // |0XFFFF'FA74|
    cout << formatMessage("|{:~#0'h}|",     { -1420 }) << "\n"; // |ffff'fa74h|
    cout << formatMessage("|{:~#12x}|",     { -1420 }) << "\n"; // |0xfffffa74  |
    cout << formatMessage("|{:~#12X}|",     { -1420 }) << "\n"; // |0XFFFFFA74  |
    cout << formatMessage("|{:~#12h}|",     { -1420 }) << "\n"; // |fffffa74h   |
    cout << formatMessage("|{:~#12'x}|",    { -1420 }) << "\n"; // |0xffff'fa74 |
    cout << formatMessage("|{:~#12'X}|",    { -1420 }) << "\n"; // |0XFFFF'FA74 |
    cout << formatMessage("|{:~#12'h}|",    { -1420 }) << "\n"; // |ffff'fa74h  |
    cout << formatMessage("|{:~!#0b}|",     { -1420 }) << "\n"; // |0B11111111111111111111101001110100|
    cout << formatMessage("|{:~!#0x}|",     { -1420 }) << "\n"; // |0Xfffffa74|
    cout << formatMessage("|{:~!#0X}|",     { -1420 }) << "\n"; // |0xFFFFFA74|
    cout << formatMessage("|{:~!#0h}|",     { -1420 }) << "\n"; // |fffffa74H|
    cout << formatMessage("|{:~!#0'b}|",    { -1420 }) << "\n"; // |0B1111'1111'1111'1111'1111'1010'0111'0100|
    cout << formatMessage("|{:~!#0'x}|",    { -1420 }) << "\n"; // |0Xffff'fa74|
    cout << formatMessage("|{:~!#0'X}|",    { -1420 }) << "\n"; // |0xFFFF'FA74|
    cout << formatMessage("|{:~!#0'h}|",    { -1420 }) << "\n"; // |ffff'fa74H|
    cout << formatMessage("|{:~!#12x}|",    { -1420 }) << "\n"; // |0Xfffffa74  |
    cout << formatMessage("|{:~!#12X}|",    { -1420 }) << "\n"; // |0xFFFFFA74  |
    cout << formatMessage("|{:~!#12h}|",    { -1420 }) << "\n"; // |fffffa74H   |
    cout << formatMessage("|{:~!#12'x}|",   { -1420 }) << "\n"; // |0Xffff'fa74 |
    cout << formatMessage("|{:~!#12'X}|",   { -1420 }) << "\n"; // |0xFFFF'FA74 |
    cout << formatMessage("|{:~!#12'h}|",   { -1420 }) << "\n"; // |ffff'fa74H  |
    cout << formatMessage("|{:+~0b}|",      { -1420 }) << "\n"; // |+11111111111111111111101001110100|
    cout << formatMessage("|{:+~0x}|",      { -1420 }) << "\n"; // |+fffffa74|
    cout << formatMessage("|{:+~0X}|",      { -1420 }) << "\n"; // |+FFFFFA74|
    cout << formatMessage("|{:+~0'b}|",     { -1420 }) << "\n"; // |+1111'1111'1111'1111'1111'1010'0111'0100|
    cout << formatMessage("|{:+~0'x}|",     { -1420 }) << "\n"; // |+ffff'fa74|
    cout << formatMessage("|{:+~0'X}|",     { -1420 }) << "\n"; // |+FFFF'FA74|
    cout << formatMessage("|{:+~12x}|",     { -1420 }) << "\n"; // |+fffffa74   |
    cout << formatMessage("|{:+~12X}|",     { -1420 }) << "\n"; // |+FFFFFA74   |
    cout << formatMessage("|{:+~12'x}|",    { -1420 }) << "\n"; // |+ffff'fa74  |
    cout << formatMessage("|{:+~12'X}|",    { -1420 }) << "\n"; // |+FFFF'FA74  |
    cout << formatMessage("|{:+~#0b}|",     { -1420 }) << "\n"; // |+0b11111111111111111111101001110100|
    cout << formatMessage("|{:+~#0x}|",     { -1420 }) << "\n"; // |+0xfffffa74|
    cout << formatMessage("|{:+~#0X}|",     { -1420 }) << "\n"; // |+0XFFFFFA74|
    cout << formatMessage("|{:+~#0h}|",     { -1420 }) << "\n"; // |+fffffa74h|
    cout << formatMessage("|{:+~#0'b}|",    { -1420 }) << "\n"; // |+0b1111'1111'1111'1111'1111'1010'0111'0100|
    cout << formatMessage("|{:+~#0'x}|",    { -1420 }) << "\n"; // |+0xffff'fa74|
    cout << formatMessage("|{:+~#0'X}|",    { -1420 }) << "\n"; // |+0XFFFF'FA74|
    cout << formatMessage("|{:+~#0'h}|",    { -1420 }) << "\n"; // |+ffff'fa74h|
    cout << formatMessage("|{:+~#12x}|",    { -1420 }) << "\n"; // |+0xfffffa74 |
    cout << formatMessage("|{:+~#12X}|",    { -1420 }) << "\n"; // |+0XFFFFFA74 |
    cout << formatMessage("|{:+~#12h}|",    { -1420 }) << "\n"; // |+fffffa74h  |
    cout << formatMessage("|{:+~#12'h}|",   { -1420 }) << "\n"; // |+ffff'fa74h |
    cout << formatMessage("|{:+~!#0b}|",    { -1420 }) << "\n"; // |+0B11111111111111111111101001110100|
    cout << formatMessage("|{:+~!#0x}|",    { -1420 }) << "\n"; // |+0Xfffffa74|
    cout << formatMessage("|{:+~!#0X}|",    { -1420 }) << "\n"; // |+0xFFFFFA74|
    cout << formatMessage("|{:+~!#0h}|",    { -1420 }) << "\n"; // |+fffffa74H|
    cout << formatMessage("|{:+~!#0'b}|",   { -1420 }) << "\n"; // |+0B1111'1111'1111'1111'1111'1010'0111'0100|
    cout << formatMessage("|{:+~!#0'x}|",   { -1420 }) << "\n"; // |+0Xffff'fa74|
    cout << formatMessage("|{:+~!#0'X}|",   { -1420 }) << "\n"; // |+0xFFFF'FA74|
    cout << formatMessage("|{:+~!#0'h}|",   { -1420 }) << "\n"; // |+ffff'fa74H|
    cout << formatMessage("|{:+~!#12x}|",   { -1420 }) << "\n"; // |+0Xfffffa74 |
    cout << formatMessage("|{:+~!#12X}|",   { -1420 }) << "\n"; // |+0xFFFFFA74 |
    cout << formatMessage("|{:+~!#12h}|",   { -1420 }) << "\n"; // |+fffffa74H  |
    cout << formatMessage("|{:+~!#12'h}|",  { -1420 }) << "\n"; // |+ffff'fa74H |
    cout << formatMessage("|{:=12}|",       { -1420 }) << "\n"; // |-       1420|
    cout << formatMessage("|{:=12x}|",      { -1420 }) << "\n"; // |-        58c|
    cout << formatMessage("|{:=12X}|",      { -1420 }) << "\n"; // |-        58C|
    cout << formatMessage("|{:=12%}|",      { -1420 }) << "\n"; // |-      1420%|
    cout << formatMessage("|{:=12'}|",      { -1420 }) << "\n"; // |-      1'420|
    cout << formatMessage("|{:=12'%}|",     { -1420 }) << "\n"; // |-     1'420%|
    cout << formatMessage("|{:=12'n}|",     { -1420 }) << "\n"; // |-      1 420|
    cout << formatMessage("|{:=#12x}|",     { -1420 }) << "\n"; // |-      0x58c|
    cout << formatMessage("|{:=#12X}|",     { -1420 }) << "\n"; // |-      0X58C|
    cout << formatMessage("|{:=#12h}|",     { -1420 }) << "\n"; // |-       58ch|
    cout << formatMessage("|{:=!#12x}|",    { -1420 }) << "\n"; // |-      0X58c|
    cout << formatMessage("|{:=!#12X}|",    { -1420 }) << "\n"; // |-      0x58C|
    cout << formatMessage("|{:=!#12h}|",    { -1420 }) << "\n"; // |-       58cH|
    cout << formatMessage("|{:=~12x}|",     { -1420 }) << "\n"; // |    fffffa74|
    cout << formatMessage("|{:=~12X}|",     { -1420 }) << "\n"; // |    FFFFFA74|
    cout << formatMessage("|{:=~12'x}|",    { -1420 }) << "\n"; // |   ffff'fa74|
    cout << formatMessage("|{:=~12'X}|",    { -1420 }) << "\n"; // |   FFFF'FA74|
    cout << formatMessage("|{:=~#12x}|",    { -1420 }) << "\n"; // |  0xfffffa74|
    cout << formatMessage("|{:=~#12X}|",    { -1420 }) << "\n"; // |  0XFFFFFA74|
    cout << formatMessage("|{:=~#12h}|",    { -1420 }) << "\n"; // |   fffffa74h|
    cout << formatMessage("|{:=~#12'x}|",   { -1420 }) << "\n"; // | 0xffff'fa74|
    cout << formatMessage("|{:=~#12'X}|",   { -1420 }) << "\n"; // | 0XFFFF'FA74|
    cout << formatMessage("|{:=~#12'h}|",   { -1420 }) << "\n"; // |  ffff'fa74h|
    cout << formatMessage("|{:=~!#12x}|",   { -1420 }) << "\n"; // |  0Xfffffa74|
    cout << formatMessage("|{:=~!#12X}|",   { -1420 }) << "\n"; // |  0xFFFFFA74|
    cout << formatMessage("|{:=~!#12h}|",   { -1420 }) << "\n"; // |   fffffa74H|
    cout << formatMessage("|{:=~!#12'x}|",  { -1420 }) << "\n"; // | 0Xffff'fa74|
    cout << formatMessage("|{:=~!#12'X}|",  { -1420 }) << "\n"; // | 0xFFFF'FA74|
    cout << formatMessage("|{:=~!#12'h}|",  { -1420 }) << "\n"; // |  ffff'fa74H|
    cout << formatMessage("|{:=+~12x}|",    { -1420 }) << "\n"; // |+   fffffa74|
    cout << formatMessage("|{:=+~12X}|",    { -1420 }) << "\n"; // |+   FFFFFA74|
    cout << formatMessage("|{:=+~12'x}|",   { -1420 }) << "\n"; // |+  ffff'fa74|
    cout << formatMessage("|{:=+~12'X}|",   { -1420 }) << "\n"; // |+  FFFF'FA74|
    cout << formatMessage("|{:=+~#12x}|",   { -1420 }) << "\n"; // |+ 0xfffffa74|
    cout << formatMessage("|{:=+~#12X}|",   { -1420 }) << "\n"; // |+ 0XFFFFFA74|
    cout << formatMessage("|{:=+~#12h}|",   { -1420 }) << "\n"; // |+  fffffa74h|
    cout << formatMessage("|{:=+~#12'h}|",  { -1420 }) << "\n"; // |+ ffff'fa74h|
    cout << formatMessage("|{:=+~!#12x}|",  { -1420 }) << "\n"; // |+ 0Xfffffa74|
    cout << formatMessage("|{:=+~!#12X}|",  { -1420 }) << "\n"; // |+ 0xFFFFFA74|
    cout << formatMessage("|{:=+~!#12h}|",  { -1420 }) << "\n"; // |+  fffffa74H|
    cout << formatMessage("|{:=+~!#12'h}|", { -1420 }) << "\n"; // |+ ffff'fa74H|
    cout << formatMessage("|{:= ~0b}|",     { -1420 }) << "\n"; // | 11111111111111111111101001110100|
    cout << formatMessage("|{:= ~0x}|",     { -1420 }) << "\n"; // | fffffa74|
    cout << formatMessage("|{:= ~0X}|",     { -1420 }) << "\n"; // | FFFFFA74|
    cout << formatMessage("|{:= ~0'b}|",    { -1420 }) << "\n"; // | 1111'1111'1111'1111'1111'1010'0111'0100|
    cout << formatMessage("|{:= ~0'x}|",    { -1420 }) << "\n"; // | ffff'fa74|
    cout << formatMessage("|{:= ~0'X}|",    { -1420 }) << "\n"; // | FFFF'FA74|
    cout << formatMessage("|{:= ~#0b}|",    { -1420 }) << "\n"; // | 0b11111111111111111111101001110100|
    cout << formatMessage("|{:= ~#0x}|",    { -1420 }) << "\n"; // | 0xfffffa74|
    cout << formatMessage("|{:= ~#0X}|",    { -1420 }) << "\n"; // | 0XFFFFFA74|
    cout << formatMessage("|{:= ~#0h}|",    { -1420 }) << "\n"; // | fffffa74h|
    cout << formatMessage("|{:= ~#0'b}|",   { -1420 }) << "\n"; // | 0b1111'1111'1111'1111'1111'1010'0111'0100|
    cout << formatMessage("|{:= ~#0'h}|",   { -1420 }) << "\n"; // | ffff'fa74h|
    cout << formatMessage("|{:= ~!#0b}|",   { -1420 }) << "\n"; // | 0B11111111111111111111101001110100|
    cout << formatMessage("|{:= ~!#0x}|",   { -1420 }) << "\n"; // | 0Xfffffa74|
    cout << formatMessage("|{:= ~!#0X}|",   { -1420 }) << "\n"; // | 0xFFFFFA74|
    cout << formatMessage("|{:= ~!#0h}|",   { -1420 }) << "\n"; // | fffffa74H|
    cout << formatMessage("|{:= ~!#0'b}|",  { -1420 }) << "\n"; // | 0B1111'1111'1111'1111'1111'1010'0111'0100|
    cout << formatMessage("|{:= ~!#0'h}|",  { -1420 }) << "\n"; // | ffff'fa74H|
    cout << formatMessage("|{:*=12}|",      { -1420 }) << "\n"; // |-*******1420|
    cout << formatMessage("|{:*=12x}|",     { -1420 }) << "\n"; // |-********58c|
    cout << formatMessage("|{:*=12X}|",     { -1420 }) << "\n"; // |-********58C|
    cout << formatMessage("|{:*=12%}|",     { -1420 }) << "\n"; // |-******1420%|
    cout << formatMessage("|{:*=12'}|",     { -1420 }) << "\n"; // |-******1'420|
    cout << formatMessage("|{:*=12'%}|",    { -1420 }) << "\n"; // |-*****1'420%|
    cout << formatMessage("|{:*=12'n}|",    { -1420 }) << "\n"; // |-******1 420|
    cout << formatMessage("|{:*=#12x}|",    { -1420 }) << "\n"; // |-******0x58c|
    cout << formatMessage("|{:*=#12X}|",    { -1420 }) << "\n"; // |-******0X58C|
    cout << formatMessage("|{:*=#12h}|",    { -1420 }) << "\n"; // |-*******58ch|
    cout << formatMessage("|{:*=!#12x}|",   { -1420 }) << "\n"; // |-******0X58c|
    cout << formatMessage("|{:*=!#12X}|",   { -1420 }) << "\n"; // |-******0x58C|
    cout << formatMessage("|{:*=!#12h}|",   { -1420 }) << "\n"; // |-*******58cH|
    cout << formatMessage("|{:*=~12x}|",    { -1420 }) << "\n"; // |****fffffa74|
    cout << formatMessage("|{:*=~12X}|",    { -1420 }) << "\n"; // |****FFFFFA74|
    cout << formatMessage("|{:*=~12'x}|",   { -1420 }) << "\n"; // |***ffff'fa74|
    cout << formatMessage("|{:*=~12'X}|",   { -1420 }) << "\n"; // |***FFFF'FA74|
    cout << formatMessage("|{:*=~#12x}|",   { -1420 }) << "\n"; // |**0xfffffa74|
    cout << formatMessage("|{:*=~#12X}|",   { -1420 }) << "\n"; // |**0XFFFFFA74|
    cout << formatMessage("|{:*=~#12h}|",   { -1420 }) << "\n"; // |***fffffa74h|
    cout << formatMessage("|{:*=~#12'x}|",  { -1420 }) << "\n"; // |*0xffff'fa74|
    cout << formatMessage("|{:*=~#12'X}|",  { -1420 }) << "\n"; // |*0XFFFF'FA74|
    cout << formatMessage("|{:*=~#12'h}|",  { -1420 }) << "\n"; // |**ffff'fa74h|
    cout << formatMessage("|{:*=~!#12x}|",  { -1420 }) << "\n"; // |**0Xfffffa74|
    cout << formatMessage("|{:*=~!#12X}|",  { -1420 }) << "\n"; // |**0xFFFFFA74|
    cout << formatMessage("|{:*=~!#12h}|",  { -1420 }) << "\n"; // |***fffffa74H|
    cout << formatMessage("|{:*=~!#12'x}|", { -1420 }) << "\n"; // |*0Xffff'fa74|
    cout << formatMessage("|{:*=~!#12'X}|", { -1420 }) << "\n"; // |*0xFFFF'FA74|
    cout << formatMessage("|{:*=~!#12'h}|", { -1420 }) << "\n"; // |**ffff'fa74H|
    cout << formatMessage("|{:*=+~12x}|",   { -1420 }) << "\n"; // |+***fffffa74|
    cout << formatMessage("|{:*=+~12X}|",   { -1420 }) << "\n"; // |+***FFFFFA74|
    cout << formatMessage("|{:*=+~12'x}|",  { -1420 }) << "\n"; // |+**ffff'fa74|
    cout << formatMessage("|{:*=+~12'X}|",  { -1420 }) << "\n"; // |+**FFFF'FA74|
    cout << formatMessage("|{:*=+~#12x}|",  { -1420 }) << "\n"; // |+*0xfffffa74|
    cout << formatMessage("|{:*=+~#12X}|",  { -1420 }) << "\n"; // |+*0XFFFFFA74|
    cout << formatMessage("|{:*=+~#12h}|",  { -1420 }) << "\n"; // |+**fffffa74h|
    cout << formatMessage("|{:*=+~#12'h}|", { -1420 }) << "\n"; // |+*ffff'fa74h|
    cout << formatMessage("|{:*=+~!#12x}|", { -1420 }) << "\n"; // |+*0Xfffffa74|
    cout << formatMessage("|{:*=+~!#12X}|", { -1420 }) << "\n"; // |+*0xFFFFFA74|
    cout << formatMessage("|{:*=+~!#12h}|", { -1420 }) << "\n"; // |+**fffffa74H|
    cout << formatMessage("|{:*=+~!#12'h}|", { -1420 }) << "\n"; // |+*ffff'fa74H|
    cout << formatMessage("|{:*= ~12x}|",   { -1420 }) << "\n"; // | ***fffffa74|
    cout << formatMessage("|{:*= ~12X}|",   { -1420 }) << "\n"; // | ***FFFFFA74|
    cout << formatMessage("|{:*= ~12'x}|",  { -1420 }) << "\n"; // | **ffff'fa74|
    cout << formatMessage("|{:*= ~12'X}|",  { -1420 }) << "\n"; // | **FFFF'FA74|
    cout << formatMessage("|{:*= ~#12x}|",  { -1420 }) << "\n"; // | *0xfffffa74|
    cout << formatMessage("|{:*= ~#12X}|",  { -1420 }) << "\n"; // | *0XFFFFFA74|
    cout << formatMessage("|{:*= ~#12h}|",  { -1420 }) << "\n"; // | **fffffa74h|
    cout << formatMessage("|{:*= ~#12'h}|", { -1420 }) << "\n"; // | *ffff'fa74h|
    cout << formatMessage("|{:*= ~!#12x}|", { -1420 }) << "\n"; // | *0Xfffffa74|
    cout << formatMessage("|{:*= ~!#12X}|", { -1420 }) << "\n"; // | *0xFFFFFA74|
    cout << formatMessage("|{:*= ~!#12h}|", { -1420 }) << "\n"; // | **fffffa74H|
    cout << formatMessage("|{:*= ~!#12'h}|", { -1420 }) << "\n"; // | *ffff'fa74H|
    cout << formatMessage("|{:< ~12x}|",    { -1420 }) << "\n"; // | fffffa74   |
    cout << formatMessage("|{:< ~12X}|",    { -1420 }) << "\n"; // | FFFFFA74   |
    cout << formatMessage("|{:< ~12'x}|",   { -1420 }) << "\n"; // | ffff'fa74  |
    cout << formatMessage("|{:< ~12'X}|",   { -1420 }) << "\n"; // | FFFF'FA74  |
    cout << formatMessage("|{:< ~#12x}|",   { -1420 }) << "\n"; // | 0xfffffa74 |
    cout << formatMessage("|{:< ~#12X}|",   { -1420 }) << "\n"; // | 0XFFFFFA74 |
    cout << formatMessage("|{:< ~#12h}|",   { -1420 }) << "\n"; // | fffffa74h  |
    cout << formatMessage("|{:< ~#12'h}|",  { -1420 }) << "\n"; // | ffff'fa74h |
    cout << formatMessage("|{:< ~!#12x}|",  { -1420 }) << "\n"; // | 0Xfffffa74 |
    cout << formatMessage("|{:< ~!#12X}|",  { -1420 }) << "\n"; // | 0xFFFFFA74 |
    cout << formatMessage("|{:< ~!#12h}|",  { -1420 }) << "\n"; // | fffffa74H  |
    cout << formatMessage("|{:< ~!#12'h}|", { -1420 }) << "\n"; // | ffff'fa74H |
    cout << formatMessage("|{:*<12}|",      { -1420 }) << "\n"; // |-1420*******|
    cout << formatMessage("|{:*<12x}|",     { -1420 }) << "\n"; // |-58c********|
    cout << formatMessage("|{:*<12X}|",     { -1420 }) << "\n"; // |-58C********|
    cout << formatMessage("|{:*<12%}|",     { -1420 }) << "\n"; // |-1420%******|
    cout << formatMessage("|{:*<12'}|",     { -1420 }) << "\n"; // |-1'420******|
    cout << formatMessage("|{:*<12'%}|",    { -1420 }) << "\n"; // |-1'420%*****|
    cout << formatMessage("|{:*<12'n}|",    { -1420 }) << "\n"; // |-1 420******|
    cout << formatMessage("|{:*<#12x}|",    { -1420 }) << "\n"; // |-0x58c******|
    cout << formatMessage("|{:*<#12X}|",    { -1420 }) << "\n"; // |-0X58C******|
    cout << formatMessage("|{:*<#12h}|",    { -1420 }) << "\n"; // |-58ch*******|
    cout << formatMessage("|{:*<!#12x}|",   { -1420 }) << "\n"; // |-0X58c******|
    cout << formatMessage("|{:*<!#12X}|",   { -1420 }) << "\n"; // |-0x58C******|
    cout << formatMessage("|{:*<!#12h}|",   { -1420 }) << "\n"; // |-58cH*******|
    cout << formatMessage("|{:*<~12x}|",    { -1420 }) << "\n"; // |fffffa74****|
    cout << formatMessage("|{:*<~12X}|",    { -1420 }) << "\n"; // |FFFFFA74****|
    cout << formatMessage("|{:*<~12'x}|",   { -1420 }) << "\n"; // |ffff'fa74***|
    cout << formatMessage("|{:*<~12'X}|",   { -1420 }) << "\n"; // |FFFF'FA74***|
    cout << formatMessage("|{:*<~#12x}|",   { -1420 }) << "\n"; // |0xfffffa74**|
    cout << formatMessage("|{:*<~#12X}|",   { -1420 }) << "\n"; // |0XFFFFFA74**|
    cout << formatMessage("|{:*<~#12h}|",   { -1420 }) << "\n"; // |fffffa74h***|
    cout << formatMessage("|{:*<~#12'x}|",  { -1420 }) << "\n"; // |0xffff'fa74*|
    cout << formatMessage("|{:*<~#12'X}|",  { -1420 }) << "\n"; // |0XFFFF'FA74*|
    cout << formatMessage("|{:*<~#12'h}|",  { -1420 }) << "\n"; // |ffff'fa74h**|
    cout << formatMessage("|{:*<~!#12x}|",  { -1420 }) << "\n"; // |0Xfffffa74**|
    cout << formatMessage("|{:*<~!#12X}|",  { -1420 }) << "\n"; // |0xFFFFFA74**|
    cout << formatMessage("|{:*<~!#12h}|",  { -1420 }) << "\n"; // |fffffa74H***|
    cout << formatMessage("|{:*<~!#12'x}|", { -1420 }) << "\n"; // |0Xffff'fa74*|
    cout << formatMessage("|{:*<~!#12'X}|", { -1420 }) << "\n"; // |0xFFFF'FA74*|
    cout << formatMessage("|{:*<~!#12'h}|", { -1420 }) << "\n"; // |ffff'fa74H**|
    cout << formatMessage("|{:*<+~12x}|",   { -1420 }) << "\n"; // |+fffffa74***|
    cout << formatMessage("|{:*<+~12X}|",   { -1420 }) << "\n"; // |+FFFFFA74***|
    cout << formatMessage("|{:*<+~12'x}|",  { -1420 }) << "\n"; // |+ffff'fa74**|
    cout << formatMessage("|{:*<+~12'X}|",  { -1420 }) << "\n"; // |+FFFF'FA74**|
    cout << formatMessage("|{:*<+~#12x}|",  { -1420 }) << "\n"; // |+0xfffffa74*|
    cout << formatMessage("|{:*<+~#12X}|",  { -1420 }) << "\n"; // |+0XFFFFFA74*|
    cout << formatMessage("|{:*<+~#12h}|",  { -1420 }) << "\n"; // |+fffffa74h**|
    cout << formatMessage("|{:*<+~#12'h}|", { -1420 }) << "\n"; // |+ffff'fa74h*|
    cout << formatMessage("|{:*<+~!#12x}|", { -1420 }) << "\n"; // |+0Xfffffa74*|
    cout << formatMessage("|{:*<+~!#12X}|", { -1420 }) << "\n"; // |+0xFFFFFA74*|
    cout << formatMessage("|{:*<+~!#12h}|", { -1420 }) << "\n"; // |+fffffa74H**|
    cout << formatMessage("|{:*<+~!#12'h}|", { -1420 }) << "\n"; // |+ffff'fa74H*|
    cout << formatMessage("|{:*< ~12x}|",   { -1420 }) << "\n"; // | fffffa74***|
    cout << formatMessage("|{:*< ~12X}|",   { -1420 }) << "\n"; // | FFFFFA74***|
    cout << formatMessage("|{:*< ~12'x}|",  { -1420 }) << "\n"; // | ffff'fa74**|
    cout << formatMessage("|{:*< ~12'X}|",  { -1420 }) << "\n"; // | FFFF'FA74**|
    cout << formatMessage("|{:*< ~#12x}|",  { -1420 }) << "\n"; // | 0xfffffa74*|
    cout << formatMessage("|{:*< ~#12X}|",  { -1420 }) << "\n"; // | 0XFFFFFA74*|
    cout << formatMessage("|{:*< ~#12h}|",  { -1420 }) << "\n"; // | fffffa74h**|
    cout << formatMessage("|{:*< ~#12'h}|", { -1420 }) << "\n"; // | ffff'fa74h*|
    cout << formatMessage("|{:*< ~!#12x}|", { -1420 }) << "\n"; // | 0Xfffffa74*|
    cout << formatMessage("|{:*< ~!#12X}|", { -1420 }) << "\n"; // | 0xFFFFFA74*|
    cout << formatMessage("|{:*< ~!#12h}|", { -1420 }) << "\n"; // | fffffa74H**|
    cout << formatMessage("|{:*< ~!#12'h}|", { -1420 }) << "\n"; // | ffff'fa74H*|
    cout << formatMessage("|{:>12}|",       { -1420 }) << "\n"; // |       -1420|
    cout << formatMessage("|{:>12x}|",      { -1420 }) << "\n"; // |        -58c|
    cout << formatMessage("|{:>12X}|",      { -1420 }) << "\n"; // |        -58C|
    cout << formatMessage("|{:>12%}|",      { -1420 }) << "\n"; // |      -1420%|
    cout << formatMessage("|{:>12'}|",      { -1420 }) << "\n"; // |      -1'420|
    cout << formatMessage("|{:>12'%}|",     { -1420 }) << "\n"; // |     -1'420%|
    cout << formatMessage("|{:>12'n}|",     { -1420 }) << "\n"; // |      -1 420|
    cout << formatMessage("|{:>#12x}|",     { -1420 }) << "\n"; // |      -0x58c|
    cout << formatMessage("|{:>#12X}|",     { -1420 }) << "\n"; // |      -0X58C|
    cout << formatMessage("|{:>#12h}|",     { -1420 }) << "\n"; // |       -58ch|
    cout << formatMessage("|{:>!#12x}|",    { -1420 }) << "\n"; // |      -0X58c|
    cout << formatMessage("|{:>!#12X}|",    { -1420 }) << "\n"; // |      -0x58C|
    cout << formatMessage("|{:>!#12h}|",    { -1420 }) << "\n"; // |       -58cH|
    cout << formatMessage("|{:>+~12x}|",    { -1420 }) << "\n"; // |   +fffffa74|
    cout << formatMessage("|{:>+~12X}|",    { -1420 }) << "\n"; // |   +FFFFFA74|
    cout << formatMessage("|{:>+~12'x}|",   { -1420 }) << "\n"; // |  +ffff'fa74|
    cout << formatMessage("|{:>+~12'X}|",   { -1420 }) << "\n"; // |  +FFFF'FA74|
    cout << formatMessage("|{:>+~#12x}|",   { -1420 }) << "\n"; // | +0xfffffa74|
    cout << formatMessage("|{:>+~#12X}|",   { -1420 }) << "\n"; // | +0XFFFFFA74|
    cout << formatMessage("|{:>+~#12h}|",   { -1420 }) << "\n"; // |  +fffffa74h|
    cout << formatMessage("|{:>+~#12'h}|",  { -1420 }) << "\n"; // | +ffff'fa74h|
    cout << formatMessage("|{:>+~!#12x}|",  { -1420 }) << "\n"; // | +0Xfffffa74|
    cout << formatMessage("|{:>+~!#12X}|",  { -1420 }) << "\n"; // | +0xFFFFFA74|
    cout << formatMessage("|{:>+~!#12h}|",  { -1420 }) << "\n"; // |  +fffffa74H|
    cout << formatMessage("|{:>+~!#12'h}|", { -1420 }) << "\n"; // | +ffff'fa74H|
    cout << formatMessage("|{:*>12}|",      { -1420 }) << "\n"; // |*******-1420|
    cout << formatMessage("|{:*>12x}|",     { -1420 }) << "\n"; // |********-58c|
    cout << formatMessage("|{:*>12X}|",     { -1420 }) << "\n"; // |********-58C|
    cout << formatMessage("|{:*>12%}|",     { -1420 }) << "\n"; // |******-1420%|
    cout << formatMessage("|{:*>12'}|",     { -1420 }) << "\n"; // |******-1'420|
    cout << formatMessage("|{:*>12'%}|",    { -1420 }) << "\n"; // |*****-1'420%|
    cout << formatMessage("|{:*>12'n}|",    { -1420 }) << "\n"; // |******-1 420|
    cout << formatMessage("|{:*>#12x}|",    { -1420 }) << "\n"; // |******-0x58c|
    cout << formatMessage("|{:*>#12X}|",    { -1420 }) << "\n"; // |******-0X58C|
    cout << formatMessage("|{:*>#12h}|",    { -1420 }) << "\n"; // |*******-58ch|
    cout << formatMessage("|{:*>!#12x}|",   { -1420 }) << "\n"; // |******-0X58c|
    cout << formatMessage("|{:*>!#12X}|",   { -1420 }) << "\n"; // |******-0x58C|
    cout << formatMessage("|{:*>!#12h}|",   { -1420 }) << "\n"; // |*******-58cH|
    cout << formatMessage("|{:*>+~12x}|",   { -1420 }) << "\n"; // |***+fffffa74|
    cout << formatMessage("|{:*>+~12X}|",   { -1420 }) << "\n"; // |***+FFFFFA74|
    cout << formatMessage("|{:*>+~12'x}|",  { -1420 }) << "\n"; // |**+ffff'fa74|
    cout << formatMessage("|{:*>+~12'X}|",  { -1420 }) << "\n"; // |**+FFFF'FA74|
    cout << formatMessage("|{:*>+~#12x}|",  { -1420 }) << "\n"; // |*+0xfffffa74|
    cout << formatMessage("|{:*>+~#12X}|",  { -1420 }) << "\n"; // |*+0XFFFFFA74|
    cout << formatMessage("|{:*>+~#12h}|",  { -1420 }) << "\n"; // |**+fffffa74h|
    cout << formatMessage("|{:*>+~#12'h}|", { -1420 }) << "\n"; // |*+ffff'fa74h|
    cout << formatMessage("|{:*>+~!#12x}|", { -1420 }) << "\n"; // |*+0Xfffffa74|
    cout << formatMessage("|{:*>+~!#12X}|", { -1420 }) << "\n"; // |*+0xFFFFFA74|
    cout << formatMessage("|{:*>+~!#12h}|", { -1420 }) << "\n"; // |**+fffffa74H|
    cout << formatMessage("|{:*>+~!#12'h}|", { -1420 }) << "\n"; // |*+ffff'fa74H|
    cout << formatMessage("|{:*> ~12x}|",   { -1420 }) << "\n"; // |*** fffffa74|
    cout << formatMessage("|{:*> ~12X}|",   { -1420 }) << "\n"; // |*** FFFFFA74|
    cout << formatMessage("|{:*> ~12'x}|",  { -1420 }) << "\n"; // |** ffff'fa74|
    cout << formatMessage("|{:*> ~12'X}|",  { -1420 }) << "\n"; // |** FFFF'FA74|
    cout << formatMessage("|{:*> ~#12x}|",  { -1420 }) << "\n"; // |* 0xfffffa74|
    cout << formatMessage("|{:*> ~#12X}|",  { -1420 }) << "\n"; // |* 0XFFFFFA74|
    cout << formatMessage("|{:*> ~#12h}|",  { -1420 }) << "\n"; // |** fffffa74h|
    cout << formatMessage("|{:*> ~#12'h}|", { -1420 }) << "\n"; // |* ffff'fa74h|
    cout << formatMessage("|{:*> ~!#12x}|", { -1420 }) << "\n"; // |* 0Xfffffa74|
    cout << formatMessage("|{:*> ~!#12X}|", { -1420 }) << "\n"; // |* 0xFFFFFA74|
    cout << formatMessage("|{:*> ~!#12h}|", { -1420 }) << "\n"; // |** fffffa74H|
    cout << formatMessage("|{:*> ~!#12'h}|", { -1420 }) << "\n"; // |* ffff'fa74H|
    cout << formatMessage("|{:^12}|",       { -1420 }) << "\n"; // |   -1420    |
    cout << formatMessage("|{:^12x}|",      { -1420 }) << "\n"; // |    -58c    |
    cout << formatMessage("|{:^12X}|",      { -1420 }) << "\n"; // |    -58C    |
    cout << formatMessage("|{:^12%}|",      { -1420 }) << "\n"; // |   -1420%   |
    cout << formatMessage("|{:^12'}|",      { -1420 }) << "\n"; // |   -1'420   |
    cout << formatMessage("|{:^12'%}|",     { -1420 }) << "\n"; // |  -1'420%   |
    cout << formatMessage("|{:^12'n}|",     { -1420 }) << "\n"; // |   -1 420   |
    cout << formatMessage("|{:^#12x}|",     { -1420 }) << "\n"; // |   -0x58c   |
    cout << formatMessage("|{:^#12X}|",     { -1420 }) << "\n"; // |   -0X58C   |
    cout << formatMessage("|{:^#12h}|",     { -1420 }) << "\n"; // |   -58ch    |
    cout << formatMessage("|{:^!#12x}|",    { -1420 }) << "\n"; // |   -0X58c   |
    cout << formatMessage("|{:^!#12X}|",    { -1420 }) << "\n"; // |   -0x58C   |
    cout << formatMessage("|{:^!#12h}|",    { -1420 }) << "\n"; // |   -58cH    |
    cout << formatMessage("|{:^~12x}|",     { -1420 }) << "\n"; // |  fffffa74  |
    cout << formatMessage("|{:^~12X}|",     { -1420 }) << "\n"; // |  FFFFFA74  |
    cout << formatMessage("|{:^+~12x}|",    { -1420 }) << "\n"; // | +fffffa74  |
    cout << formatMessage("|{:^+~12X}|",    { -1420 }) << "\n"; // | +FFFFFA74  |
    cout << formatMessage("|{:^+~12'x}|",   { -1420 }) << "\n"; // | +ffff'fa74 |
    cout << formatMessage("|{:^+~12'X}|",   { -1420 }) << "\n"; // | +FFFF'FA74 |
    cout << formatMessage("|{:^+~#12h}|",   { -1420 }) << "\n"; // | +fffffa74h |
    cout << formatMessage("|{:^+~!#12h}|",  { -1420 }) << "\n"; // | +fffffa74H |
    cout << formatMessage("|{:^ ~12'x}|",   { -1420 }) << "\n"; // |  ffff'fa74 |
    cout << formatMessage("|{:^ ~12'X}|",   { -1420 }) << "\n"; // |  FFFF'FA74 |
    cout << formatMessage("|{:^ ~#12h}|",   { -1420 }) << "\n"; // |  fffffa74h |
    cout << formatMessage("|{:^ ~!#12h}|",  { -1420 }) << "\n"; // |  fffffa74H |
    cout << formatMessage("|{:*^12}|",      { -1420 }) << "\n"; // |***-1420****|
    cout << formatMessage("|{:*^12x}|",     { -1420 }) << "\n"; // |****-58c****|
    cout << formatMessage("|{:*^12X}|",     { -1420 }) << "\n"; // |****-58C****|
    cout << formatMessage("|{:*^12%}|",     { -1420 }) << "\n"; // |***-1420%***|
    cout << formatMessage("|{:*^12'}|",     { -1420 }) << "\n"; // |***-1'420***|
    cout << formatMessage("|{:*^12'%}|",    { -1420 }) << "\n"; // |**-1'420%***|
    cout << formatMessage("|{:*^12'n}|",    { -1420 }) << "\n"; // |***-1 420***|
    cout << formatMessage("|{:*^#12x}|",    { -1420 }) << "\n"; // |***-0x58c***|
    cout << formatMessage("|{:*^#12X}|",    { -1420 }) << "\n"; // |***-0X58C***|
    cout << formatMessage("|{:*^#12h}|",    { -1420 }) << "\n"; // |***-58ch****|
    cout << formatMessage("|{:*^!#12x}|",   { -1420 }) << "\n"; // |***-0X58c***|
    cout << formatMessage("|{:*^!#12X}|",   { -1420 }) << "\n"; // |***-0x58C***|
    cout << formatMessage("|{:*^!#12h}|",   { -1420 }) << "\n"; // |***-58cH****|
    cout << formatMessage("|{:*^~12x}|",    { -1420 }) << "\n"; // |**fffffa74**|
    cout << formatMessage("|{:*^~12X}|",    { -1420 }) << "\n"; // |**FFFFFA74**|
    cout << formatMessage("|{:*^~12'x}|",   { -1420 }) << "\n"; // |*ffff'fa74**|
    cout << formatMessage("|{:*^~12'X}|",   { -1420 }) << "\n"; // |*FFFF'FA74**|
    cout << formatMessage("|{:*^~#12x}|",   { -1420 }) << "\n"; // |*0xfffffa74*|
    cout << formatMessage("|{:*^~#12X}|",   { -1420 }) << "\n"; // |*0XFFFFFA74*|
    cout << formatMessage("|{:*^~#12h}|",   { -1420 }) << "\n"; // |*fffffa74h**|
    cout << formatMessage("|{:*^~#12'h}|",  { -1420 }) << "\n"; // |*ffff'fa74h*|
    cout << formatMessage("|{:*^~!#12x}|",  { -1420 }) << "\n"; // |*0Xfffffa74*|
    cout << formatMessage("|{:*^~!#12X}|",  { -1420 }) << "\n"; // |*0xFFFFFA74*|
    cout << formatMessage("|{:*^~!#12h}|",  { -1420 }) << "\n"; // |*fffffa74H**|
    cout << formatMessage("|{:*^~!#12'h}|", { -1420 }) << "\n"; // |*ffff'fa74H*|
    cout << formatMessage("|{:*^+~12x}|",   { -1420 }) << "\n"; // |*+fffffa74**|
    cout << formatMessage("|{:*^+~12X}|",   { -1420 }) << "\n"; // |*+FFFFFA74**|
    cout << formatMessage("|{:*^+~12'x}|",  { -1420 }) << "\n"; // |*+ffff'fa74*|
    cout << formatMessage("|{:*^+~12'X}|",  { -1420 }) << "\n"; // |*+FFFF'FA74*|
    cout << formatMessage("|{:*^+~#12h}|",  { -1420 }) << "\n"; // |*+fffffa74h*|
    cout << formatMessage("|{:*^+~!#12h}|", { -1420 }) << "\n"; // |*+fffffa74H*|
    cout << formatMessage("|{:*^ ~12x}|",   { -1420 }) << "\n"; // |* fffffa74**|
    cout << formatMessage("|{:*^ ~12X}|",   { -1420 }) << "\n"; // |* FFFFFA74**|
    cout << formatMessage("|{:*^ ~12'x}|",  { -1420 }) << "\n"; // |* ffff'fa74*|
    cout << formatMessage("|{:*^ ~12'X}|",  { -1420 }) << "\n"; // |* FFFF'FA74*|
    cout << formatMessage("|{:*^ ~#12h}|",  { -1420 }) << "\n"; // |* fffffa74h*|
    cout << formatMessage("|{:*^ ~!#12h}|", { -1420 }) << "\n"; // |* fffffa74H*|
    cout << formatMessage("|{:0}|",         { 1420 }) << "\n"; // |1420|
    cout << formatMessage("|{:0b}|",        { 1420 }) << "\n"; // |00000000000000000000010110001100|
    cout << formatMessage("|{:0x}|",        { 1420 }) << "\n"; // |0000058c|
    cout << formatMessage("|{:0X}|",        { 1420 }) << "\n"; // |0000058C|
    cout << formatMessage("|{:0%}|",        { 1420 }) << "\n"; // |1420%|
    cout << formatMessage("|{:0'}|",        { 1420 }) << "\n"; // |1'420|
    cout << formatMessage("|{:0'b}|",       { 1420 }) << "\n"; // |0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:0'x}|",       { 1420 }) << "\n"; // |0000'058c|
    cout << formatMessage("|{:0'X}|",       { 1420 }) << "\n"; // |0000'058C|
    cout << formatMessage("|{:0'%}|",       { 1420 }) << "\n"; // |1'420%|
    cout << formatMessage("|{:0'n}|",       { 1420 }) << "\n"; // |1 420|
    cout << formatMessage("|{:12}|",        { 1420 }) << "\n"; // |1420        |
    cout << formatMessage("|{:12b}|",       { 1420 }) << "\n"; // |10110001100 |
    cout << formatMessage("|{:12x}|",       { 1420 }) << "\n"; // |58c         |
    cout << formatMessage("|{:12X}|",       { 1420 }) << "\n"; // |58C         |
    cout << formatMessage("|{:12%}|",       { 1420 }) << "\n"; // |1420%       |
    cout << formatMessage("|{:12'}|",       { 1420 }) << "\n"; // |1'420       |
    cout << formatMessage("|{:12'b}|",      { 1420 }) << "\n"; // |101'1000'1100|
    cout << formatMessage("|{:12'%}|",      { 1420 }) << "\n"; // |1'420%      |
    cout << formatMessage("|{:12'n}|",      { 1420 }) << "\n"; // |1 420       |
    cout << formatMessage("|{:#0b}|",       { 1420 }) << "\n"; // |0b00000000000000000000010110001100|
    cout << formatMessage("|{:#0x}|",       { 1420 }) << "\n"; // |0x0000058c|
    cout << formatMessage("|{:#0X}|",       { 1420 }) << "\n"; // |0X0000058C|
    cout << formatMessage("|{:#0h}|",       { 1420 }) << "\n"; // |0000058ch|
    cout << formatMessage("|{:#0'b}|",      { 1420 }) << "\n"; // |0b0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:#0'x}|",      { 1420 }) << "\n"; // |0x0000'058c|
    cout << formatMessage("|{:#0'X}|",      { 1420 }) << "\n"; // |0X0000'058C|
    cout << formatMessage("|{:#0'h}|",      { 1420 }) << "\n"; // |0000'058ch|
    cout << formatMessage("|{:#12b}|",      { 1420 }) << "\n"; // |0b10110001100|
    cout << formatMessage("|{:#12x}|",      { 1420 }) << "\n"; // |0x58c       |
    cout << formatMessage("|{:#12X}|",      { 1420 }) << "\n"; // |0X58C       |
    cout << formatMessage("|{:#12h}|",      { 1420 }) << "\n"; // |58ch        |
    cout << formatMessage("|{:#12'b}|",     { 1420 }) << "\n"; // |0b101'1000'1100|
    cout << formatMessage("|{:012b}|",      { 1420 }) << "\n"; // |10110001100|
    cout << formatMessage("|{:012x}|",      { 1420 }) << "\n"; // |58c|
    cout << formatMessage("|{:012X}|",      { 1420 }) << "\n"; // |58C|
    cout << formatMessage("|{:#012x}|",     { 1420 }) << "\n"; // |0x58c|
    cout << formatMessage("|{:#012X}|",     { 1420 }) << "\n"; // |0X58C|
    cout << formatMessage("|{:#012h}|",     { 1420 }) << "\n"; // |58ch|
    cout << formatMessage("|{:!#0b}|",      { 1420 }) << "\n"; // |0B00000000000000000000010110001100|
    cout << formatMessage("|{:!#0x}|",      { 1420 }) << "\n"; // |0X0000058c|
    cout << formatMessage("|{:!#0X}|",      { 1420 }) << "\n"; // |0x0000058C|
    cout << formatMessage("|{:!#0h}|",      { 1420 }) << "\n"; // |0000058cH|
    cout << formatMessage("|{:!#0'b}|",     { 1420 }) << "\n"; // |0B0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:!#0'x}|",     { 1420 }) << "\n"; // |0X0000'058c|
    cout << formatMessage("|{:!#0'X}|",     { 1420 }) << "\n"; // |0x0000'058C|
    cout << formatMessage("|{:!#0'h}|",     { 1420 }) << "\n"; // |0000'058cH|
    cout << formatMessage("|{:!#12b}|",     { 1420 }) << "\n"; // |0B10110001100|
    cout << formatMessage("|{:!#12x}|",     { 1420 }) << "\n"; // |0X58c       |
    cout << formatMessage("|{:!#12X}|",     { 1420 }) << "\n"; // |0x58C       |
    cout << formatMessage("|{:!#12h}|",     { 1420 }) << "\n"; // |58cH        |
    cout << formatMessage("|{:!#12'b}|",    { 1420 }) << "\n"; // |0B101'1000'1100|
    cout << formatMessage("|{:!#012x}|",    { 1420 }) << "\n"; // |0X58c|
    cout << formatMessage("|{:!#012X}|",    { 1420 }) << "\n"; // |0x58C|
    cout << formatMessage("|{:!#012h}|",    { 1420 }) << "\n"; // |58cH|
    cout << formatMessage("|{:+0}|",        { 1420 }) << "\n"; // |+1420|
    cout << formatMessage("|{:+0b}|",       { 1420 }) << "\n"; // |+00000000000000000000010110001100|
    cout << formatMessage("|{:+0x}|",       { 1420 }) << "\n"; // |+0000058c|
    cout << formatMessage("|{:+0X}|",       { 1420 }) << "\n"; // |+0000058C|
    cout << formatMessage("|{:+0%}|",       { 1420 }) << "\n"; // |+1420%|
    cout << formatMessage("|{:+0'}|",       { 1420 }) << "\n"; // |+1'420|
    cout << formatMessage("|{:+0'b}|",      { 1420 }) << "\n"; // |+0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:+0'x}|",      { 1420 }) << "\n"; // |+0000'058c|
    cout << formatMessage("|{:+0'X}|",      { 1420 }) << "\n"; // |+0000'058C|
    cout << formatMessage("|{:+0'%}|",      { 1420 }) << "\n"; // |+1'420%|
    cout << formatMessage("|{:+12}|",       { 1420 }) << "\n"; // |+1420       |
    cout << formatMessage("|{:+12b}|",      { 1420 }) << "\n"; // |+10110001100|
    cout << formatMessage("|{:+12x}|",      { 1420 }) << "\n"; // |+58c        |
    cout << formatMessage("|{:+12X}|",      { 1420 }) << "\n"; // |+58C        |
    cout << formatMessage("|{:+12%}|",      { 1420 }) << "\n"; // |+1420%      |
    cout << formatMessage("|{:+12'}|",      { 1420 }) << "\n"; // |+1'420      |
    cout << formatMessage("|{:+12'b}|",     { 1420 }) << "\n"; // |+101'1000'1100|
    cout << formatMessage("|{:+12'%}|",     { 1420 }) << "\n"; // |+1'420%     |
    cout << formatMessage("|{:+#0b}|",      { 1420 }) << "\n"; // |+0b00000000000000000000010110001100|
    cout << formatMessage("|{:+#0x}|",      { 1420 }) << "\n"; // |+0x0000058c|
    cout << formatMessage("|{:+#0X}|",      { 1420 }) << "\n"; // |+0X0000058C|
    cout << formatMessage("|{:+#0h}|",      { 1420 }) << "\n"; // |+0000058ch|
    cout << formatMessage("|{:+#0'b}|",     { 1420 }) << "\n"; // |+0b0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:+#0'x}|",     { 1420 }) << "\n"; // |+0x0000'058c|
    cout << formatMessage("|{:+#0'X}|",     { 1420 }) << "\n"; // |+0X0000'058C|
    cout << formatMessage("|{:+#0'h}|",     { 1420 }) << "\n"; // |+0000'058ch|
    cout << formatMessage("|{:+#12b}|",     { 1420 }) << "\n"; // |+0b10110001100|
    cout << formatMessage("|{:+#12x}|",     { 1420 }) << "\n"; // |+0x58c      |
    cout << formatMessage("|{:+#12X}|",     { 1420 }) << "\n"; // |+0X58C      |
    cout << formatMessage("|{:+#12h}|",     { 1420 }) << "\n"; // |+58ch       |
    cout << formatMessage("|{:+#12'b}|",    { 1420 }) << "\n"; // |+0b101'1000'1100|
    cout << formatMessage("|{:+012x}|",     { 1420 }) << "\n"; // |+58c|
    cout << formatMessage("|{:+012X}|",     { 1420 }) << "\n"; // |+58C|
    cout << formatMessage("|{:+#012x}|",    { 1420 }) << "\n"; // |+0x58c|
    cout << formatMessage("|{:+#012X}|",    { 1420 }) << "\n"; // |+0X58C|
    cout << formatMessage("|{:+#012h}|",    { 1420 }) << "\n"; // |+58ch|
    cout << formatMessage("|{:+!#0b}|",     { 1420 }) << "\n"; // |+0B00000000000000000000010110001100|
    cout << formatMessage("|{:+!#0x}|",     { 1420 }) << "\n"; // |+0X0000058c|
    cout << formatMessage("|{:+!#0X}|",     { 1420 }) << "\n"; // |+0x0000058C|
    cout << formatMessage("|{:+!#0h}|",     { 1420 }) << "\n"; // |+0000058cH|
    cout << formatMessage("|{:+!#0'b}|",    { 1420 }) << "\n"; // |+0B0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:+!#0'x}|",    { 1420 }) << "\n"; // |+0X0000'058c|
    cout << formatMessage("|{:+!#0'X}|",    { 1420 }) << "\n"; // |+0x0000'058C|
    cout << formatMessage("|{:+!#0'h}|",    { 1420 }) << "\n"; // |+0000'058cH|
    cout << formatMessage("|{:+!#12b}|",    { 1420 }) << "\n"; // |+0B10110001100|
    cout << formatMessage("|{:+!#12x}|",    { 1420 }) << "\n"; // |+0X58c      |
    cout << formatMessage("|{:+!#12X}|",    { 1420 }) << "\n"; // |+0x58C      |
    cout << formatMessage("|{:+!#12h}|",    { 1420 }) << "\n"; // |+58cH       |
    cout << formatMessage("|{:+!#12'b}|",   { 1420 }) << "\n"; // |+0B101'1000'1100|
    cout << formatMessage("|{:+!#012x}|",   { 1420 }) << "\n"; // |+0X58c|
    cout << formatMessage("|{:+!#012X}|",   { 1420 }) << "\n"; // |+0x58C|
    cout << formatMessage("|{:+!#012h}|",   { 1420 }) << "\n"; // |+58cH|
    cout << formatMessage("|{:=12}|",       { 1420 }) << "\n"; // |        1420|
    cout << formatMessage("|{:=12b}|",      { 1420 }) << "\n"; // | 10110001100|
    cout << formatMessage("|{:=12x}|",      { 1420 }) << "\n"; // |         58c|
    cout << formatMessage("|{:=12X}|",      { 1420 }) << "\n"; // |         58C|
    cout << formatMessage("|{:=12%}|",      { 1420 }) << "\n"; // |       1420%|
    cout << formatMessage("|{:=12'}|",      { 1420 }) << "\n"; // |       1'420|
    cout << formatMessage("|{:=12'%}|",     { 1420 }) << "\n"; // |      1'420%|
    cout << formatMessage("|{:=12'n}|",     { 1420 }) << "\n"; // |       1 420|
    cout << formatMessage("|{:=#12x}|",     { 1420 }) << "\n"; // |       0x58c|
    cout << formatMessage("|{:=#12X}|",     { 1420 }) << "\n"; // |       0X58C|
    cout << formatMessage("|{:=#12h}|",     { 1420 }) << "\n"; // |        58ch|
    cout << formatMessage("|{:=!#12x}|",    { 1420 }) << "\n"; // |       0X58c|
    cout << formatMessage("|{:=!#12X}|",    { 1420 }) << "\n"; // |       0x58C|
    cout << formatMessage("|{:=!#12h}|",    { 1420 }) << "\n"; // |        58cH|
    cout << formatMessage("|{:=+12}|",      { 1420 }) << "\n"; // |+       1420|
    cout << formatMessage("|{:=+12x}|",     { 1420 }) << "\n"; // |+        58c|
    cout << formatMessage("|{:=+12X}|",     { 1420 }) << "\n"; // |+        58C|
    cout << formatMessage("|{:=+12%}|",     { 1420 }) << "\n"; // |+      1420%|
    cout << formatMessage("|{:=+12'}|",     { 1420 }) << "\n"; // |+      1'420|
    cout << formatMessage("|{:=+12'%}|",    { 1420 }) << "\n"; // |+     1'420%|
    cout << formatMessage("|{:=+#12x}|",    { 1420 }) << "\n"; // |+      0x58c|
    cout << formatMessage("|{:=+#12X}|",    { 1420 }) << "\n"; // |+      0X58C|
    cout << formatMessage("|{:=+#12h}|",    { 1420 }) << "\n"; // |+       58ch|
    cout << formatMessage("|{:=+!#12x}|",   { 1420 }) << "\n"; // |+      0X58c|
    cout << formatMessage("|{:=+!#12X}|",   { 1420 }) << "\n"; // |+      0x58C|
    cout << formatMessage("|{:=+!#12h}|",   { 1420 }) << "\n"; // |+       58cH|
    cout << formatMessage("|{:= 0}|",       { 1420 }) << "\n"; // | 1420|
    cout << formatMessage("|{:= 0b}|",      { 1420 }) << "\n"; // | 00000000000000000000010110001100|
    cout << formatMessage("|{:= 0x}|",      { 1420 }) << "\n"; // | 0000058c|
    cout << formatMessage("|{:= 0X}|",      { 1420 }) << "\n"; // | 0000058C|
    cout << formatMessage("|{:= 0%}|",      { 1420 }) << "\n"; // | 1420%|
    cout << formatMessage("|{:= 0'}|",      { 1420 }) << "\n"; // | 1'420|
    cout << formatMessage("|{:= 0'b}|",     { 1420 }) << "\n"; // | 0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:= 0'x}|",     { 1420 }) << "\n"; // | 0000'058c|
    cout << formatMessage("|{:= 0'X}|",     { 1420 }) << "\n"; // | 0000'058C|
    cout << formatMessage("|{:= 0'%}|",     { 1420 }) << "\n"; // | 1'420%|
    cout << formatMessage("|{:= 0'n}|",     { 1420 }) << "\n"; // | 1 420|
    cout << formatMessage("|{:= 12'b}|",    { 1420 }) << "\n"; // | 101'1000'1100|
    cout << formatMessage("|{:= #0b}|",     { 1420 }) << "\n"; // | 0b00000000000000000000010110001100|
    cout << formatMessage("|{:= #0x}|",     { 1420 }) << "\n"; // | 0x0000058c|
    cout << formatMessage("|{:= #0X}|",     { 1420 }) << "\n"; // | 0X0000058C|
    cout << formatMessage("|{:= #0h}|",     { 1420 }) << "\n"; // | 0000058ch|
    cout << formatMessage("|{:= #0'b}|",    { 1420 }) << "\n"; // | 0b0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:= #0'x}|",    { 1420 }) << "\n"; // | 0x0000'058c|
    cout << formatMessage("|{:= #0'X}|",    { 1420 }) << "\n"; // | 0X0000'058C|
    cout << formatMessage("|{:= #0'h}|",    { 1420 }) << "\n"; // | 0000'058ch|
    cout << formatMessage("|{:= #12b}|",    { 1420 }) << "\n"; // | 0b10110001100|
    cout << formatMessage("|{:= #12'b}|",   { 1420 }) << "\n"; // | 0b101'1000'1100|
    cout << formatMessage("|{:= 012x}|",    { 1420 }) << "\n"; // | 58c|
    cout << formatMessage("|{:= 012X}|",    { 1420 }) << "\n"; // | 58C|
    cout << formatMessage("|{:= #012x}|",   { 1420 }) << "\n"; // | 0x58c|
    cout << formatMessage("|{:= #012X}|",   { 1420 }) << "\n"; // | 0X58C|
    cout << formatMessage("|{:= #012h}|",   { 1420 }) << "\n"; // | 58ch|
    cout << formatMessage("|{:= !#0b}|",    { 1420 }) << "\n"; // | 0B00000000000000000000010110001100|
    cout << formatMessage("|{:= !#0x}|",    { 1420 }) << "\n"; // | 0X0000058c|
    cout << formatMessage("|{:= !#0X}|",    { 1420 }) << "\n"; // | 0x0000058C|
    cout << formatMessage("|{:= !#0h}|",    { 1420 }) << "\n"; // | 0000058cH|
    cout << formatMessage("|{:= !#0'b}|",   { 1420 }) << "\n"; // | 0B0000'0000'0000'0000'0000'0101'1000'1100|
    cout << formatMessage("|{:= !#0'x}|",   { 1420 }) << "\n"; // | 0X0000'058c|
    cout << formatMessage("|{:= !#0'X}|",   { 1420 }) << "\n"; // | 0x0000'058C|
    cout << formatMessage("|{:= !#0'h}|",   { 1420 }) << "\n"; // | 0000'058cH|
    cout << formatMessage("|{:= !#12b}|",   { 1420 }) << "\n"; // | 0B10110001100|
    cout << formatMessage("|{:= !#12'b}|",  { 1420 }) << "\n"; // | 0B101'1000'1100|
    cout << formatMessage("|{:= !#012x}|",  { 1420 }) << "\n"; // | 0X58c|
    cout << formatMessage("|{:= !#012X}|",  { 1420 }) << "\n"; // | 0x58C|
    cout << formatMessage("|{:= !#012h}|",  { 1420 }) << "\n"; // | 58cH|
    cout << formatMessage("|{:*=12}|",      { 1420 }) << "\n"; // |********1420|
    cout << formatMessage("|{:*=12b}|",     { 1420 }) << "\n"; // |*10110001100|
    cout << formatMessage("|{:*=12x}|",     { 1420 }) << "\n"; // |*********58c|
    cout << formatMessage("|{:*=12X}|",     { 1420 }) << "\n"; // |*********58C|
    cout << formatMessage("|{:*=12%}|",     { 1420 }) << "\n"; // |*******1420%|
    cout << formatMessage("|{:*=12'}|",     { 1420 }) << "\n"; // |*******1'420|
    cout << formatMessage("|{:*=12'%}|",    { 1420 }) << "\n"; // |******1'420%|
    cout << formatMessage("|{:*=12'n}|",    { 1420 }) << "\n"; // |*******1 420|
    cout << formatMessage("|{:*=#12x}|",    { 1420 }) << "\n"; // |*******0x58c|
    cout << formatMessage("|{:*=#12X}|",    { 1420 }) << "\n"; // |*******0X58C|
    cout << formatMessage("|{:*=#12h}|",    { 1420 }) << "\n"; // |********58ch|
    cout << formatMessage("|{:*=!#12x}|",   { 1420 }) << "\n"; // |*******0X58c|
    cout << formatMessage("|{:*=!#12X}|",   { 1420 }) << "\n"; // |*******0x58C|
    cout << formatMessage("|{:*=!#12h}|",   { 1420 }) << "\n"; // |********58cH|
    cout << formatMessage("|{:*=+12}|",     { 1420 }) << "\n"; // |+*******1420|
    cout << formatMessage("|{:*=+12x}|",    { 1420 }) << "\n"; // |+********58c|
    cout << formatMessage("|{:*=+12X}|",    { 1420 }) << "\n"; // |+********58C|
    cout << formatMessage("|{:*=+12%}|",    { 1420 }) << "\n"; // |+******1420%|
    cout << formatMessage("|{:*=+12'}|",    { 1420 }) << "\n"; // |+******1'420|
    cout << formatMessage("|{:*=+12'%}|",   { 1420 }) << "\n"; // |+*****1'420%|
    cout << formatMessage("|{:*=+#12x}|",   { 1420 }) << "\n"; // |+******0x58c|
    cout << formatMessage("|{:*=+#12X}|",   { 1420 }) << "\n"; // |+******0X58C|
    cout << formatMessage("|{:*=+#12h}|",   { 1420 }) << "\n"; // |+*******58ch|
    cout << formatMessage("|{:*=+!#12x}|",  { 1420 }) << "\n"; // |+******0X58c|
    cout << formatMessage("|{:*=+!#12X}|",  { 1420 }) << "\n"; // |+******0x58C|
    cout << formatMessage("|{:*=+!#12h}|",  { 1420 }) << "\n"; // |+*******58cH|
    cout << formatMessage("|{:*= 12}|",     { 1420 }) << "\n"; // | *******1420|
    cout << formatMessage("|{:*= 12x}|",    { 1420 }) << "\n"; // | ********58c|
    cout << formatMessage("|{:*= 12X}|",    { 1420 }) << "\n"; // | ********58C|
    cout << formatMessage("|{:*= 12%}|",    { 1420 }) << "\n"; // | ******1420%|
    cout << formatMessage("|{:*= 12'}|",    { 1420 }) << "\n"; // | ******1'420|
    cout << formatMessage("|{:*= 12'%}|",   { 1420 }) << "\n"; // | *****1'420%|
    cout << formatMessage("|{:*= 12'n}|",   { 1420 }) << "\n"; // | ******1 420|
    cout << formatMessage("|{:*= #12x}|",   { 1420 }) << "\n"; // | ******0x58c|
    cout << formatMessage("|{:*= #12X}|",   { 1420 }) << "\n"; // | ******0X58C|
    cout << formatMessage("|{:*= #12h}|",   { 1420 }) << "\n"; // | *******58ch|
    cout << formatMessage("|{:*= !#12x}|",  { 1420 }) << "\n"; // | ******0X58c|
    cout << formatMessage("|{:*= !#12X}|",  { 1420 }) << "\n"; // | ******0x58C|
    cout << formatMessage("|{:*= !#12h}|",  { 1420 }) << "\n"; // | *******58cH|
    cout << formatMessage("|{:< 12}|",      { 1420 }) << "\n"; // | 1420       |
    cout << formatMessage("|{:< 12x}|",     { 1420 }) << "\n"; // | 58c        |
    cout << formatMessage("|{:< 12X}|",     { 1420 }) << "\n"; // | 58C        |
    cout << formatMessage("|{:< 12%}|",     { 1420 }) << "\n"; // | 1420%      |
    cout << formatMessage("|{:< 12'}|",     { 1420 }) << "\n"; // | 1'420      |
    cout << formatMessage("|{:< 12'%}|",    { 1420 }) << "\n"; // | 1'420%     |
    cout << formatMessage("|{:< 12'n}|",    { 1420 }) << "\n"; // | 1 420      |
    cout << formatMessage("|{:< #12x}|",    { 1420 }) << "\n"; // | 0x58c      |
    cout << formatMessage("|{:< #12X}|",    { 1420 }) << "\n"; // | 0X58C      |
    cout << formatMessage("|{:< #12h}|",    { 1420 }) << "\n"; // | 58ch       |
    cout << formatMessage("|{:< !#12x}|",   { 1420 }) << "\n"; // | 0X58c      |
    cout << formatMessage("|{:< !#12X}|",   { 1420 }) << "\n"; // | 0x58C      |
    cout << formatMessage("|{:< !#12h}|",   { 1420 }) << "\n"; // | 58cH       |
    cout << formatMessage("|{:*<12}|",      { 1420 }) << "\n"; // |1420********|
    cout << formatMessage("|{:*<12b}|",     { 1420 }) << "\n"; // |10110001100*|
    cout << formatMessage("|{:*<12x}|",     { 1420 }) << "\n"; // |58c*********|
    cout << formatMessage("|{:*<12X}|",     { 1420 }) << "\n"; // |58C*********|
    cout << formatMessage("|{:*<12%}|",     { 1420 }) << "\n"; // |1420%*******|
    cout << formatMessage("|{:*<12'}|",     { 1420 }) << "\n"; // |1'420*******|
    cout << formatMessage("|{:*<12'%}|",    { 1420 }) << "\n"; // |1'420%******|
    cout << formatMessage("|{:*<12'n}|",    { 1420 }) << "\n"; // |1 420*******|
    cout << formatMessage("|{:*<#12x}|",    { 1420 }) << "\n"; // |0x58c*******|
    cout << formatMessage("|{:*<#12X}|",    { 1420 }) << "\n"; // |0X58C*******|
    cout << formatMessage("|{:*<#12h}|",    { 1420 }) << "\n"; // |58ch********|
    cout << formatMessage("|{:*<!#12x}|",   { 1420 }) << "\n"; // |0X58c*******|
    cout << formatMessage("|{:*<!#12X}|",   { 1420 }) << "\n"; // |0x58C*******|
    cout << formatMessage("|{:*<!#12h}|",   { 1420 }) << "\n"; // |58cH********|
    cout << formatMessage("|{:*<+12}|",     { 1420 }) << "\n"; // |+1420*******|
    cout << formatMessage("|{:*<+12x}|",    { 1420 }) << "\n"; // |+58c********|
    cout << formatMessage("|{:*<+12X}|",    { 1420 }) << "\n"; // |+58C********|
    cout << formatMessage("|{:*<+12%}|",    { 1420 }) << "\n"; // |+1420%******|
    cout << formatMessage("|{:*<+12'}|",    { 1420 }) << "\n"; // |+1'420******|
    cout << formatMessage("|{:*<+12'%}|",   { 1420 }) << "\n"; // |+1'420%*****|
    cout << formatMessage("|{:*<+#12x}|",   { 1420 }) << "\n"; // |+0x58c******|
    cout << formatMessage("|{:*<+#12X}|",   { 1420 }) << "\n"; // |+0X58C******|
    cout << formatMessage("|{:*<+#12h}|",   { 1420 }) << "\n"; // |+58ch*******|
    cout << formatMessage("|{:*<+!#12x}|",  { 1420 }) << "\n"; // |+0X58c******|
    cout << formatMessage("|{:*<+!#12X}|",  { 1420 }) << "\n"; // |+0x58C******|
    cout << formatMessage("|{:*<+!#12h}|",  { 1420 }) << "\n"; // |+58cH*******|
    cout << formatMessage("|{:*< 12}|",     { 1420 }) << "\n"; // | 1420*******|
    cout << formatMessage("|{:*< 12x}|",    { 1420 }) << "\n"; // | 58c********|
    cout << formatMessage("|{:*< 12X}|",    { 1420 }) << "\n"; // | 58C********|
    cout << formatMessage("|{:*< 12%}|",    { 1420 }) << "\n"; // | 1420%******|
    cout << formatMessage("|{:*< 12'}|",    { 1420 }) << "\n"; // | 1'420******|
    cout << formatMessage("|{:*< 12'%}|",   { 1420 }) << "\n"; // | 1'420%*****|
    cout << formatMessage("|{:*< 12'n}|",   { 1420 }) << "\n"; // | 1 420******|
    cout << formatMessage("|{:*< #12x}|",   { 1420 }) << "\n"; // | 0x58c******|
    cout << formatMessage("|{:*< #12X}|",   { 1420 }) << "\n"; // | 0X58C******|
    cout << formatMessage("|{:*< #12h}|",   { 1420 }) << "\n"; // | 58ch*******|
    cout << formatMessage("|{:*< !#12x}|",  { 1420 }) << "\n"; // | 0X58c******|
    cout << formatMessage("|{:*< !#12X}|",  { 1420 }) << "\n"; // | 0x58C******|
    cout << formatMessage("|{:*< !#12h}|",  { 1420 }) << "\n"; // | 58cH*******|
    cout << formatMessage("|{:>+12}|",      { 1420 }) << "\n"; // |       +1420|
    cout << formatMessage("|{:>+12x}|",     { 1420 }) << "\n"; // |        +58c|
    cout << formatMessage("|{:>+12X}|",     { 1420 }) << "\n"; // |        +58C|
    cout << formatMessage("|{:>+12%}|",     { 1420 }) << "\n"; // |      +1420%|
    cout << formatMessage("|{:>+12'}|",     { 1420 }) << "\n"; // |      +1'420|
    cout << formatMessage("|{:>+12'%}|",    { 1420 }) << "\n"; // |     +1'420%|
    cout << formatMessage("|{:>+#12x}|",    { 1420 }) << "\n"; // |      +0x58c|
    cout << formatMessage("|{:>+#12X}|",    { 1420 }) << "\n"; // |      +0X58C|
    cout << formatMessage("|{:>+#12h}|",    { 1420 }) << "\n"; // |       +58ch|
    cout << formatMessage("|{:>+!#12x}|",   { 1420 }) << "\n"; // |      +0X58c|
    cout << formatMessage("|{:>+!#12X}|",   { 1420 }) << "\n"; // |      +0x58C|
    cout << formatMessage("|{:>+!#12h}|",   { 1420 }) << "\n"; // |       +58cH|
    cout << formatMessage("|{:*>+12}|",     { 1420 }) << "\n"; // |*******+1420|
    cout << formatMessage("|{:*>+12x}|",    { 1420 }) << "\n"; // |********+58c|
    cout << formatMessage("|{:*>+12X}|",    { 1420 }) << "\n"; // |********+58C|
    cout << formatMessage("|{:*>+12%}|",    { 1420 }) << "\n"; // |******+1420%|
    cout << formatMessage("|{:*>+12'}|",    { 1420 }) << "\n"; // |******+1'420|
    cout << formatMessage("|{:*>+12'%}|",   { 1420 }) << "\n"; // |*****+1'420%|
    cout << formatMessage("|{:*>+#12x}|",   { 1420 }) << "\n"; // |******+0x58c|
    cout << formatMessage("|{:*>+#12X}|",   { 1420 }) << "\n"; // |******+0X58C|
    cout << formatMessage("|{:*>+#12h}|",   { 1420 }) << "\n"; // |*******+58ch|
    cout << formatMessage("|{:*>+!#12x}|",  { 1420 }) << "\n"; // |******+0X58c|
    cout << formatMessage("|{:*>+!#12X}|",  { 1420 }) << "\n"; // |******+0x58C|
    cout << formatMessage("|{:*>+!#12h}|",  { 1420 }) << "\n"; // |*******+58cH|
    cout << formatMessage("|{:*> 12}|",     { 1420 }) << "\n"; // |******* 1420|
    cout << formatMessage("|{:*> 12x}|",    { 1420 }) << "\n"; // |******** 58c|
    cout << formatMessage("|{:*> 12X}|",    { 1420 }) << "\n"; // |******** 58C|
    cout << formatMessage("|{:*> 12%}|",    { 1420 }) << "\n"; // |****** 1420%|
    cout << formatMessage("|{:*> 12'}|",    { 1420 }) << "\n"; // |****** 1'420|
    cout << formatMessage("|{:*> 12'%}|",   { 1420 }) << "\n"; // |***** 1'420%|
    cout << formatMessage("|{:*> 12'n}|",   { 1420 }) << "\n"; // |****** 1 420|
    cout << formatMessage("|{:*> #12x}|",   { 1420 }) << "\n"; // |****** 0x58c|
    cout << formatMessage("|{:*> #12X}|",   { 1420 }) << "\n"; // |****** 0X58C|
    cout << formatMessage("|{:*> #12h}|",   { 1420 }) << "\n"; // |******* 58ch|
    cout << formatMessage("|{:*> !#12x}|",  { 1420 }) << "\n"; // |****** 0X58c|
    cout << formatMessage("|{:*> !#12X}|",  { 1420 }) << "\n"; // |****** 0x58C|
    cout << formatMessage("|{:*> !#12h}|",  { 1420 }) << "\n"; // |******* 58cH|
    cout << formatMessage("|{:^12}|",       { 1420 }) << "\n"; // |    1420    |
    cout << formatMessage("|{:^12x}|",      { 1420 }) << "\n"; // |    58c     |
    cout << formatMessage("|{:^12X}|",      { 1420 }) << "\n"; // |    58C     |
    cout << formatMessage("|{:^12%}|",      { 1420 }) << "\n"; // |   1420%    |
    cout << formatMessage("|{:^12'}|",      { 1420 }) << "\n"; // |   1'420    |
    cout << formatMessage("|{:^12'%}|",     { 1420 }) << "\n"; // |   1'420%   |
    cout << formatMessage("|{:^12'n}|",     { 1420 }) << "\n"; // |   1 420    |
    cout << formatMessage("|{:^#12x}|",     { 1420 }) << "\n"; // |   0x58c    |
    cout << formatMessage("|{:^#12X}|",     { 1420 }) << "\n"; // |   0X58C    |
    cout << formatMessage("|{:^#12h}|",     { 1420 }) << "\n"; // |    58ch    |
    cout << formatMessage("|{:^!#12x}|",    { 1420 }) << "\n"; // |   0X58c    |
    cout << formatMessage("|{:^!#12X}|",    { 1420 }) << "\n"; // |   0x58C    |
    cout << formatMessage("|{:^!#12h}|",    { 1420 }) << "\n"; // |    58cH    |
    cout << formatMessage("|{:^+12}|",      { 1420 }) << "\n"; // |   +1420    |
    cout << formatMessage("|{:^+12x}|",     { 1420 }) << "\n"; // |    +58c    |
    cout << formatMessage("|{:^+12X}|",     { 1420 }) << "\n"; // |    +58C    |
    cout << formatMessage("|{:^+12%}|",     { 1420 }) << "\n"; // |   +1420%   |
    cout << formatMessage("|{:^+12'}|",     { 1420 }) << "\n"; // |   +1'420   |
    cout << formatMessage("|{:^+12'%}|",    { 1420 }) << "\n"; // |  +1'420%   |
    cout << formatMessage("|{:^+#12x}|",    { 1420 }) << "\n"; // |   +0x58c   |
    cout << formatMessage("|{:^+#12X}|",    { 1420 }) << "\n"; // |   +0X58C   |
    cout << formatMessage("|{:^+#12h}|",    { 1420 }) << "\n"; // |   +58ch    |
    cout << formatMessage("|{:^+!#12x}|",   { 1420 }) << "\n"; // |   +0X58c   |
    cout << formatMessage("|{:^+!#12X}|",   { 1420 }) << "\n"; // |   +0x58C   |
    cout << formatMessage("|{:^+!#12h}|",   { 1420 }) << "\n"; // |   +58cH    |
    cout << formatMessage("|{:^ 12x}|",     { 1420 }) << "\n"; // |     58c    |
    cout << formatMessage("|{:^ 12X}|",     { 1420 }) << "\n"; // |     58C    |
    cout << formatMessage("|{:^ 12%}|",     { 1420 }) << "\n"; // |    1420%   |
    cout << formatMessage("|{:^ 12'}|",     { 1420 }) << "\n"; // |    1'420   |
    cout << formatMessage("|{:^ 12'n}|",    { 1420 }) << "\n"; // |    1 420   |
    cout << formatMessage("|{:^ #12x}|",    { 1420 }) << "\n"; // |    0x58c   |
    cout << formatMessage("|{:^ #12X}|",    { 1420 }) << "\n"; // |    0X58C   |
    cout << formatMessage("|{:^ !#12x}|",   { 1420 }) << "\n"; // |    0X58c   |
    cout << formatMessage("|{:^ !#12X}|",   { 1420 }) << "\n"; // |    0x58C   |
    cout << formatMessage("|{:*^12}|",      { 1420 }) << "\n"; // |****1420****|
    cout << formatMessage("|{:*^12x}|",     { 1420 }) << "\n"; // |****58c*****|
    cout << formatMessage("|{:*^12X}|",     { 1420 }) << "\n"; // |****58C*****|
    cout << formatMessage("|{:*^12%}|",     { 1420 }) << "\n"; // |***1420%****|
    cout << formatMessage("|{:*^12'}|",     { 1420 }) << "\n"; // |***1'420****|
    cout << formatMessage("|{:*^12'%}|",    { 1420 }) << "\n"; // |***1'420%***|
    cout << formatMessage("|{:*^12'n}|",    { 1420 }) << "\n"; // |***1 420****|
    cout << formatMessage("|{:*^#12x}|",    { 1420 }) << "\n"; // |***0x58c****|
    cout << formatMessage("|{:*^#12X}|",    { 1420 }) << "\n"; // |***0X58C****|
    cout << formatMessage("|{:*^#12h}|",    { 1420 }) << "\n"; // |****58ch****|
    cout << formatMessage("|{:*^!#12x}|",   { 1420 }) << "\n"; // |***0X58c****|
    cout << formatMessage("|{:*^!#12X}|",   { 1420 }) << "\n"; // |***0x58C****|
    cout << formatMessage("|{:*^!#12h}|",   { 1420 }) << "\n"; // |****58cH****|
    cout << formatMessage("|{:*^+12}|",     { 1420 }) << "\n"; // |***+1420****|
    cout << formatMessage("|{:*^+12x}|",    { 1420 }) << "\n"; // |****+58c****|
    cout << formatMessage("|{:*^+12X}|",    { 1420 }) << "\n"; // |****+58C****|
    cout << formatMessage("|{:*^+12%}|",    { 1420 }) << "\n"; // |***+1420%***|
    cout << formatMessage("|{:*^+12'}|",    { 1420 }) << "\n"; // |***+1'420***|
    cout << formatMessage("|{:*^+12'%}|",   { 1420 }) << "\n"; // |**+1'420%***|
    cout << formatMessage("|{:*^+#12x}|",   { 1420 }) << "\n"; // |***+0x58c***|
    cout << formatMessage("|{:*^+#12X}|",   { 1420 }) << "\n"; // |***+0X58C***|
    cout << formatMessage("|{:*^+#12h}|",   { 1420 }) << "\n"; // |***+58ch****|
    cout << formatMessage("|{:*^+!#12x}|",  { 1420 }) << "\n"; // |***+0X58c***|
    cout << formatMessage("|{:*^+!#12X}|",  { 1420 }) << "\n"; // |***+0x58C***|
    cout << formatMessage("|{:*^+!#12h}|",  { 1420 }) << "\n"; // |***+58cH****|
    cout << formatMessage("|{:*^ 12}|",     { 1420 }) << "\n"; // |*** 1420****|
    cout << formatMessage("|{:*^ 12x}|",    { 1420 }) << "\n"; // |**** 58c****|
    cout << formatMessage("|{:*^ 12X}|",    { 1420 }) << "\n"; // |**** 58C****|
    cout << formatMessage("|{:*^ 12%}|",    { 1420 }) << "\n"; // |*** 1420%***|
    cout << formatMessage("|{:*^ 12'}|",    { 1420 }) << "\n"; // |*** 1'420***|
    cout << formatMessage("|{:*^ 12'%}|",   { 1420 }) << "\n"; // |** 1'420%***|
    cout << formatMessage("|{:*^ 12'n}|",   { 1420 }) << "\n"; // |*** 1 420***|
    cout << formatMessage("|{:*^ #12x}|",   { 1420 }) << "\n"; // |*** 0x58c***|
    cout << formatMessage("|{:*^ #12X}|",   { 1420 }) << "\n"; // |*** 0X58C***|
    cout << formatMessage("|{:*^ #12h}|",   { 1420 }) << "\n"; // |*** 58ch****|
    cout << formatMessage("|{:*^ !#12x}|",  { 1420 }) << "\n"; // |*** 0X58c***|
    cout << formatMessage("|{:*^ !#12X}|",  { 1420 }) << "\n"; // |*** 0x58C***|
    cout << formatMessage("|{:*^ !#12h}|",  { 1420 }) << "\n"; // |*** 58cH****|

}


