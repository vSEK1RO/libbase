#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace baseN
{
    bool isValid(const std::string &str, const int8_t *map) noexcept;
    std::string encode(std::vector<uint8_t> data, uint8_t base, uint64_t enc_size, const char *digits) noexcept;
    std::vector<uint8_t> decode(const std::string &str, uint8_t base, uint64_t dec_size, const char *digits, const int8_t *map);
}