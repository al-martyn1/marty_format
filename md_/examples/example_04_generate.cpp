#include <cassert>
#include <string>
#include <iostream>
#include <array>
#include <unordered_set>

#include "marty_format/marty_format.h"

using std::cout;
using namespace marty::format;
 
int main()
{

    cout << "\n"
    "#include <cassert>\n"
    "#include <string>\n"
    "#include <iostream>\n"
    "#include <array>\n"
    "\n\n"
    "#include \"marty_format/marty_format.h\"\n"
    "\n\n"
    "using std::cout;\n"
    "using namespace marty::format;\n"
    "\n"
    "int main()\n"
    "{\n";

    constexpr const std::size_t fmtTextWidth = 12u;

    std::array<char, 3> fillChars  = {0, ' ', '*'}; // Fill используем, только если используется align
    std::array<char, 5> alignChars = {0, '=', '<', '>', '^' };
    std::array<char, 4> signChars  = {0, '-', '+', ' '};
    std::array<const char*, 5> alignNames = {"Default", "Width", "Left", "Right", "Center"};

    int valueInitialVal = 1420;
    std::array<int, 2> signs  = {-1, 1};

    std::array<bool, 2> useZeros            = {false, true};
    std::array<bool, 2> useAlters           = {false, true};
    std::array<bool, 2> useCaseInverses     = {false, true};
    std::array<bool, 2> useBitcasts         = {false, true};
    std::array<const char*, 2> fmtWidthStrs = {"0", /* "8", */ "12"};
    std::array<char, 8> fmtChars            = {0, 'b', /* 'B', */ 'd',  'x', 'X', 'h' /* , 'H' */ , '%', 'n'};
    std::array<char, 3> grpSeps             = {0, /* '_', */ '\''};


    std::unordered_set<std::string> usedFmts;
    std::unordered_set<std::string> resultStrings;


    for(auto sign: signs)
    {
        auto value = valueInitialVal * sign;

        //std::string strNegPosTitle = "# " + std::string(sign<0 ? "Negative" : "Positive");
        //cout << "\n    cout << \"" << strNegPosTitle << "\\n\";\n";

        for(std::size_t ai=0; ai!=alignChars.size(); ++ai)
        {
            auto alignChar = alignChars[ai];

            //auto alignName = alignNames[ai];
            //std::string strAlignTitle = "## Align: " + std::string(alignName);
            //cout << "\n    cout << \"" << strAlignTitle << "\\n\";\n";

            for(auto fillCh : fillChars)
            {
                for(auto signChar : signChars)
                {
                    for(const auto useBitcast : useBitcasts)
                    {
                        for(const auto useCaseInverse : useCaseInverses)
                        {
                            for(const auto useZero : useZeros)
                            {
                                for(const auto useAlter : useAlters)
                                {
                                    for(auto fmtWidthStr: fmtWidthStrs)
                                    {
                                        for(auto grpSep : grpSeps)
                                        {
                                            for(auto fmtChar : fmtChars)
                                            {
                                                bool useZeroVal        = useZero ;
                                                bool useAlterVal       = useAlter;
                                                bool useCaseInverseVal = useCaseInverse;
                                                bool useBitcastVal     = useBitcast;

                                                std::string fmt = "{:";
                                                if (alignChar!=0)
                                                {
                                                    if (fillCh!=0)
                                                        fmt.append(1, fillCh);
                                                    fmt.append(1, alignChar);
                                                }
    
                                                if (signChar)
                                                    fmt.append(1, signChar);
            
                                                if (sign>0)
                                                    useBitcastVal = false;
    
                                                if (fmtChar!='b' && fmtChar!='B' && fmtChar!='x' && fmtChar!='X' && fmtChar!='h' && fmtChar!='H')
                                                {
                                                    // Имеет смысл только для 0b/0x
                                                    useBitcastVal     = false;
                                                    useCaseInverseVal = false;
                                                    useAlterVal       = false;
                                                }
    
                                                if (useBitcastVal)
                                                    fmt.append(1, '~');
            
                                                if (useCaseInverseVal)
                                                    fmt.append(1, '!');
            
                                                if (useAlterVal)
                                                    fmt.append(1, '#');
                    
                                                if (useZeroVal)
                                                    fmt.append(1, '0');
    
                                                fmt.append(fmtWidthStr);

                                                if (grpSep!=0)
                                                    fmt.append(1, grpSep);

                                                if (fmtChar!=0)
                                                    fmt.append(1, fmtChar);
    
                                                fmt.append(1, '}');
    
                                                auto key = std::string(1, (sign<0 ? '-' : '+'));
                                                key += fmt;
                                                
                                                if (usedFmts.find(key)!=usedFmts.end())
                                                    continue;
    
                                                usedFmts.insert(key);
    
                                                std::size_t fmtFillSize = 0;
                                                if (fmt.size()<fmtTextWidth)
                                                    fmtFillSize = std::size_t(fmtTextWidth-fmt.size());

                                                auto strRes = marty::format::formatMessage(fmt, {value} );
                                                if (resultStrings.find(strRes)!=resultStrings.end())
                                                    continue;

                                                resultStrings.insert(strRes);
                    
                                                cout << "    cout << formatMessage(\"|" << fmt << "|\", " << std::string(fmtFillSize, ' ') << "{ " << value << " }) << \"\\n\"; // |" << strRes << "|\n";

                                                // std::string fullFmtStr = std::string("{") + fmt + "}" + std::string(fmtFillSize, ' ') + "  -  |" + fmt + "|\\n";
                                                // cout << "    cout << formatMessage(\"" << fullFmtStr.c_str() << "\", {" << value << "} );\n";
    
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    cout << "\n}\n\n\n";

}

