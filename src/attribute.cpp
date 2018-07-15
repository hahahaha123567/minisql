#include "attribute.h"

#include <string>
#include <sstream>
#include <QDebug>

Attribute::Attribute() : name(""), type(-3), isUnique(false)
{

}

void Attribute::setType(const std::string & a_type)
{
    if (a_type == "int") {
        type = -1;
    }
    else if (a_type == "float") {
        type = -2;
    }
    else if (a_type.find("char(") != std::string::npos) {
        int begin = (int)a_type.find("(");
        int end = (int)a_type.find(")");
        std::string str_length = a_type.substr(begin+1, end-begin);
        std::stringstream ss;
        ss << str_length;
        ss >> type;
        if (type < 1 || type >255) {
            qDebug() << "attribute type error" << endl;
        }
    }
    else {
        qDebug() << "attribute type error" << endl;
    }
}

void Attribute::setType(int arg)
{
    type = arg;
}


