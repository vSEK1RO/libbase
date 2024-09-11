#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace hex
{
    bool isValid(const std::string &str) noexcept;
    std::string encode(const std::vector<uint8_t> &data) noexcept;
    std::vector<uint8_t> decode(const std::string &str);
}