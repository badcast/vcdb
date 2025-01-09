#include <fstream>

// Virtual Card Structs
#include <vcard.h>
#include <text_io.h>

#include "vcdb.hpp"
#include "VCDB_Helper.hpp"
#include "VCF_Importer.hpp"

namespace vcdb::import
{

std::tuple<bool,VCDataBase> from_file_name(const std::string &filename)
{
    VCDataBase database;
    std::ifstream file;
    file.open(filename);
    if(!file)
    {
        return {false,{}};
    }
    VCNode node;
    TextReader tr {file};
    std::vector<vCard> cards = tr.parseCards();
    file.close();
    for(auto & vcard : cards)
    {
        node = {};
        node.Name = helper::url_decode(vcard[VC_NAME].getValue());
        node.FormatName = helper::url_decode(vcard[VC_FORMATTED_NAME].getValue());
        node.Tel = vcard[VC_TELEPHONE].getValue();
        node.address = vcard[VC_ADDRESS].getValue();
        database.nodes.emplace_back(node);
    }
    database.sha256sum = vcdb::helper::get_sha256sum(filename);
    database.filename = filename;
    return {true,database};
}

}
