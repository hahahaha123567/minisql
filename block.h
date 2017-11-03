#ifndef BLOCK_H
#define BLOCK_H

#include <string>
; // I don't know why but if there is no ';', there will be a compiling error

const int DefaultBlockSize = 1 << 12; // 4KB

class Block {
public:
    Block(std::string file, int fileOffset);
    Block(const Block &);
    ~Block();
    std::string file;
    int fileOffset;
    int spaceUsed;
    bool isDirty;

    char * data;
};

#endif // BLOCK_H
