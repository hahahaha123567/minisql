#include "buffermanager.h"
#include <QDir>
#include <fstream>
#include <QDebug>

BufferManager::BufferManager()
{
    // get all data file name in this path
    QString filter = "I:\\Qt\\MiniSQL\\DBFiles";
    QDir dir(filter, "*.data");
    foreach(QFileInfo tempInfo, dir.entryInfoList()) {
        std::string tableName = tempInfo.fileName().remove(".data").toStdString();
        BlockVec vec;
        // calculate the number of blocks needed
        int needBlockNum = tempInfo.size() >> 12; // tempInfo.size() / DefaultBlockSize;
        if ((tempInfo.size() & 0xFFF) != 0) {
            needBlockNum++;
        }
        for (int i = 0; i < needBlockNum; ++i) {
            Block temp = readFromFile(tableName, i << 12);
            vec.push_back(temp);
        }
        map.insert(std::make_pair(tableName, vec));
    }
}

BufferManager::~BufferManager()
{
    BlockMap::iterator it;
    for (it = map.begin(); it != map.end(); ++it) {
        for (int i = 0; i < it->second.size(); ++i) {
            writeToFile(it->second[i], "I:\\Qt\\MiniSQL\\DBFiles\\"+it->first+".data", DefaultBlockSize*i);
        }
    }
}

BlockVec & BufferManager::read(std::string table)
{
    BlockMap::iterator it = map.find(table);
    if (it == map.end()) {
        qDebug() << "no such table" << endl;
        BlockVec error;
        return error;
    }
    else if (map[table].size() == 0){
        addBlock(table);
        return map[table];
    }
    else {
        return map[table];
    }
}

bool BufferManager::addBlock(std::string table)
{
    BlockMap::iterator it = map.find(table);
    if (it == map.end()) {
        qDebug() << "no such table" << endl;
        return false;
    }
    Block b(table, (int)map[table].size()*DefaultBlockSize);
    map[table].push_back(b);
    return true;
}

Block BufferManager::readFromFile(std::string tableName, int offset)
{
    std::string fileName = "I:\\Qt\\MiniSQL\\DBFiles\\";
    fileName += tableName;
    fileName += ".data";

    Block b(tableName, offset);
    std::fstream file(fileName, std::ios::in | std::ios::binary);
    file.seekg(offset, std::ios::beg);
    file.read(b.data, DefaultBlockSize);
    b.spaceUsed = file.gcount();
    file.close();

    return b;
}

bool BufferManager::writeToFile(const Block & b, std::string fileName, int offset)
{
    // qDebug() << "write to " << QString::fromStdString(fileName) << ", spaceused: " << b.spaceUsed << endl;
    std::fstream file(fileName, std::ios::in | std::ios::out);
    if (file.is_open() == false) {
        qDebug() << "write to file error" << endl;
        return false;
    }
    int * i = (int *)b.data;
    qDebug() << *i << endl;
    i++;
    qDebug() << *i << endl;
    file.seekp(offset);
    file.write(b.data, b.spaceUsed);
    file.close();

    return true;
}
