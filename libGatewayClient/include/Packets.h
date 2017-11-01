#ifndef PACKETS_H_INCLUDED
#define PACKETS_H_INCLUDED

#include <stdlib.h>
#include <string.h>

// Key update record
struct KeyUpdateItem
{
    std::string KeyCode;
    bool Active;
    unsigned long long UtcTicks;
};

#endif // PACKETS_H_INCLUDED
