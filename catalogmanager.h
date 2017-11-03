#ifndef CATALOGMANAGER_H
#define CATALOGMANAGER_H

#include "table.h"
#include "attribute.h"
#include <string>
#include <vector>

class CatalogManager
{
public:
    CatalogManager();

    virtual ~CatalogManager();

    bool tableExists(const std::string & tableName);

    int fieldCount(const std::string & tableName);

    // int indexCount(const std::string & tableName);

    std::vector<Attribute> fieldsOnTable(const std::string & tableName);

    // std::vector<std::string> indicesOnTable(const std::string & tableName);

    int rowLength(const std::string & tableName);

    int pkOnTable(const std::string & tableName);

    int fieldType(const std::string & tableName, int fieldNum);

    // bool hasIndex(const std::string & tableName, const std::string & field);

    // std::string indexName(const std::string & tableName, const std::string & field);

    // std::string tableIndexOn(const std::string & index);

    // std::string fieldIndexOn(const std::string & index);

    void addTable(const Table & tableName);

    void deleteTable(const std::string & tableName);

    // bool addIndex();

    // bool deleteIndex(const std::string & index);

    std::string fieldName(const std::string & tableName, int dieldNum);

    const Table getTable(const std::string tableName);

private:
    std::vector<Table> tableInfo;

    int locaOfTable(const std::string tableName);
};

#endif // CATALOGMANAGER_H
