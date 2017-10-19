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
        printf("Serial number is %s\n", serialNumber);
    }
    else
    {
        printf("Failed to fetch serial number.  Error %d\n", status);
    }

    return 0;
}
