#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>

class Attribute
{
public:
    Attribute();
    inline void setName(const std::string &);
    void setType(const std::string &);
    void setType(int);
    inline std::string getName() const;
    inline int getType() const;
    bool isUnique;
private:
    std::string name;
    int type; // -1 -> int, -2 -> float, >= 1 -> char, -3 -> undefined
};

inline void Attribute::setName(const std::string & a_name)
{
    name = a_name;
}

inline std::string Attribute::getName() const
{
    return name;
}

inline int Attribute::getType() const
{
    return type;
}

#endif // ATTRIBUTE_H
