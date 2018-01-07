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

// Apartment record
struct ApartmentItem
{
    std::string ApartmentId;
    int ApartmentNumber;
    bool DivertToConcierge;
};

#endif // PACKETS_H_INCLUDED
