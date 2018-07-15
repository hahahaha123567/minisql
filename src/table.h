#ifndef TABLE_H
#define TABLE_H

#include "attribute.h"
#include <vector>
#include <string>

class Table{
public:
    std::string name;
    std::string primaryKey;
    std::vector<Attribute> attri;
};

#endif // TABLE_H
