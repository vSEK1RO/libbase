#include <stdexcept>

#include <basen/base58.hpp>
#include <basen/baseN.hpp>
#include <basen/hash/sha256.hpp>

namespace base58
{
    const char digits[] =
        "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    const int8_t map[] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1, -1, -1, -1, -1,
        -1, 9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
        22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
        -1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
        47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
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
    uint64_t sizeEncoded(std::span<const uint8_t> data) noexcept
    {
        return baseN::sizeEncoded(data, 58);
    }
    uint64_t sizeDecoded(std::string_view str) noexcept
    {
        return baseN::sizeDecoded(str, 58, digits);
    }
    void encode(const uint8_t *data, uint64_t data_size, char *str, uint64_t str_size) noexcept
    {
        baseN::encode(data, data_size, str, str_size, 58, digits);
    }
    std::string encode(std::span<const uint8_t> data) noexcept
    {
        return baseN::encode(data, 58, digits);
    }
    void decode(const char *str, uint64_t str_size, uint8_t *data, uint64_t data_size) noexcept
    {
        baseN::decode(str, str_size, data, data_size, 58, digits, map);
    }
    std::vector<uint8_t> decode(std::string_view str) noexcept
    {
        return baseN::decode(str, 58, digits, map);
    }
    std::string encodeCheck(std::span<const uint8_t> data) noexcept
    {
        std::vector<uint8_t> buff(data.begin(), data.end()), dhash;
        dhash = hash::sha256(hash::sha256(data));
        buff.insert(buff.end(), &dhash[0], &dhash[4]);
        return base58::encode(buff);
    }
    std::vector<uint8_t> decodeCheck(std::string_view str)
    {
        std::vector<uint8_t> buff(base58::decode(str));
        std::span<uint8_t> data(buff.begin(), buff.end() - 4);
        std::span<uint8_t> dhash(buff.end() - 4, buff.end());
        if (!std::equal(dhash.begin(), dhash.end(), hash::sha256(hash::sha256(data)).begin()))
        {
            throw std::logic_error("base58::decodeCheck: checksum incorrect");
        }
        return std::vector<uint8_t>(data.begin(), data.end());
    }
}