
#include <cassert>
#include <charconv>
#include <string>
#include <iostream>
#include <array>
#include <system_error>


template <typename T>
std::string format_float_to_chars(T value, char spec=0, int precision = -1) {
    std::array<char, 64> buffer;

    bool hasSpec = true;
    // Выбор формата для std::to_chars
    std::chars_format fmt = std::chars_format::general;
    switch (spec) {
        case 'a': case 'A': fmt = std::chars_format::hex; break;
        case 'e': case 'E': fmt = std::chars_format::scientific; break;
        case 'f': case 'F': fmt = std::chars_format::fixed; break;
        case 'g': case 'G': fmt = std::chars_format::general; break;
        default: hasSpec = false; // throw std::invalid_argument("Invalid specifier");
    }

    // Преобразование числа в строку
    std::to_chars_result result;
    if (!hasSpec)
    {
         result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);
    }
    else if (precision<0)
    {
        result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, fmt);
    }
    else
    {
        result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, fmt, precision);
    }
    
    if (result.ec != std::errc()) {
        throw std::runtime_error("Formatting failed");
    }

    return std::string(buffer.data(), result.ptr);
}

inline
void test(char spec)
{
    double vals[4] = { 3.14E-15
                     , 0.0000001
                     , 3.1415926535
                     , 271828182845904523536028747135266249775724709369995.0
                     };
    for(auto i=0; i!=4; ++i)
    {
        std::cout << (i!=0 ? "---------------" : "---------------------------------------------") << "\n";;
        std::cout << "With spec '" << std::string(1, spec) << "' and precision: " << format_float_to_chars(vals[i], spec, 6) << "\n";
        std::cout << "With spec '" << std::string(1, spec) << "' no precision : " << format_float_to_chars(vals[i], spec) << "\n";
        std::cout << "No spec '" << std::string(1, spec) << "' no precision   : " << format_float_to_chars(vals[i]) << "\n";
    }

    std::cout << "\n";
}


int main()
{
    try
    {
        test('a');
        test('A');
        test('e');
        test('E');
        test('f');
        test('F');
        test('g');
        test('G');
    }
    catch(const std::exception &e)
    {
        std::cout << "Exception catched: " << e.what() << "\n";
    }

}

