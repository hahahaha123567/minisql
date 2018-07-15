#include "block.h"


Block::Block(std::string t_file, int t_fileOffset)
    : file(t_file), fileOffset(t_fileOffset), spaceUsed(0), isDirty(false)
{
    this->data = new char[DefaultBlockSize];
}

Block::Block(const Block & b)
{
    if (this != &b) {
        this->file = b.file;
        this->fileOffset = b.fileOffset;
        this->spaceUsed = b.spaceUsed;
        this->isDirty = b.isDirty;
        this->data = new char[DefaultBlockSize];
        memcpy(this->data, b.data, DefaultBlockSize);
    }
}

Block::~Block()
{
    delete(this->data);
}
