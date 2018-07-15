#ifndef REQUEST_H
#define REQUEST_H

#include "attribute.h"

struct where{
    std::string attr;
    std::string relation; // '>' / '=' / '<'
    std::string value;
};

enum requestType {
    no_request = 0,
    create_table = 1,
    drop_table = 2,
    insert = 3,
    _delete = 4,
    select = 5,
    create_index = 6,
    drop_index = 7,
};

class Request
{
public:
    Request();
    requestType type;
    std::string tableName;
    // create table
    std::vector<Attribute> * attri;
    std::string primaryKey;
    // create & drop index
    std::string indexName;
    std::string indexAttr;
    // select
    std::vector<where> * w;
    // insert
    std::vector<std::string> * insertValue;
};

#endif // REQUEST_H
