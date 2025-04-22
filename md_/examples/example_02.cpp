/*! \file
    \brief 
 */


#include "marty_format/marty_format.h"

//
#include <iostream>

using std::cout;
using std::cerr;

int main(int argc, char* argv[])
{

    (void)(argc);
    (void)(argv);

    #if 0
    {
        using std::cout;
        using namespace marty::format;

        cout << formatMessage( //---
                               "|{str1:?20.13}|\n"
                               "|{str1:*<20.13}|\n"
                               //---
                             , Args().arg("str1", "String len more than 16")
                             );
        //#!
    }
    cout << "\n\n\n" << std::flush;
    #endif


    //----------------------------------------------------------------------------

    #if 0
    cout << "//#! FormattingBoolTest\n";
    {
        //#! FormattingBoolTest
        using std::cout;
        using namespace marty::format;
        cout << formatMessage( "{:!s}, {:!s}\n"
                             , Args().arg(true)
                                     .arg(false)
                             );
        //#!
    }
    cout << "\n\n\n" << std::flush;
    #endif

    //----------------------------------------------------------------------------

    cout << "//#! UsingArgs\n";
    {
        // Автоматически вычисляемый индекс аргумента
        // Ширину и точность (на самом деле макс ширину строки)
        // задаём также аргументами, а не в форматной строке
        //#! UsingArgs
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
        //#!
    }
    cout << "\n\n\n" << std::flush;

    //----------------------------------------------------------------------------

    cout << "//#! StringWidthPrecision01\n";
    {
        //#! StringWidthPrecision01
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
                               "{adname:{anamew}} aligned: |{strL20:*20.13}|\n"
                               "{alname:{anamew}} aligned: |{strL20:*<20.13}|\n"
                               "{arname:{anamew}} aligned: |{strL20:*>20.13}|\n"
                               "{acname:{anamew}} aligned: |{strL20:*^20.13}|\n"
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
                               "{adname:{anamew}} aligned: |{strL20:*{w1}.{p1}}|\n"
                               "{alname:{anamew}} aligned: |{strL20:*<{w1}.{p1}}|\n"
                               "{arname:{anamew}} aligned: |{strL20:*>{w1}.{p1}}|\n"
                               "{acname:{anamew}} aligned: |{strL20:*^{w1}.{p1}}|\n"
                               "//---\n"
                               "Align: indirect, Width (I): {w1}, precision (I): {p1}\n"
                               "str: |{strL20}|\n"
                               "{adname:{anamew}} aligned: |{strL20:*{w1}.{p1}}|\n"
                               "{alname:{anamew}} aligned: |{strL20:*{al}{w1}.{p1}}|\n"
                               "{arname:{anamew}} aligned: |{strL20:*{ar}{w1}.{p1}}|\n"
                               "{acname:{anamew}} aligned: |{strL20:*{ac}{w1}.{p1}}|\n"
                               // "//---\n"
                             , Args().arg("anamew", 8)            // alignment name width
                                     .arg("adname", "Default")    // name for default alignment
                                     .arg("alname", "Left")       // name for left alignment
                                     .arg("arname", "Right")      // name for right alignment
                                     .arg("acname", "Center")     // name for center alignment
                                     //---
                                     .arg("w1", 16).arg("p1", 13) // indirect width & precision #1
                                     .arg("al", '<').arg("ar", '>').arg("ac", '^') // indirect align
                                     //---
                                     .arg("strL20", "String larger than 20")
                                     .arg("strS19", "Str smaler than 19")
                                     .arg("str<4" , "S<4")
                                     .arg("str>5" , "Str > 5")
                             );
        //#!
    }
    cout << "\n\n\n" << std::flush;

    //----------------------------------------------------------------------------

    cout << "//#! FormattingBool\n";
    {
        // Примеры форматирования bool
        //#! FormattingBool
        using std::cout;
        using namespace marty::format;
        cout << formatMessage( "Bool as string, true : {bt:s}, false: {bf:s}\n"
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
                             , Args().arg("bt", true)
                                     .arg("bf", false)
                                     .arg("ut", 1u)
                                     .arg("uf", 0u)
                                     .arg("st", -1)
                                     .arg("sf",  0)
                             );
        //#!
    }
    cout << "\n\n\n" << std::flush;

    //----------------------------------------------------------------------------

    cout << "//#! UsingHtmlFilters\n";
    {
        // Используем стандартные встроенные фильтры для вывода в HTML
        //#! UsingHtmlFilters
        using std::cout;
        using namespace marty::format;

        cout << formatMessage( "<a href=\"{url|html-attr}\">{text|html-text}</a>\n"
                             , Args().arg("url" , "http://site.com/&q=/'")
                                     .arg("text", "<Some & text>")
                             );
        //#!
    }
    cout << "\n\n\n" << std::flush;

    //----------------------------------------------------------------------------

    cout << "//#! UsingInitializerList\n";
    {
        // Автоматически вычисляемый индекс аргумента, используем std::initializer_list
        // Перевод строки отдельно выводим
        //#! UsingInitializerList
        using std::cout;
        using namespace marty::format;
        cout << formatMessage("Integer number: {:d}, string: {:{}.{}s}, Pi: {:f}"
                             , { 10, "Very long string, does not fit into 20 characters"
                               , 10, 20, 3.14159
                               }
                             ) << "\n";
        //#!
    }
    cout << "\n\n\n" << std::flush;

    //----------------------------------------------------------------------------

    cout << "//#! UsingFlatVector\n";
    {
        // Автоматически вычисляемый индекс аргумента, используем std::vector
        // Перевод строки отдельно выводим
        //#! UsingFlatVector
        using std::cout;
        using namespace marty::format;
        auto argsVec = std::vector<FormatArgumentVariant>{ 10, "Very long string, "
                              "does not fit into 20 characters", 10, 20, 3.14159 };
        cout << formatMessage( "Integer number: {:d}, string: {:{}.{}s}, Pi: {:f}"
                             , argsVec
                             ) << "\n";
        //#!
    }
    cout << "\n\n\n" << std::flush;

    //----------------------------------------------------------------------------

    cout << "//#! UsingNamedArgs\n";
    {
        // Используем именованные аргументы,
        // перемешали порядок аргументов, по сравнению с предыдущим примером
        //#! UsingNamedArgs
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
        //#!
    }
    cout << "\n\n\n" << std::flush;

    //----------------------------------------------------------------------------

    cout << "//#! UsingVectorOfNameValuePairs\n";
    {
        // Используем std::vector вместо marty::format::Args
        // Тут поиск по имени не такой эффективный, простым перебором, но тоже работает
        // Готовим вектор заранее
        //#! UsingVectorOfNameValuePairs
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
        //#!
    }
    cout << "\n\n\n" << std::flush;

    //----------------------------------------------------------------------------

    cout << "//#! UsingFillRefAndConvert\n";
    {
        // Используем именованные аргументы
        // Задаём конвертацию значения (но в текущий момент она игнорируется)
        // Символ заполнения передаём аргументом
        //#! UsingFillRefAndConvert
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
        //#!
    }
    cout << "\n\n\n" << std::flush;

    //----------------------------------------------------------------------------

    
    return 0;
}


