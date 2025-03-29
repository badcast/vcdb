#ifdef VCDB_HAS_SQLITE3
#include <sqlite3.h>
#endif

#include "vcdb.hpp"
#include "VCDB_SQL.hpp"

namespace vcdb::db
{

void sync(std::vector<VCDataBase>& vcdb, const std::string &filename)
{
#ifdef VCDB_HAS_SQLITE3
    sqlite3 *db;
    int exitv = sqlite3_open(filename.c_str(), &db);

    if(exitv)
        return;

    // init sql3 db
    const char *sqlCreateTable = "CREATE TABLE IF NOT EXISTS VCDB ("
                                 "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY"
                                 "Name TEXT NOT NULL, "
                                 "Telephone TEXT NOT NULL, "
                                 "Address TEXT NOT NULL, "
                                 "City TEXT NOT NULL);";
    exitv = sqlite3_exec(db, sqlCreateTable, nullptr, nullptr, nullptr);
    if(exitv)
    {
        sqlite3_close(db);
        return;
    }

    // inserts
    const char *sqlInsert = "INSERT INTO VCDB (Name, Telephone, Address, City) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmtInsert;

    if(sqlite3_prepare_v2(db, sqlInsert, -1, &stmtInsert, nullptr) != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    for(size_t x = 0; x < vcdb.size(); ++x)
    {
        VCDataBase * _db = &vcdb[x];
        for(VCNode & node : _db->nodes)
        {
            sqlite3_bind_text(stmtInsert, 1, node.Name.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmtInsert, 2, node.Tel.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmtInsert, 3, node.address.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmtInsert, 4, node.Country.c_str(), -1, SQLITE_STATIC);
            if(sqlite3_step(stmtInsert) != SQLITE_DONE)
            {
                // todo: fail message
            }
            sqlite3_reset(stmtInsert);
        }
    }
    sqlite3_exec(db, "END TRANSACTION;", nullptr, nullptr, nullptr);

    exitv = sqlite3_exec(db, "SELECT DISTINCT Name, Telephone, Address, City FROM VCDB;", nullptr, nullptr, nullptr);

    sqlite3_finalize(stmtInsert);
    sqlite3_close(db);
#endif
}
}
