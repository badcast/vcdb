#pragma once

#include "VCDB_begin.hpp"

struct VCDataBase;
struct VCNode;

struct VCNode
{
    std::string Name;
    std::string Tel;
    std::string address;
    int CountryCode;
    std::string Country;
};

struct VCDataBase
{
    std::vector<VCNode> nodes;
    std::string filename;
    std::string sha256sum;
};

