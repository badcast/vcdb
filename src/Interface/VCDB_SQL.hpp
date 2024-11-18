#pragma once

#include "VCDB_begin.hpp"

namespace vcdb::db
{
void sync(std::vector<VCDataBase> &vcdb, const std::string &filename);
}
