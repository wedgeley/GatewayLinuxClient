#include <string>
#include "GatewayClient.h"
#include "gway_api.h"


// Requests serial number from the Gateway
// Writes the serial number into the supplied buffer
extern "C"
GatewayReturnCodes LookupGatewaySerialNumber(const char* url, char* buffer, uint length)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    GatewayClient client;
    std::string serialNumber;
    status = client.LookupGatewaySerialNumber(url, serialNumber);

    if (status == GWAY_SUCCESS && serialNumber.length() >= length)
    {
        // Insufficient room for the serial number
        status = GWAY_BUFFER_TOO_SMALL;
    }

    if (status == GWAY_SUCCESS)
    {
        strcpy(buffer, serialNumber.c_str());
    }

    return status;
}