#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../libGatewayClient/include/gway_api.h"

int main()
{
    const char* url = "http://192.168.2.3:8080";
    fprintf(stdout, "Querying %s...\n", url);

    char serialNumber[80];
    GatewayReturnCodes status = LookupGatewaySerialNumber(url, serialNumber, 80);

    if (IsSuccess(status))
    {
        fprintf(stdout, "Gateway found!  Serial number is %s\n", serialNumber);
    }
    else
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "*** Failed to fetch serial number ***\n");
        fprintf(stderr, "\tError code %d\n", status);
        fprintf(stderr, "\t\"%s\"\n", ErrorDescription(status));
    }

    return 0;
}
