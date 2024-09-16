#include <stdexcept>

#include <base/baseN.hpp>
#include <base/hex.hpp>

static const char hexdigits[] = "0123456789abcdef";

static const int8_t hexmap[] = {
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

namespace hex
{
    bool isValid(const char *str) noexcept
    {
        return baseN::isValid(str, hexmap);
    }
    bool isValid(std::string_view str) noexcept
    {
        return baseN::isValid(str, hexmap);
    }
    void encode(const uint8_t *data, size_t data_size, char *str, size_t str_size) noexcept
    {
        for (size_t i = 0; i < data_size && i * 2 + 1 < str_size; i++)
        {
            str[i * 2] = hexdigits[data[i] >> 4];
            str[i * 2 + 1] = hexdigits[data[i] & 0x0F];
        }
    }
    std::string encode(std::span<const uint8_t> data) noexcept
    {
        std::string str(data.size() * 2, ' ');
        hex::encode(data.data(), data.size(), str.data(), str.size());
        return str;
    }
    void decode(const char *str, size_t str_size, uint8_t *data, size_t data_size)
    {
        if (!hex::isValid(str))
        {
            throw std::logic_error("hex::decode: out of digits map");
        }
        if (str_size % 2 != 0)
        {
            throw std::logic_error("hex::decode: isn't hex");
        }
        for (size_t i = 0; i < data_size && i * 2 < str_size; i++)
        {
            data[i] = hexmap[(int8_t)str[i * 2]] << 4 | hexmap[(int8_t)str[i * 2 + 1]];
        }
    }
    std::vector<uint8_t> decode(std::string_view str)
    {
        std::vector<uint8_t> data(str.size() / 2);
        hex::decode(str.data(), str.size(), data.data(), data.size());
        return data;
    }
}