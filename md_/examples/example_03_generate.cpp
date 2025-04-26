#include <cassert>
#include <string>
#include <iostream>
#include <array>
#include <unordered_set>

using std::cout;
 
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

    int valueInitialVal = 420;
    std::array<int, 2> signs  = {-1, 1};

    std::array<bool, 2> useZeros            = {false, true};
    std::array<bool, 2> useAlters           = {false, true};
    std::array<bool, 2> useCaseInverses     = {false, true};
    std::array<bool, 2> useBitcasts         = {false, true};
    std::array<const char*, 2> fmtWidthStrs = {"0",  /* "8", */  "12"};
    std::array<char, 8> fmtChars           = {0, 'b',  /* 'B', */  'd',  'x', 'X', 'h' /* , 'H' */ , '%', 'n'};

    std::unordered_set<std::string> usedFmts;


    for(auto sign: signs)
    {
        auto value = valueInitialVal * sign;

        std::string strNegPosTitle = "# " + std::string(sign<0 ? "Negative" : "Positive");
        cout << "\n    cout << \"" << strNegPosTitle << "\\n\";\n";

        for(std::size_t ai=0; ai!=alignChars.size(); ++ai)
        {
            auto alignChar = alignChars[ai];
            auto alignName = alignNames[ai];

            std::string strAlignTitle = "## Align: " + std::string(alignName);
            cout << "\n    cout << \"" << strAlignTitle << "\\n\";\n";

            for(auto fillCh : fillChars)
            {
                for(auto signChar : signChars)
                {
                    for(auto useBitcast : useBitcasts)
                    {
                        for(auto useCaseInverse : useCaseInverses)
                        {
                            for(auto useZero : useZeros)
                            {
                                for(auto useAlter : useAlters)
                                {
                                    for(auto fmtWidthStr: fmtWidthStrs)
                                    {
                                        for(auto fmtChar : fmtChars)
                                        {
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
                                                useBitcast = false;

                                            if (fmtChar!='b' && fmtChar!='B' && fmtChar!='x' && fmtChar!='X' && fmtChar!='h' && fmtChar!='H')
                                            {
                                                // Имеет смысл только для 0b/0x
                                                useBitcast     = false;
                                                useCaseInverse = false;
                                                useAlter       = false;
                                            }

                                            if (useBitcast)
                                                fmt.append(1, '~');
        
                                            if (useCaseInverse)
                                                fmt.append(1, '!');
        
                                            if (useAlter)
                                                fmt.append(1, '#');
                
                                            if (useZero)
                                                fmt.append(1, '0');

                                            fmt.append(fmtWidthStr);
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
                
                                            std::string fullFmtStr = std::string("{") + fmt + "}" + std::string(fmtFillSize, ' ') + "  -  |" + fmt + "|\\n";
                
                                            cout << "    cout << formatMessage(\"" << fullFmtStr.c_str() << "\", {" << value << "} );\n";

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

