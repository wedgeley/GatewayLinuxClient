#include <string>
#include "GatewayAPI.h"


// Test function adding two integers and returning the result
extern "C"
int SampleAddInt(int i1, int i2)
{
    return i1 + i2;
}

// Requests serial number from the Gateway
// Writes the serial number into the supplied buffer
extern "C"
void LookupGatewaySerialNumber(const char* url, char* buffer, uint length)
{
    GatewayAPI api;
    std::string serialNumber = api.LookupGatewaySerialNumber(url);

    if (serialNumber.length() < length)
    {
        // Sufficient room for the serial number
        strcpy(buffer, serialNumber.c_str());
    }
}
