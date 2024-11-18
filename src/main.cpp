#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <unistd.h>

// Virtual Card Structs
#include <vcard.h>
// Parser
#include <text_io.h>

using namespace std;

struct node_t
{
    string name;
    string tel;
    char countrycode[8];
};

struct nodefile_t
{
    string filename;
    char sha256sum[64];
    vector<node_t> nodes;
};

string get_sha256sum(const char *file)
{
    string _result;

    constexpr int sha256_size = 64; // 256 bits = 64 bytes

#ifdef __unix__

    _result = "/usr/bin/sha256sum \"";
    _result += file;
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

bool getnode(const char *filename, nodefile_t *_node)
{
    if(_node == nullptr || filename == nullptr)
    {
        cerr << "var is null" << endl;
        return false;
    }

    ifstream file;

    file.open(filename);

    if(!file)
    {
        cerr << "Fail load vcard file" << endl;
        return false;
    }

    std::vector<vCard> vcards;
    TextReader tr {file};

    /* try
     {*/
    vcards = tr.parseCards();

    file.close();
    /* }
    catch(exception &ex)
    {
        cerr << ex.what() << endl;
        return false;
    }*/

    // Fill sha256sum field
    memcpy(_node->sha256sum, get_sha256sum(filename).data(), sizeof(_node->sha256sum));

    return true;
}

int main()
{
    ifstream file;

    nodefile_t node;

    bool result = getnode("/media/storage/._Private/Контакты_002 (2).vcf", &node);

    if(!result)
        cerr << "Error load node" << endl;
    else
    {
        cout << "Node file: " << node.filename << endl;
        cout << "Node sha256: " << node.sha256sum << endl;
        cout << "Node elems: " << node.nodes.size() << endl;
    }

    return 0;
}
