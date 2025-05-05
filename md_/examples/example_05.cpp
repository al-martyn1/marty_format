/*! \file
    \brief Тестируем форматирование чисел с плавающей точкой, используем все форматные символы.
 */


#include "marty_format/marty_format.h"

//
#include <iostream>

using std::cout;
using std::cerr;

inline
void test()
{
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
}


int main()
{
    try
    {
        using namespace marty::format;
        std::cout << "f'.05': " << formatMessage("|{:'.05'f}|", Args().arg(3.14E-15)) << "\n";

        test();
    }
    catch(const std::exception &e)
    {
        std::cout << "Exception catched: " << e.what() << "\n";
    }

    return 0;
}

