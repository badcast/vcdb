#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <omp.h>
#include <mutex>

#include "vcdb.hpp"

using namespace std;
namespace fs = std::filesystem;

std::string defaultSQDBFile()
{
    constexpr auto DBFIleName = "vcdbsql.db";
    std::string retval;
#ifdef __linux__
    retval = "/tmp/";
#elif WIN32
    retval = std::getenv("TEMP");
    retval += '/';
#endif
    retval += DBFIleName;
    return retval;
}

void helpMsg()
{
    std::cout << "vcdb <Target Directory or File> <Optinal SQL3 Filepath>" << std::endl;
}

int main(int argc, char **argv)
{
    std::string TargetDir;
    std::string sqdbFile;
    std::vector<VCDataBase> databases;
    std::mutex mtx;

    if(argc > 3)
    {
        std::cout << "Argument is long" << std::endl;
        helpMsg();
        return 1;
    }
    if(argc == 1)
    {
        std::cout << "Target Directory is not set" << std::endl;
        helpMsg();
        return 1;
    }
    if(argc == 3)
        sqdbFile = argv[2];
    else
        sqdbFile = defaultSQDBFile();

    TargetDir = argv[1];

    if(fs::exists(TargetDir) && fs::is_directory(TargetDir))
    {
        std::size_t total = 0;
        std::size_t totalBytes = 0;
        std::vector<std::string> files;
        for(auto &entry : fs::directory_iterator(TargetDir))
        {
            if(!fs::is_regular_file(entry.status()))
                continue;
            files.emplace_back(entry.path());
        }
#pragma omp parallel for
        for(size_t x = 0; x < files.size(); ++x)
        {
            std::cout << "Query File: " << files[x] << std::endl;
            std::cout << " Status: ";
            std::tuple<bool, VCDataBase> queryResult = vcdb::import::from_file_name(files[x]);
            if(!std::get<0>(queryResult))
            {
                std::cout << "Invalid" << std::endl;
                continue;
            }
            std::cout << "Good" << std::endl;
            std::cout << " Nodes: " << std::get<1>(queryResult).nodes.size() << std::endl;
            std::cout << " Sha256sums: " << std::get<1>(queryResult).sha256sum << std::endl;
            mtx.lock();
            totalBytes += fs::file_size(files[x]);
            total += std::get<1>(queryResult).nodes.size();
            if(!std::get<1>(queryResult).nodes.empty())
                databases.push_back(std::get<1>(queryResult));
            mtx.unlock();
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
        std::cout << "||=================================||" << std::endl;
        std::cout << "|| Files: " << databases.size() << std::endl;
        std::cout << "|| Total load entries: " << total << std::endl;
        std::cout << "|| Total Mb: " << totalBytes / 1024 / 1024 << std::endl;
        std::cout << "||=================================||" << std::endl;

        std::cout << "SQL3 syncing." << std::endl;
        vcdb::db::sync(databases, sqdbFile);
        std::cout << "SQL3 sync complete" << std::endl;
    }
    return 0;
}
