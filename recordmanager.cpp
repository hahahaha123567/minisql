#include "recordmanager.h"
#include "buffermanager.h"
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>

int string2int(std::string str);
float string2float(std::string str);

RecordManager::RecordManager(BufferManager * p, CatalogManager *q)
    :bufferM(p), catalogM(q)
{

}

RecordManager::~RecordManager()
{

}

void RecordManager::dropTable(const std::string & tableName)
{
    std::string path = "I:\\Qt\\MiniSQL\\DBFiles\\";
    path += tableName;
    path += ".data";
    remove(path.c_str());
}
/*
isDeleted nullList value1 value2 ...
*/
void RecordManager::insert(const Table &t, const std::vector<std::string> &data)
{
    char * add = (char *)malloc(DefaultBlockSize);
    char * move = add;
    int index = 0;
    for (int i = 0; i < t.attri.size(); ++i) {
        if (t.attri[i].getType() == -1) { // int
            int temp = string2int(data[index]);
            memcpy(move, &temp, sizeof(int));
            move += sizeof(int);
            index++;
        }
        else if (t.attri[i].getType() == -2) { // float
            float temp = string2float((data[index]));
            memcpy(move, &temp, sizeof(float));
            move += sizeof(float);
            index++;
        }
        else { // string
            memcpy(move, data[index].c_str(), data[index].size());
            move += data[index].size();
            index++;
        }
    }
    memcpy(bufferM->read(t.name)[0].data + bufferM->read(t.name)[0].spaceUsed, add, (int)(move-add));
    bufferM->read(t.name)[0].spaceUsed += (int)(move-add);
}

void RecordManager::createTable(const Table & t)
{
    std::string path = "I:\\Qt\\MiniSQL\\DBFiles\\";
    path += t.name;
    path += ".data";
    std::fstream in;
    in.open(path, std::ios::out | std::ios::trunc);
    in.close();
}

int string2int(std::string str)
{
    int ret;
    std::stringstream ss;

    ss << str;
    ss >> ret;

    return ret;
}

float string2float(std::string str)
{
    float ret;
    std::stringstream ss;

    ss << str;
    ss >> ret;

    return ret;
}
