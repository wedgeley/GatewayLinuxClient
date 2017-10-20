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
        return 1;
    }


    fprintf(stdout, "Fetching keys...\n");

    // TODO: Change to dynamically allocated memory?
    // TODO: Request a fixed number of keys (paging)
    char keys[512][32];
    char* buffer[512];
    int i;
    for (i = 0 ; i < 512 ; i++) buffer[i] = keys[i];
    int keyCount;

    status = ListAllKeys(url, "ENTRANCE001", buffer, 32, 512, &keyCount);

    if (IsSuccess(status))
    {
        fprintf(stdout, "%d keys returned\n", keyCount);
        for (i = 0 ; i < keyCount ; i++)
        {
            fprintf(stdout, "\t%s\n", buffer[i]);
        }
    }
    else
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "*** Failed to fetch keys ***\n");
        fprintf(stderr, "\tError code %d\n", status);
        fprintf(stderr, "\t\"%s\"\n", ErrorDescription(status));
        return 1;
    }

    return 0;
}
