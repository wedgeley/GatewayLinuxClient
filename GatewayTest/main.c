#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../libGatewayClient/include/gway_api.h"
int main()
{
    char serialNumber[80];
    GatewayReturnCodes status = LookupGatewaySerialNumber("http://192.168.2.3:8080", serialNumber, 80);
    if (IsSuccess(status))
    {
        fprintf(stdout, "Serial number is %s\n", serialNumber);
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
