#ifndef PACKETS_H_INCLUDED
#define PACKETS_H_INCLUDED

#include <stdlib.h>
#include <string.h>

// Key entry record
struct KeyEntryItem
{
    std::string KeyCode;
    bool Tagged;
};

// Key update record
struct KeyUpdateItem
{
    std::string KeyCode;
    bool Active;
    bool Tagged;
    unsigned long long UtcTicks;
};

#endif // PACKETS_H_INCLUDED
