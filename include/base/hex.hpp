#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace hex
{
    bool isValid(const char *str) noexcept;
    bool isValid(std::string_view str) noexcept;
    void encode(const uint8_t *data, uint64_t data_size, char *str) noexcept;
    std::string encode(std::span<const uint8_t> data) noexcept;
    void decode(const char *str, uint8_t *data, uint64_t data_size);
    std::vector<uint8_t> decode(std::string_view str);
}