#include <cassert>
#include <format>
#include <string>
#include <iostream>
#include <array>

using std::format;
using std::cout;
 
int main()
{

    cout << "\n"
    "#include <cassert>\n"
    "#include <format>\n"
    "#include <string>\n"
    "#include <iostream>\n"
    "#include <array>\n"
    "\n\n"
    "using std::format;\n"
    "using std::cout;\n"
    "\n"
    "int main()\n"
    "{\n";

    std::array<char, 3> fillChars  = {0, ' ', '*'}; // Fill используем, только если используется align
    std::array<char, 4> alignChars = {0, '<', '>', '^'};
    std::array<char, 4> signChars  = {0, '-', '+', ' '};
    std::array<const char*, 4> alignNames = {"Default", "Left", "Right", "Center"};

    int valueInitialVal = 42;
    std::array<int, 2> signs  = {-1, 1};

    std::array<bool, 2> useZeros  = {false, true};
    std::array<bool, 2> useAlters = {false, true};

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
                    for(auto useZero : useZeros)
                    {
                        for(auto useAlter : useAlters)
                        {
                            std::string fmt = "{:";
                            if (alignChar!=0)
                            {
                                if (fillCh!=0)
                                    fmt.append(1, fillCh);
                                fmt.append(1, alignChar);
                            }

                            if (useAlter)
                                fmt.append(1, '#');

                            if (useZero)
                                fmt.append(1, '0');

                            fmt.append("8x}");

                            std::size_t fmtFillSize = 0;
                            if (fmt.size()<12u)
                                fmtFillSize = std::size_t(12u-fmt.size());

                            std::string fullFmtStr = std::string("{") + fmt + "}" + std::string(fmtFillSize, ' ') + "  -  |" + fmt + "|\\n";

                            cout << "    cout << format(\"" << fullFmtStr.c_str() << "\", " << value << ");\n";
                        }
                    }
                }
            }
        }
    }

    cout << "\n}\n\n\n";

}

