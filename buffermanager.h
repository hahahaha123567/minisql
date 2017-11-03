#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include "buffermanager.h"
#include "block.h"

#include <vector>
#include <string>
#include <map>

const int MaxBlockNum =  1 << 14; // 4KB * 2^14 = 64MB

typedef std::vector<Block> BlockVec;
typedef std::map<std::string, BlockVec> BlockMap;

class BufferManager
{
public:
    BufferManager();
    ~BufferManager();
    BlockVec & read(std::string table);
    bool addBlock(std::string table);

    Block readFromFile(std::string fileName, int offset);
    bool writeToFile(const Block & b, std::string fileName, int offset);

private:
    BlockMap map;
    BufferManager * bufferM;
};

#endif // BUFFERMANAGER_H
