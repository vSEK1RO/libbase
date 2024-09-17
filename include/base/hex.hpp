#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace hex
{
    bool isValid(const char *str, uint64_t str_size) noexcept;
    bool isValid(std::string_view str) noexcept;

    void encode(const uint8_t *data, uint64_t data_size, char *str, uint64_t str_size);
    std::string encode(std::span<const uint8_t> data) noexcept;

    void decode(const char *str, uint64_t str_size, uint8_t *data, uint64_t data_size);
    std::vector<uint8_t> decode(std::string_view str) noexcept;
}