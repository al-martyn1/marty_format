
#include <cassert>
#include <string>
#include <iostream>

#include <cstdlib>
#include <stdlib.h>

#if !defined(WIN32) && !defined(_WIN32)
    #include <stdlib.h>
#endif


inline
void test()
{
    double vals[4] = { 3.14E-15
                     , 0.0000001
                     , 3.1415926535
                     , 271828182845904523536028747135266249775724709369995.0
                     };
    for(auto i=0; i!=4; ++i)
    {
        std::cout << "----------------------------------------------------------------------------\n";
        std::cout << vals[i] << "\n";
        std::cout << "------------------------------\n";

        {
            int dec  = 0;
            int sign = 0;
            auto fcvtRes = fcvt(vals[i], 6, &dec, &sign);
            std::cout << "FCVT: |" << fcvtRes << "|\n";
            std::cout << "    dec : " << unsigned(dec)  << "\n";
            std::cout << "    sign: " << sign << "\n";
            std::cout << "\n";
        }

        {
            int dec  = 0;
            int sign = 0;
            auto ecvtRes = ecvt(vals[i], 6, &dec, &sign);
            std::cout << "ECVT: |" << ecvtRes << "|\n";
            std::cout << "    dec : " << unsigned(dec)  << "\n";
            std::cout << "    sign: " << sign << "\n";
            std::cout << "\n";
        }
        // #define _CVTBUFSIZE (309+40) /* # of digits in max. dp value + slop */
        {
        #if defined(CVTBUFSIZE)
            char buf[CVTBUFSIZE];
        #elif defined(CVTBUFSIZE)
            char buf[_CVTBUFSIZE];
        #else
            char buf[256+128];
        #endif

            // int dec  = 0;
            // int sign = 0;
            std::cout << "GCVT: |" << gcvt(vals[i], 6, buf) << "|\n";
            // std::cout << "    dec : " << unsigned(dec) << "\n";
            // std::cout << "    sign: " << sign << "\n";
            std::cout << "\n";
        }
    }

    std::cout << "\n";
}


int main()
{
    try
    {
        test();
    }
    catch(const std::exception &e)
    {
        std::cout << "Exception catched: " << e.what() << "\n";
    }

}

