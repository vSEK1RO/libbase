#include <stdexcept>

#include <base/baseN.hpp>
#include <base/hex.hpp>

static const char hexdigits[] = "0123456789abcdef";

static const int8_t hexmap[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
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
};

namespace hex
{
    bool isValid(const char *str) noexcept
    {
        return baseN::isValid(str, hexmap);
    }
    bool isValid(const std::string &str) noexcept
    {
        return baseN::isValid(str, hexmap);
    }
    void encode(const uint8_t *data, uint64_t data_size, char *str) noexcept
    {
        uint64_t i = 0;
        while (str[i] != '\0' && i / 2 < data_size)
        {
            if (i % 2 == 0)
            {
                str[i] = hexdigits[data[i / 2] >> 4];
            } else {
                str[i] = hexdigits[data[i / 2] & 0x0F];
            }
            i++;
        }
    }
    std::string encode(const std::vector<uint8_t> &data) noexcept
    {
        std::string str(data.size() * 2, ' ');
        hex::encode(data.data(), data.size(), str.data());
        return str;
    }
    void decode(const char *str, uint8_t *data, uint64_t data_size)
    {
        if (!hex::isValid(str))
        {
            throw std::logic_error("hex::decode: out of digits map");
        }
        uint64_t i = 0;
        while (i < data_size && str[i * 2] != '\0')
        {
            if(str[i + 1] == '\0')
            {
                throw std::logic_error("hex::decode: isn't hex");
            }
            data[i] = hexmap[(int8_t)str[i * 2]] << 4 | hexmap[(int8_t)str[i * 2 + 1]];
            i++;
        }
    }
    std::vector<uint8_t> decode(const std::string &str)
    {
        std::vector<uint8_t> data(str.size() / 2);
        hex::decode(str.data(), data.data(), data.size());
        return data;
    }
}