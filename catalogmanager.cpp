#include "catalogmanager.h"
#include "table.h"
#include <fstream>
#include <vector>
#include <QDebug>

/*
tableName
attributesNumber
primaryKey
attributeName attributeType isUnique ...
*/

CatalogManager::CatalogManager()
{
    std::fstream in("I:\\Qt\\MiniSQL\\DBFiles\\TableInfo", std::ios::in); // !!! relative path doesn't work
    while (!in.eof()) {
        Table tempTable;
        int attriNum;
        in >> tempTable.name >> attriNum >> tempTable.primaryKey;
        for (int i = 0; i < attriNum; ++i) {
            Attribute tempAttri;
            std::string name;
            int type;
            in >> name >> type >> tempAttri.isUnique;
            if (name == "") {
                break;
            }
            tempAttri.setName(name);
            tempAttri.setType(type);
            tempTable.attri.push_back(tempAttri);
        }
        if (tempTable.name != "") {
            tableInfo.push_back(tempTable);
        }
        in.get(); // !!!
        if (in.peek() == '\n') {
            break;
        }
    }
    in.close();
    qDebug() << "size of table info: " << tableInfo.size() << endl;
}

CatalogManager::~CatalogManager()
{
    qDebug() << "size of table info: " << tableInfo.size() << endl;
    //qDebug() << QString::fromStdString(tableInfo[0].name) << " " << QString::fromStdString(tableInfo[1].name) << endl;
    std::fstream out;
    out.open("I:\\Qt\\MiniSQL\\DBFiles\\TableInfo", std::ios::out | std::ios::trunc);
    for (int i = 0; i < tableInfo.size(); ++i) {
        out << tableInfo[i].name << std::endl;
        out << tableInfo[i].attri.size() << std::endl;
        out << tableInfo[i].primaryKey << std::endl;
        for (int j = 0; j < tableInfo[i].attri.size(); ++j) {
            out << tableInfo[i].attri[j].getName() << " " << tableInfo[i].attri[j].getType() << " " << tableInfo[i].attri[j].isUnique << std::endl;
        }
    }
    out.close();
}

bool CatalogManager::tableExists(const std::string &tableName)
{
    for (int i = 0; i < tableInfo.size(); ++i) {
        if (tableInfo[i].name == tableName) {
            return true;
        }
    }
    return false;
}

int CatalogManager::fieldCount(const std::string &tableName)
{
    for (int i = 0; i < tableInfo.size(); ++i) {
        if (tableInfo[i].name == tableName) {
            return (int)tableInfo[i].attri.size();
        }
    }
    return -1;
}

std::vector<Attribute> CatalogManager::fieldsOnTable(const std::string &tableName)
{
    int i = locaOfTable(tableName);
    return tableInfo[i].attri;
}

int CatalogManager::rowLength(const std::string &tableName)
{
    std::vector<Attribute> tempAttri;
    tempAttri = tableInfo[locaOfTable(tableName)].attri;
    int length = 0;
    for (int i = 0; i < tempAttri.size(); ++i) {
        int nico = tempAttri[i].getType();
        switch (nico) {
        case -1:
        case -2:
            length += 4;
            break;
        default:
            length += nico;
            break;
        }
    }
    return length;
}

int CatalogManager::pkOnTable(const std::string &tableName)
{
    Table temp = tableInfo[locaOfTable(tableName)];
    for (int i = 0; i < temp.attri.size(); ++i) {
        if (temp.attri[i].getName() == temp.primaryKey) {
            return i;
        }
    }
    return -1;
}

int CatalogManager::fieldType(const std::string &tableName, int fieldNum)
{
    return tableInfo[locaOfTable(tableName)].attri[fieldNum].getType();
}

void CatalogManager::addTable(const Table &tableName)
{
    tableInfo.push_back(tableName);
}

void CatalogManager::deleteTable(const std::string &tableName)
{
    tableInfo.erase(tableInfo.begin()+locaOfTable(tableName));
}

std::string CatalogManager::fieldName(const std::string &tableName, int fieldNum)
{
    return tableInfo[locaOfTable(tableName)].attri[fieldNum].getName();
}

const Table CatalogManager::getTable(const std::string tableName)
{
    return tableInfo[locaOfTable(tableName)];
}

int CatalogManager::locaOfTable(const std::string tableName)
{
    for (int i = 0; i < tableInfo.size(); ++i) {
        if (tableInfo[i].name == tableName) {
            return i;
        }
    }
    return -1;
}
