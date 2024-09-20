#include <limits>
#include <stdexcept>

#include <basen/baseN.hpp>
#include <basen/hex.hpp>

namespace hex
{
    const char digits[] = "0123456789abcdef";
    const int8_t map[] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        //
    };
    bool isValid(const char *str, uint64_t str_size) noexcept
    {
        return baseN::isValid(str, str_size, map);
    }
    bool isValid(std::string_view str) noexcept
    {
        return baseN::isValid(str, map);
    }
    uint64_t sizeEncoded(std::span<const uint8_t> data)
    {
        if (data.size() > std::numeric_limits<uint64_t>::max() / 2)
        {
            throw std::overflow_error("hex::sizeEncoded: overflow");
        }
        return data.size() * 2;
    }
    uint64_t sizeDecoded(std::string_view str) noexcept
    {
        return str.size() / 2;
    }
    void encode(const uint8_t *data, uint64_t data_size, char *str, uint64_t str_size)
    {
        if (str_size < hex::sizeEncoded(std::span<const uint8_t>(data, data_size)))
        {
            throw std::logic_error("hex::encode: not enough allocated length");
        }
        for (uint64_t i = 0; i < data_size; i++)
        {
            str[i * 2] = digits[data[i] >> 4];
            str[i * 2 + 1] = digits[data[i] & 0x0F];
        }
    }
    std::string encode(std::span<const uint8_t> data) noexcept
    {
        std::string str(hex::sizeEncoded(data), ' ');
        hex::encode(data.data(), data.size(), str.data(), str.size());
        return str;
    }
    void decode(const char *str, uint64_t str_size, uint8_t *data, uint64_t data_size)
    {
        if (str_size % 2 != 0)
        {
            throw std::logic_error("hex::decode: isn't hex");
        }
        if (data_size < hex::sizeDecoded(std::string_view(str, str_size)))
        {
            throw std::logic_error("hex::decode: not enough allocated length");
        }
        if (!hex::isValid(str, str_size))
        {
            throw std::logic_error("hex::decode: out of digits map");
        }
        for (uint64_t i = 0; i * 2 < str_size; i++)
        {
            data[i] = map[(int8_t)str[i * 2]] << 4 | map[(int8_t)str[i * 2 + 1]];
        }
    }
    std::vector<uint8_t> decode(std::string_view str) noexcept
    {
        std::vector<uint8_t> data(hex::sizeDecoded(str));
        hex::decode(str.data(), str.size(), data.data(), data.size());
        return data;
    }
}