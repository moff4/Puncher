#ifndef LORD_H
#define LORD_H

#include <string>

#include "types.h"
#include "bytes.h"

class Lord
{
    /**
     * read line from fd;
     * if fd is empty return ""
     */
    std::string readline(int);
public:
    /**
     * load data with different encordings
     * arg - file discriptor with read right
     */
    Bytes ** hex(int);
    Bytes ** boo(int);
    Bytes ** bin(int);
};

#endif /* LORD_H */