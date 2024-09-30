#include <functional>
#include <iostream>
#include <unordered_map>

#include <argparse/argparse.hpp>

#include <basen.hpp>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif

namespace basen
{
    typedef std::string (*encoder_t)(std::span<const uint8_t>);
    typedef std::vector<uint8_t> (*decoder_t)(std::string_view);

    const std::unordered_map<std::string, encoder_t> encoders = {
        {"base58", base58::encode},
        {"base58check", base58::encodeCheck},
        {"base64", base64::encode},
        {"hex", hex::encode},
    };
    const std::unordered_map<std::string, decoder_t> decoders = {
        {"base58", base58::decode},
        {"base58check", base58::decodeCheck},
        {"base64", base64::decode},
        {"hex", hex::decode},
    };

    std::string getPipe()
    {
        std::string str;
        str.reserve(1024 * 1024);
#ifdef _WIN32
        if (_isatty(_fileno(stdin)))
#else
        if (isatty(fileno(stdin)))
#endif
        {
            throw std::logic_error("should use pipe syntax ( | or < )");
        }
        else
        {
            char temp[4096];
            while (std::cin.read(temp, sizeof(temp)))
            {
                str.insert(str.end(), temp, temp + std::cin.gcount());
            }
            str.insert(str.end(), temp, temp + std::cin.gcount());
        }
        if (!str.empty() && str.back() == '\n')
        {
            str.erase(str.size() - 1);
        }
        return str;
    }
    uint8_t error(const std::string &str, argparse::ArgumentParser &program) noexcept
    {
        std::cerr << "\033[31merror:\t\033[0m"
                  << str << "\n\n"
                  << program << '\n';
        return 1;
    }
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("basen", "1.1.0");
    program.add_argument("-t", "--type")
        .help("encoding type")
        .metavar("STRING")
        .choices("base58", "base58check", "base64", "hex");
    program.add_argument("-a", "--alphabet")
        .help("specify alphabet")
        .metavar("STRING");
    program.add_argument("-d", "--decode")
        .help("decode flag")
        .flag();
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        return basen::error(err.what(), program);
    }
    if (!program.is_used("-t") && !program.is_used("-a"))
    {
        return basen::error("either -t or -a should be provided", program);
    }
    if (program.is_used("-t") && program.is_used("-a"))
    {
        return basen::error("-t and -a cannot be used together", program);
    }
    std::string str;
    try
    {
        str = basen::getPipe();
    }
    catch (const std::exception &e)
    {
        return basen::error(e.what(), program);
    }
    try
    {
        if (program.is_used("-t"))
        {
            auto type = program.get<std::string>("-t");
            if (program.is_used("-d"))
            {
                auto data = (*basen::decoders.at(type))(str);
                std::for_each(data.begin(), data.end(), [](uint8_t item)
                              { std::cout << (char)item; });
                std::cout << '\n';
            }
            else
            {
                std::span<uint8_t> dv((uint8_t *)str.data(), str.size());
                std::cout << (*basen::encoders.at(type))(dv) << '\n';
            }
        }
        if (program.is_used("-a"))
        {
            auto alphabet = program.get<std::string>("-a");
            uint8_t map[256];
            baseN::digitsMap(alphabet.data(), alphabet.size(), map);
            if (program.is_used("-d"))
            {
                auto data = baseN::decode(str, alphabet.size(), alphabet.data(), map);
                std::for_each(data.begin(), data.end(), [](uint8_t item)
                              { std::cout << (char)item; });
                std::cout << '\n';
            }
            else
            {
                std::span<uint8_t> dv((uint8_t *)str.data(), str.size());
                std::cout << baseN::encode(dv, alphabet.size(), alphabet.data()) << '\n';
            }
        }
    }
    catch (const basen::Exception &e)
    {
        return basen::error(e.message(), program);
    }
    return 0;
}