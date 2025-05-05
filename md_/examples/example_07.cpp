/*! \file
    \brief Тестируем форматирование чисел - проценты, валюта, с использованием локали и без
 */


#include "marty_format/marty_format.h"

//
#include <iostream>

using std::cout;
using std::cerr;


inline
std::string escapeFormatStr(const std::string &fmt)
{
    std::string res; res.reserve(fmt.size());

    for(auto ch: fmt)
    {
        if (ch=='{' || ch=='}')
            res.append(1, ch);
        res.append(1, ch);
    }
    return res;
}

inline
std::string simpleFirstArgToString(const marty::format::Args &args)
{
    if (args.empty())
        return "<ARGS_EMPTY>";

    auto it = args.find_by_pos(0);
    if (it==args.end())
        return "<ARG_NOT_FOUND>";

    const auto &val = *it;

    return std::visit( [](auto && a) -> std::string
                       {
                           return marty::format::martyFormatSimpleConvertToString<std::string>(a);
                       }
                     , val
                     );
}



inline
bool simpleTestImpl(std::size_t testNo, bool doTest, const std::string &expected, const std::string &fmt, const marty::format::Args &args)
{
    std::string strTestNo = marty::format::utils::simpleToString(testNo);

    auto strTestNoExpanded = marty::format::utils::expandBeforeUpTo(strTestNo, std::size_t(4u), char(' '));

    auto fmtPrepared = "{:"+fmt+"}";
    auto strResult   = marty::format::formatMessage(fmtPrepared, args);
    auto fmtExpanded = marty::format::utils::expandAfterUpTo(fmtPrepared, std::size_t(15u), char(' '));
    
    auto argValStr   = simpleFirstArgToString(args);
    auto argExpanded = marty::format::utils::expandAfterUpTo(argValStr, std::size_t(15u), char(' '));

    bool resAsExpected = (expected==strResult);

    // marty::format::utils::expandAfter("|"+fmt+"|", 12, ' ');
    //  
    //  
    // auto fmtExpanded = marty::format::utils::expandAfter(fmt, 12, ' ');
    // marty::format::

    if (doTest)
        std::cout << (resAsExpected ? "+" : "-") << strTestNoExpanded << ": ";

    std::cout << fmtExpanded << " with  " << argExpanded << ": " << "|" << strResult << "|";

    if (doTest)
    {
        if (resAsExpected)
            std::cout << " - test passed";
        else
            std::cout << ", but expected: |" << expected << "| - test failed";
    }

    std::cout << "\n";
    
    return resAsExpected;

}

inline
bool simpleTest(std::size_t &testNo, std::size_t &passedCounter, bool doTest, const std::string &expected, const std::string &fmt, const marty::format::Args &args)
{
    auto res = simpleTestImpl(++testNo, doTest, expected, fmt, args);
    if (res)
        ++passedCounter;
    return res;
}


inline
void test(bool doTest=false)
{
    using marty::format::Args;

    std::size_t nPassed = 0;
    std::size_t nTotal  = 0;

    double dPi = 3.1415926535897932384626433832795;
    // double dE  = 2.7182818284590452353602874713527;

    // simpleTest(nTotal, nPassed, doTest, " 003.14159"               , " 010.05f" , Args().arg(dPi)  );
    // simpleTest(nTotal, nPassed, doTest, "2.71828"                   , "09'.05f"   , Args().arg( dE ) );
    // std::cout << "\n";


    std::cout << "//---------------------\n";
    std::cout << "// n - десятичное число, форматирование с использованием локали.\n"
                 "// Точность не задана, берётся из текущей локали пользователя\n"
                 "// (русская под Win - 2 десятичных знака)\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "n"         , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// n - десятичное число, форматирование с использованием локали.\n"
                 "// Точность задана явно, .4.\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , ".4n"       , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// n - десятичное число, форматирование с использованием локали.\n"
                 "// Точность задана явно, .6.\n"
                 "// Указано разделение на разряды дробной части.\n"
                 "// Разделитель используется из локали пользователя в ОС.\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , ".6_n"       , Args().arg( dPi) );
    std::cout << "\n";


    std::cout << "//---------------------\n";
    std::cout << "// d - число. Точность не задана, берётся из локали\n"
                 "// (дефолтная \"C\"/invariant локаль - 3 символа)\n";
    simpleTest(nTotal, nPassed, doTest, "3.142"                     , "d"         , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// d - число. Точность не задана, берётся из пользовательской локали\n"
                 "// (русская под Win - 2 десятичных знака)\n"
                 "// Маркер локали 'L' указывает использовать локаль из ОС\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "Ld"        , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// d - число. Точность не задана, альтер маркер '#' указывает\n"
                 "// выбрать точность автоматически\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "#d"        , Args().arg( dPi) );
    std::cout << "\n";


    std::cout << "//---------------------\n";
    std::cout << "// % - проценты - число умножается на 100.\n"
                 "// Точность не задана, берётся из invariant локали - 3 символа\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "%"         , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// % - проценты - число умножается на 100.\n"
                 "// Точность не задана, берётся из пользовательской локали - 2 символа\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "L%"        , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// % - проценты, альтернативная форма, знак '%' опускается, invariant локаль, .3.\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "#%"        , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// % - проценты, альтернативная форма, знак '%' опускается, пользовательская локаль, .2.\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "#L%"       , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// % - проценты, точность задаётся явно.\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , ".4%"       , Args().arg( dPi) );
    std::cout << "\n";


    std::cout << "//---------------------\n";
    std::cout << "// $ - валюта.\n"
                 "// Точность не задана, берётся из invariant локали - 2 символа\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "$"         , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// $ - валюта.\n"
                 "// Точность не задана, берётся из пользовательской локали - 2 символа\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "L$"        , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// $ - валюта, альтернативная форма, знак валюты опускается, invariant локаль, .2.\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "#$"        , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// $ - валюта, альтернативная форма, знак '$' опускается, пользовательская локаль, .2.\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "#L$"       , Args().arg( dPi) );
    std::cout << "\n";

    std::cout << "// $ - валюта, точность задаётся явно.\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , ".4$"       , Args().arg( dPi) );
    std::cout << "\n";


    std::cout << "\n";
    std::cout << "//---------------------\n";
    std::cout << "// Краткий сводный пример форматирования чисел, процентов, валюты.\n";
    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "n"         , Args().arg( dPi) );
    std::cout << "\n";
    std::cout << "// 9 знаков после запятой, разбиение дробной части по разрядам.\n";
    std::cout << "// Используется локаль пользователя системы.\n";
    std::cout << "// Разделитель - неразрывный пробел (&nbsp;) - локаль RU-ru.\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , ".9'n"      , Args().arg( dPi) );
    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "d"         , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , ".9'd"      , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "f"         , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , ".9'f"      , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "%"         , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "L%"        , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "#L%"       , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "$"         , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "L$"        , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14"                      , "#L$"       , Args().arg( dPi) );

    std::cout << "\n";
}


int main(int argc)
{
    try
    {
        // using namespace marty::format;
        // std::cout << "f'.05': " << formatMessage("|{:'.05'f}|", Args().arg(3.14E-15)) << "\n";

        test(argc>1 ? true : false);
    }
    catch(const std::exception &e)
    {
        std::cout << "Exception catched: " << e.what() << "\n";
    }

    return 0;
}

