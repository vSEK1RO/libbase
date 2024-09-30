#include <stdexcept>

#include <basen/base58.hpp>
#include <basen/baseN.hpp>
#include <basen/Exception.hpp>
#include <basen/hash/sha256.hpp>

namespace base58
{
    const char digits[] =
        "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    const uint8_t map[] = {
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 255, 255, 255, 255, 255, 255,
        255, 9, 10, 11, 12, 13, 14, 15, 16, 255, 17, 18, 19, 20, 21, 255,
        22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 255, 255, 255, 255, 255,
        255, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 255, 44, 45, 46,
        47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 255, 255, 255, 255, 255,
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
        return baseN::sizeEncoded(data, 58);
    }
    size_t sizeDecoded(std::string_view str) noexcept
    {
        return baseN::sizeDecoded(str, 58, digits);
    }
    size_t encode(const uint8_t *data, size_t data_size, char *str, size_t str_size)
    {
        return baseN::encode(data, data_size, str, str_size, 58, digits);
    }
    std::string encode(std::span<const uint8_t> data)
    {
        return baseN::encode(data, 58, digits);
    }
    size_t decode(const char *str, size_t str_size, uint8_t *data, size_t data_size)
    {
        return baseN::decode(str, str_size, data, data_size, 58, digits, map);
    }
    std::vector<uint8_t> decode(std::string_view str)
    {
        return baseN::decode(str, 58, digits, map);
    }
    std::string encodeCheck(std::span<const uint8_t> data)
    {
        std::vector<uint8_t> buff(data.begin(), data.end()), dhash;
        dhash = hash::sha256(hash::sha256(data));
        buff.insert(buff.end(), &dhash[0], &dhash[4]);
        return base58::encode(buff);
    }
    std::vector<uint8_t> decodeCheck(std::string_view str)
    {
        std::vector<uint8_t> buff(base58::decode(str));
        if (buff.size() < 4)
        {
            throw basen::Exception(basen::Exception::Code::PADDING);
        }
        std::span<uint8_t> data(buff.begin(), buff.end() - 4);
        std::span<uint8_t> dhash(buff.end() - 4, buff.end());
        if (!std::equal(dhash.begin(), dhash.end(), hash::sha256(hash::sha256(data)).begin()))
        {
            throw basen::Exception(basen::Exception::Code::CHECKSUM);
        }
        return std::vector<uint8_t>(data.begin(), data.end());
    }
}