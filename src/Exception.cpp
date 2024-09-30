#include <sstream>

#include <basen/Exception.hpp>

namespace basen
{
    const std::unordered_map<Exception::Code, std::string> Exception::messages = {
        {Code::BASE, "incorrect base"},
        {Code::PADDING, "incorrect padding"},
        {Code::CHECKSUM, "incorrect checksum"},
        {Code::OVERFLOW, "overflow"},
        {Code::LENGTH, "not enough  allocated length"},
        {Code::OUT_OF_ALPH, "out of alphabet"},
        {Code::ALPH_COLLISION, "alphabet contains same chars"},
    };
    Exception::Exception(Code code, const std::source_location &location)
    {
        _code = code;
        std::ostringstream oss;
        oss << "\033[34m"
            << location.function_name()
            << ':'
            << location.line()
            << ": \033[31mbasen::Exception:\n\t\033[0m"
            << message()
            << "\n";
        _what = oss.str();
    }
}