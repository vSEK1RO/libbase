#include <limits>
#include <stdexcept>

#include <basen/baseN.hpp>
#include <basen/Exception.hpp>
#include <basen/hex.hpp>

namespace hex
{
    const char digits[] = "0123456789abcdef";
    const uint8_t map[] = {
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 255, 255, 255, 255, 255, 255,
        255, 10, 11, 12, 13, 14, 15, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 10, 11, 12, 13, 14, 15, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        //
    };
    bool isValid(const char *str, size_t str_size) noexcept
    {
        return baseN::isValid(str, str_size, map);
    }
    bool isValid(std::string_view str) noexcept
    {
        return baseN::isValid(str, map);
    }
    size_t sizeEncoded(std::span<const uint8_t> data)
    {
        if (data.size() > std::numeric_limits<size_t>::max() / 2)
        {
            throw basen::Exception(basen::Exception::Code::OVERFLOW);
        }
        return data.size() * 2;
    }
    size_t sizeDecoded(std::string_view str) noexcept
    {
        return str.size() / 2;
    }
    void encode(const uint8_t *data, size_t data_size, char *str, size_t str_size)
    {
        if (str_size < hex::sizeEncoded(std::span<const uint8_t>(data, data_size)))
        {
            throw basen::Exception(basen::Exception::Code::LENGTH);
        }
        for (size_t i = 0; i < data_size; i++)
        {
            str[i * 2] = digits[data[i] >> 4];
            str[i * 2 + 1] = digits[data[i] & 0x0F];
        }
    }
    std::string encode(std::span<const uint8_t> data)
    {
        std::string str(hex::sizeEncoded(data), ' ');
        hex::encode(data.data(), data.size(), str.data(), str.size());
        return str;
    }
    void decode(const char *str, size_t str_size, uint8_t *data, size_t data_size)
    {
        if (str_size % 2 != 0)
        {
            throw basen::Exception(basen::Exception::Code::PADDING);
        }
        if (data_size < hex::sizeDecoded(std::string_view(str, str_size)))
        {
            throw basen::Exception(basen::Exception::Code::LENGTH);
        }
        if (!hex::isValid(str, str_size))
        {
            throw basen::Exception(basen::Exception::Code::OUT_OF_ALPH);
        }
        for (size_t i = 0; i * 2 < str_size; i++)
        {
            data[i] = map[(uint8_t)str[i * 2]] << 4 | map[(uint8_t)str[i * 2 + 1]];
        }
    }
    std::vector<uint8_t> decode(std::string_view str)
    {
        std::vector<uint8_t> data(hex::sizeDecoded(str));
        hex::decode(str.data(), str.size(), data.data(), data.size());
        return data;
    }
}