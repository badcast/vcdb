#pragma once

#include "VCDB_begin.hpp"

namespace vcdb
{
namespace helper
{
std::string get_sha256sum(const std::string &filename);
std::string url_decode(const std::string &input);
std::string url_encode(const std::string &input);
}
}
