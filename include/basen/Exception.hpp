#pragma once

#include <cstdint>
#include <exception>
#include <source_location>
#include <string>
#include <unordered_map>

namespace basen
{
    class Exception : public std::exception
    {
    public:
        enum class Code
        {
            BASE,
            PADDING,
            CHECKSUM,
            OVERFLOW,
            LENGTH,
            OUT_OF_ALPH,
            ALPH_COLLISION
        };
        static const std::unordered_map<Code, std::string> messages;

        Exception(Code code, const std::source_location &location = std::source_location::current());
        const char *what() const noexcept override
        {
            return _what.c_str();
        }
        inline const char *message() const noexcept
        {
            return messages.at(_code).c_str();
        }
        inline Code code() const noexcept
        {
            return _code;
        }

    private:
        Code _code;
        std::string _what;
    };
}