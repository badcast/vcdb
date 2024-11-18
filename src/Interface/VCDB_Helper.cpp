#include <string>
#include <algorithm>
#include <iomanip>

#include "vcdb.hpp"
#include "VCDB_Helper.hpp"

using namespace vcdb;

namespace vcdb::helper{

std::string get_sha256sum(const std::string &filename)
{
    constexpr int sha256_size = 64; // 256 bits = 64 bytes
    std::string _result;

#ifdef __unix__

    _result = "/usr/bin/sha256sum \"";
    _result += filename;
    _result += "\"";

    FILE *prog = popen(_result.data(), "r");
    if(prog != nullptr)
    {
        _result.resize(sha256_size);

        fread(_result.data(), 1, _result.size(), prog);

        fclose(prog);
    }
    else
        _result.clear();

#endif

    return _result;
}

std::string url_decode(const std::string& input)
{
    std::string output;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '=' && i + 2 < input.length()) {
            std::string hex = input.substr(i + 1, 2);
            if(std::all_of(hex.begin(), hex.end(), [](char c) {return std::isxdigit(c); } ))
            {
                char decodedChar = static_cast<char>(std::stoi(hex, nullptr, 16));
                output += decodedChar;
            }
            else
            {
                output += hex;
            }
            i += hex.size();
        } else {
            output += input[i];
        }
    }
    return output;
}

std::string url_encode(const std::string& input)
{
    std::ostringstream encoded;
    for (char c : input) {
        if (c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '=' << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(c));
        }
    }
    return encoded.str();
}
}
