#include <iostream>

#include <argparse/argparse.hpp>

#include <basen.hpp>

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("basen", "1.1.0");
    program.add_argument("-t", "--type")
        .help("encoding type")
        .metavar("STRING")
        .choices("base58", "base64", "hex")
        .default_value("hex");
    program.add_argument("-d", "--decode")
        .help("is decode")
        .flag();
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << '\n';
        std::cerr << program;
        return 1;
    }
    return 0;
}