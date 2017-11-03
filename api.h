#ifndef API_H
#define API_H

#include "request.h"
#include "catalogmanager.h"
#include "recordmanager.h"

class API
{
public:
    API(CatalogManager * Manager1, RecordManager * Manager2);
    void drive(const Request &);
    void createTable(const Request &);
    void dropTable(const Request &);
    void createIndex(const Request &);
    void dropIndex(const Request &);
    bool selectInfo(const Request &);
    void insertTuple(const Request &);
    void deleteTuple(const Request &);
private:
    CatalogManager * catalogM;
    RecordManager * recordM;
};

#endif // API_H
