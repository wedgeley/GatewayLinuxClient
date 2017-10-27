#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../libGatewayClient/include/gway_api.h"

const size_t SERIALNUMBERLENGTH = 16;
const char* DEFAULT_URL = "http://192.168.2.3:8080";
const char* DEFAULT_ENTRANCEPANEL = "ENTRANCE001";

// Function prototypes
void ReadUrl(char* url, size_t bufferSize);
void ReadEntrancePanelSerialNumber(char* serialNumber, size_t bufferSize);
bool FetchSerialNumber(const char* url);
bool FetchAllKeys(const char* url, const char* entrancePanel);


int main()
{
    // Prompt for the URL of the gateway
    char url[80];
    ReadUrl(url, 80);

    // Prompt for the serial number of the entrance panel that we're emulating
    char entrancePanel[32];
    ReadEntrancePanelSerialNumber(entrancePanel, 32);

    // Fetch the gateway's serial number
    bool ok = FetchSerialNumber(url);

    if (ok)
    {
        // All keys allocated to this controller
        ok = FetchAllKeys(url, entrancePanel);
    }

    return ok ? 0 : 1;
}


//
//  Prompt for the URL of the gateway
//  User can press RETURN to use the default
//
void ReadUrl(char* url, size_t bufferSize)
{
    fprintf(stdout, "URL of gateway [%s]: ", DEFAULT_URL);
    fgets(url, bufferSize, stdin);
    if (strlen(url) <= 1) strcpy(url, DEFAULT_URL);
    url[strcspn(url, "\n")] = 0;    // Remove trailing '\n'
    fprintf(stdout, "\n");
}


//
//  Prompt for the serial number of the entrance panel we're emulating
//  User can press RETURN to use the default
//
void ReadEntrancePanelSerialNumber(char* serialNumber, size_t bufferSize)
{
    fprintf(stdout, "Serial number for entrance panel [%s]: ", DEFAULT_ENTRANCEPANEL);
    fgets(serialNumber, bufferSize, stdin);
    if (strlen(serialNumber) <= 1) strcpy(serialNumber, DEFAULT_ENTRANCEPANEL);
    serialNumber[strcspn(serialNumber, "\n")] = 0;  // Remove trailing '\n'
    fprintf(stdout, "\n");
}


//
//  Retrieves serial number of the gateway at the given url and outputs it to stdout
//
bool FetchSerialNumber(const char* url)
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

//
//  Retrieves a list of all keys for the specified entrance panel and outputs the list to stdout
//
bool FetchAllKeys(const char* url, const char* entrancePanel)
{
    fprintf(stdout, "Fetching all keys for %s...\n", entrancePanel);

    // TODO: Change to dynamically allocated memory?
    // TODO: Request a fixed number of keys (paging)
    char keys[512][32];
    char* buffer[512];
    int i;
    for (i = 0 ; i < 512 ; i++) buffer[i] = keys[i];
    int keyCount;

    GatewayReturnCodes status = ListAllKeys(url, entrancePanel, buffer, 32, 512, &keyCount);

    if (IsSuccess(status))
    {
        fprintf(stdout, "%d keys returned\n", keyCount);
        for (i = 0 ; i < keyCount ; i++)
        {
            fprintf(stdout, "\t%s\n", buffer[i]);
        }
        fprintf(stdout, "\n");
        return true;
    }
    else
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "*** Failed to fetch keys ***\n");
        fprintf(stderr, "\tError code %d - %s\n", status, ErrorDescription(status));
        return false;
    }
}
