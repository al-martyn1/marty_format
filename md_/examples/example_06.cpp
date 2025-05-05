/*! \file
    \brief Тестируем форматирование чисел с плавающей точкой (используем форматный символ 'f')
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
    double dE  = 2.7182818284590452353602874713527;

    // simpleTest(nTotal, nPassed, doTest, " 003.14159"               , " 010.05f" , Args().arg(dPi)  );
    // simpleTest(nTotal, nPassed, doTest, "2.71828"                   , "09'.05f"   , Args().arg( dE ) );
    // std::cout << "\n";

    simpleTest(nTotal, nPassed, doTest, "3.14159"                   , ".05f"      , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14159265358979311600"    , ".020f"     , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "-3.14159"                  , ".05f"      , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-3.14159265358979311600"   , ".020f"     , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14159"                   , ".5f"       , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14159265358979311600"    , ".20f"      , Args().arg( dPi) );

    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, "003.14159"                 , "09.05f"    , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "-003.14159"                , "010.05f"   , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-003.14159"                , "010.05f"   , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "+003.14159"                , "+010.05f"  , Args().arg( dPi) );

    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, " 003.14159"                , "= 010.05f" , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "+003.14159"                , "=+010.05f" , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "0003.14159"                , "=-010.05f" , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "-003.14159"                , "= 010.05f" , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-003.14159"                , "=+010.05f" , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-003.14159"                , "=-010.05f" , Args().arg(-dPi) );

    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, "   3.14159"                , "= 10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "+  3.14159"                , "=+10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "   3.14159"                , "=-10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "-  3.14159"                , "= 10.05f"  , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-  3.14159"                , "=+10.05f"  , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-  3.14159"                , "=-10.05f"  , Args().arg(-dPi) );

    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, "   3.14159"                , "= 10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "+  3.14159"                , "=+10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "   3.14159"                , "=-10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "-  3.14159"                , "= 10.05f"  , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-  3.14159"                , "=+10.05f"  , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-  3.14159"                , "=-10.05f"  , Args().arg(-dPi) );

    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, " 3.14159  "                , "< 10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "+3.14159  "                , "<+10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "3.14159   "                , "<-10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "-3.14159  "                , "< 10.05f"  , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-3.14159  "                , "<+10.05f"  , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "-3.14159  "                , "<-10.05f"  , Args().arg(-dPi) );

    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, "   3.14159"                , "> 10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "  +3.14159"                , ">+10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "   3.14159"                , ">-10.05f"  , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, "  -3.14159"                , "> 10.05f"  , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "  -3.14159"                , ">+10.05f"  , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, "  -3.14159"                , ">-10.05f"  , Args().arg(-dPi) );

    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, "  3.14159 "                , "^ 010.05f" , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, " +3.14159 "                , "^+010.05f" , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, " 3.14159  "                , "^-010.05f" , Args().arg( dPi) );
    simpleTest(nTotal, nPassed, doTest, " -3.14159 "                , "^ 010.05f" , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, " -3.14159 "                , "^+010.05f" , Args().arg(-dPi) );
    simpleTest(nTotal, nPassed, doTest, " -3.14159 "                , "^-010.05f" , Args().arg(-dPi) );

    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, "2.71828"                   , ".05f"      , Args().arg( dE ) );
    simpleTest(nTotal, nPassed, doTest, "2.71828182845904509080"    , ".020f"     , Args().arg( dE ) );

    std::cout << "\n";
    simpleTest(nTotal, nPassed, doTest, "00'002.71828"              , "012'.05f"   , Args().arg( dE ) );
    simpleTest(nTotal, nPassed, doTest, "2.718281828459045091"      , "012'.018f"  , Args().arg( dE ) );
    simpleTest(nTotal, nPassed, doTest, "0'002.718'28"              , "012'.05'f"  , Args().arg( dE ) );
    simpleTest(nTotal, nPassed, doTest, "2.718'281'828'459'045'091" , "012'.018'f" , Args().arg( dE ) );
    simpleTest(nTotal, nPassed, doTest, "0'002.718'28"              , "012'.5'f"   , Args().arg( dE ) );
    simpleTest(nTotal, nPassed, doTest, "2.718'281'828'459'045'091" , "012'.18'f"  , Args().arg( dE ) );



#if 0
    using namespace marty::format;

    double vals[8] = { 3.14159E-15
                     , 0.0000001
                     , 3.1415926535
                     , 271828182845904523536028747135266249775724709369995.0
                     , -3.14159E-15
                     , -0.0000001
                     , -3.1415926535
                     , -271828182845904523536028747135266249775724709369995.0
                     };
    for(auto i=0; i!=8; ++i)
    {
        std::cout << "----------------------------------------------------------------------------\n";
        std::cout << vals[i] << "\n";
        std::cout << "------------------------------\n";

        std::cout << "a'.05': " << formatMessage("|{:'.05'a}|", Args().arg(vals[i])) << "\n";
        std::cout << "A'.05': " << formatMessage("|{:'.05'A}|", Args().arg(vals[i])) << "\n";
        std::cout << "a'.5' : " << formatMessage("|{:'.5a}|" , Args().arg(vals[i])) << "\n";
        std::cout << "A'.5' : " << formatMessage("|{:'.5'A}|", Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "a.05: " << formatMessage("|{:.05a}|", Args().arg(vals[i])) << "\n";
        std::cout << "A.05: " << formatMessage("|{:.05A}|", Args().arg(vals[i])) << "\n";
        std::cout << "a.5 : " << formatMessage("|{:.5a}|" , Args().arg(vals[i])) << "\n";
        std::cout << "A.5 : " << formatMessage("|{:.5A}|" , Args().arg(vals[i])) << "\n";
        std::cout << "a   : " << formatMessage("|{:a}|"   , Args().arg(vals[i])) << "\n";
        std::cout << "A   : " << formatMessage("|{:A}|"   , Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "a05'.05': " << formatMessage("|{:05'.05'a}|", Args().arg(vals[i])) << "\n";
        std::cout << "A05'.05': " << formatMessage("|{:05'.05'A}|", Args().arg(vals[i])) << "\n";
        std::cout << "a05'.5' : " << formatMessage("|{:05'.5a}|" , Args().arg(vals[i])) << "\n";
        std::cout << "A05'.5' : " << formatMessage("|{:05'.5'A}|", Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "a05.05: " << formatMessage("|{:05.05a}|", Args().arg(vals[i])) << "\n";
        std::cout << "A05.05: " << formatMessage("|{:05.05A}|", Args().arg(vals[i])) << "\n";
        std::cout << "a05.5 : " << formatMessage("|{:05.5a}|" , Args().arg(vals[i])) << "\n";
        std::cout << "A05.5 : " << formatMessage("|{:05.5A}|" , Args().arg(vals[i])) << "\n";
        std::cout << "a05   : " << formatMessage("|{:05a}|"   , Args().arg(vals[i])) << "\n";
        std::cout << "A05   : " << formatMessage("|{:05A}|"   , Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "f'.05': " << formatMessage("|{:'.05'f}|", Args().arg(vals[i])) << "\n";
        std::cout << "F'.05': " << formatMessage("|{:'.05'F}|", Args().arg(vals[i])) << "\n";
        std::cout << "f'.5' : " << formatMessage("|{:'.5f}|"  , Args().arg(vals[i])) << "\n";
        std::cout << "F'.5' : " << formatMessage("|{:'.5'F}|" , Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "f.05: " << formatMessage("|{:.05f}|", Args().arg(vals[i])) << "\n";
        std::cout << "F.05: " << formatMessage("|{:.05F}|", Args().arg(vals[i])) << "\n";
        std::cout << "f.5 : " << formatMessage("|{:.5f}|" , Args().arg(vals[i])) << "\n";
        std::cout << "F.5 : " << formatMessage("|{:.5F}|" , Args().arg(vals[i])) << "\n";
        std::cout << "f   : " << formatMessage("|{:f}|"   , Args().arg(vals[i])) << "\n";
        std::cout << "F   : " << formatMessage("|{:F}|"   , Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "f05'.05': " << formatMessage("|{:05'.05'f}|", Args().arg(vals[i])) << "\n";
        std::cout << "F05'.05': " << formatMessage("|{:05'.05'F}|", Args().arg(vals[i])) << "\n";
        std::cout << "f05'.5' : " << formatMessage("|{:05'.5f}|"  , Args().arg(vals[i])) << "\n";
        std::cout << "F05'.5' : " << formatMessage("|{:05'.5'F}|" , Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "f05.05: " << formatMessage("|{:05.05f}|", Args().arg(vals[i])) << "\n";
        std::cout << "F05.05: " << formatMessage("|{:05.05F}|", Args().arg(vals[i])) << "\n";
        std::cout << "f05.5 : " << formatMessage("|{:05.5f}|" , Args().arg(vals[i])) << "\n";
        std::cout << "F05.5 : " << formatMessage("|{:05.5F}|" , Args().arg(vals[i])) << "\n";
        std::cout << "f05   : " << formatMessage("|{:05f}|"   , Args().arg(vals[i])) << "\n";
        std::cout << "F05   : " << formatMessage("|{:05F}|"   , Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "e'.05': " << formatMessage("|{:'.05'e}|", Args().arg(vals[i])) << "\n";
        std::cout << "E'.05': " << formatMessage("|{:'.05'E}|", Args().arg(vals[i])) << "\n";
        std::cout << "e'.5' : " << formatMessage("|{:'.5e}|"  , Args().arg(vals[i])) << "\n";
        std::cout << "E'.5' : " << formatMessage("|{:'.5'E}|" , Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "e.05: " << formatMessage("|{:.05e}", Args().arg(vals[i])) << "\n";
        std::cout << "E.05: " << formatMessage("|{:.05E}", Args().arg(vals[i])) << "\n";
        std::cout << "e.5 : " << formatMessage("|{:.5e}" , Args().arg(vals[i])) << "\n";
        std::cout << "E.5 : " << formatMessage("|{:.5E}" , Args().arg(vals[i])) << "\n";
        std::cout << "e   : " << formatMessage("|{:e}|"  , Args().arg(vals[i])) << "\n";
        std::cout << "E   : " << formatMessage("|{:E}|"  , Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "g'.05': " << formatMessage("|{:'.05'g}|", Args().arg(vals[i])) << "\n";
        std::cout << "G'.05': " << formatMessage("|{:'.05'G}|", Args().arg(vals[i])) << "\n";
        std::cout << "g'.5' : " << formatMessage("|{:'.5g}|"  , Args().arg(vals[i])) << "\n";
        std::cout << "G'.5' : " << formatMessage("|{:'.5'G}|" , Args().arg(vals[i])) << "\n";
        
        std::cout << "------------------------------\n";

        std::cout << "g.05: " << formatMessage("|{:.05g}|", Args().arg(vals[i])) << "\n";
        std::cout << "G.05: " << formatMessage("|{:.05G}|", Args().arg(vals[i])) << "\n";
        std::cout << "g.5 : " << formatMessage("|{:.5g}|" , Args().arg(vals[i])) << "\n";
        std::cout << "G.5 : " << formatMessage("|{:.5G}|" , Args().arg(vals[i])) << "\n";
        std::cout << "g   : " << formatMessage("|{:g}|"   , Args().arg(vals[i])) << "\n";
        std::cout << "G   : " << formatMessage("|{:G}|"   , Args().arg(vals[i])) << "\n";
        
        //std::cout << "------------------------------\n";

    }

    std::cout << "\n";
#endif
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

