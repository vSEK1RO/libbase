#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace base58
{
    extern const char digits[59];
    extern const int8_t map[256];

    bool isValid(const char *str, uint64_t str_size) noexcept;
    bool isValid(std::string_view str) noexcept;

    uint64_t sizeEncoded(std::span<const uint8_t> data) noexcept;
    uint64_t sizeDecoded(std::string_view str) noexcept;

    void encode(const uint8_t *data, uint64_t data_size, char *str, uint64_t str_size) noexcept;
    std::string encode(std::span<const uint8_t> data) noexcept;

    void decode(const char *str, uint64_t str_size, uint8_t *data, uint64_t data_size) noexcept;
    std::vector<uint8_t> decode(std::string_view str) noexcept;

    std::string encodeCheck(std::span<const uint8_t> data) noexcept;
    std::vector<uint8_t> decodeCheck(std::string_view str);
}