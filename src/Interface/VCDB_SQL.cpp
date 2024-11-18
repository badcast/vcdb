#include <sqlite3.h>

#include "vcdb.hpp"
#include "VCDB_SQL.hpp"

namespace vcdb::db
{
void sync(std::vector<VCDataBase>& vcdb, const std::string &filename)
{

    sqlite3 *db;
    int exitv = sqlite3_open(filename.c_str(), &db);

    if(exitv)
        return;

    // init sql3 db
    const char *sqlCreateTable = "CREATE TABLE IF NOT EXISTS Contacts ("
                                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "Name TEXT NOT NULL, "
                                 "Telephone TEXT NOT NULL, "
                                 "Address TEXT NOT NULL, "
                                 "City TEXT NOT NULL);";
    exitv = sqlite3_exec(db, sqlCreateTable, nullptr, 0, nullptr);
    if(exitv)
        return;

    // inserts
    const char *sqlInsert = "INSERT INTO Contacts (Name, Telephone, Address, City) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if(sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }


    for( VCDataBase & _db : vcdb)
    {
        for(VCNode & node : _db.nodes)
        {
            sqlite3_bind_text(stmt, 1, node.Name.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, node.Tel.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, node.address.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, node.Country.c_str(), -1, SQLITE_STATIC);
            if(sqlite3_step(stmt) != SQLITE_DONE)
            {
                // todo: fail message
            }

            sqlite3_reset(stmt);
        }
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
}
}
