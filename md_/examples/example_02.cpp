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

    {
        //#! UsingArgs
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
        //#!
    }

    {
        //#! UsingInitializerList
        // Автоматически вычисляемый индекс аргумента, используем std::initializer_list
        // Перевод строки отдельно выводим
        using namespace marty::format;
        cout << formatMessage("Integer number: {:d}, string: {:{}.{}s}, Pi: {:f}"
                             , { 10, "Very long string, does not fit into 20 characters"
                               , 10, 20, 3.14159
                               }
                             ) << "\n";
        //#!
    }

    {
        //#! UsingFlatVector
        // Автоматически вычисляемый индекс аргумента, используем std::vector
        // Перевод строки отдельно выводим
        using namespace marty::format;
        auto argsVec = std::vector<FormatArgumentVariant>{ 10, "Very long string, "
                              "does not fit into 20 characters", 10, 20, 3.14159 };
        cout << formatMessage( "Integer number: {:d}, string: {:{}.{}s}, Pi: {:f}"
                             , argsVec
                             ) << "\n";
        //#!
    }

    {
        //#! UsingNamedArgs
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
        //#!
    }

    {
        //#! UsingVectorOfNameValuePairs
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
                              "Pi: {Pi:f}\n", argsVec)
             << "\n";
        //#!
    }
    
    return 0;
}


