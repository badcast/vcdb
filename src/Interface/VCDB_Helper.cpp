#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>

#include "vcdb.hpp"
#include "VCDB_Helper.hpp"

#ifdef VCDB_HAS_OPENSSL
#include <openssl/sha.h>
#endif

#ifdef __unix__
#define SHA256_UNIX_UTIL 1
#endif

using namespace vcdb;

namespace vcdb::helper{

std::string get_sha256sum(const std::string &filename)
{
    std::string _result {};

#ifdef VCDB_HAS_OPENSSL
    FILE *file;
    char _buf[1024];
    size_t num;
    SHA256_CTX sha256;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    file = fopen(filename.c_str(), "rb");
    if(file == nullptr)
    {
        return {};
    }

    SHA256_Init(&sha256);
    while((num=fread(_buf, sizeof(char), sizeof(_buf), file)) > 0)
    {
        SHA256_Update(&sha256, _buf, num);
    }
    fclose(file);
    SHA256_Final(hash, &sha256);

    for(int x = 0; x < SHA256_DIGEST_LENGTH; ++x)
    {
        snprintf(reinterpret_cast<char*>(_buf), sizeof(3), "%02x", hash[x]);
        _result += reinterpret_cast<char*>(_buf);
    }
#elif defined(SHA256_UNIX_UTIL)
    constexpr int sha256_size = 64; // 256 bits = 64 bytes
    FILE *pipe;
    _result = "/usr/bin/sha256sum \"";
    _result += filename;
    _result += "\"";
    pipe = popen(_result.data(), "r");
    if(pipe != nullptr)
    {
        _result.resize(sha256_size);
        fread(_result.data(), 1, _result.size(), pipe);
        fclose(pipe);
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
