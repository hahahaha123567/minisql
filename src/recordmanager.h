#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include "buffermanager.h"
#include "catalogmanager.h"
#include <table.h>

class RecordManager
{
public:
    RecordManager(BufferManager * p, CatalogManager * q);

    virtual ~RecordManager();

    void createTable(const Table & t);

    void dropTable(const std::string & tableName);

    std::vector<std::string> select(const Table & t, std::vector<std::string> conditions);

    void insert(const Table & t, const std::vector<std::string> & data);

    // int deleteTuple(table t, std::vector<Condition> conditions);
private:
    BufferManager * bufferM;
    CatalogManager * catalogM;
};

#endif // RECORDMANAGER_H
