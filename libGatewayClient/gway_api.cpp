#include <string>
#include "GatewayClient.h"
#include "gway_api.h"


// Requests serial number from the Gateway
// Writes the serial number into the supplied buffer
extern "C"
GatewayReturnCodes LookupGatewaySerialNumber(const char* url, char* buffer, size_t bufferLength)
{
    GatewayReturnCodes status = GWAY_SUCCESS;
    GatewayClient client(url);

    std::string serialNumber;
    status = client.LookupGatewaySerialNumber(serialNumber);

    if (IsSuccess(status) && serialNumber.length() > (bufferLength - 1))
    {
        // Insufficient room for the serial number
        fprintf(stderr, "Error: Buffer of size %d characters too small for serial number of length %d\n", bufferLength, serialNumber.length());
        status = GWAY_BUFFER_TOO_SMALL;
    }

    if (IsSuccess(status))
    {
        strcpy(buffer, serialNumber.c_str());
    }

    return status;
}

// Requests all keys for the controller with the specified serial number
extern "C"
GatewayReturnCodes ListAllKeys(const char* url, const char* controllerSerialNumber, char* buffer[], size_t keyCodeSize, uint numberOfKeycodes, int* returnedKeyCount)
{
    GatewayReturnCodes status = GWAY_SUCCESS;
    *returnedKeyCount = 0;
    GatewayClient client(url);

    std::vector<std::string> keycodes;
    status = client.ListAllKeys(controllerSerialNumber, keycodes);

    if (IsSuccess(status) && keycodes.size() > numberOfKeycodes)
    {
        // Insufficient room for this many keys
        status = GWAY_ARRAY_TOO_SMALL;
    }

    if (IsSuccess(status))
    {
        *returnedKeyCount = keycodes.size();
        for (uint i=0 ; i < keycodes.size(); i++)
        {
            if (keycodes[i].length() > (keyCodeSize - 1))
            {
                status = GWAY_BUFFER_TOO_SMALL;
            }
            else
            {
                strcpy(buffer[i], keycodes[i].c_str());
            }
        }
    }

    return status;
}
