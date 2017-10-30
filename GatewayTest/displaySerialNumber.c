#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../libGatewayClient/include/gway_api.h"

const size_t SERIALNUMBERLENGTH = 16;

//
//  Retrieves serial number of the gateway at the given url and outputs it to stdout
//
bool DisplaySerialNumber(const char* url)
{
    fprintf(stdout, "Querying %s...\n", url);

    char serialNumber[SERIALNUMBERLENGTH + 1];
    GatewayReturnCodes status = LookupGatewaySerialNumber(url, serialNumber, SERIALNUMBERLENGTH + 1);

    if (IsSuccess(status))
    {
        fprintf(stdout, "Gateway found.  Serial number is %s\n\n", serialNumber);
        return true;
    }
    else
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "*** Failed to fetch serial number ***\n");
        fprintf(stderr, "\tError code %d - %s\n", status, ErrorDescription(status));
        return false;
    }
}
