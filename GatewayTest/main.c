#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../libGatewayClient/include/gway_api.h"
#include "displaySerialNumber.h"
#include "displayAllKeycodes.h"
#include "displayKeyUpdates.h"

const char* DEFAULT_URL = "http://192.168.2.2:8080";
const char* DEFAULT_ENTRANCEPANEL = "ENTRANCE001";

// Function prototypes
void PromptForUrl(char* url, size_t bufferSize);
void PromptForEntrancePanelSerialNumber(char* serialNumber, size_t bufferSize);

int main()
{
    // Prompt for the URL of the gateway
    char url[80];
    PromptForUrl(url, 80);

    // Prompt for the serial number of the entrance panel that we're emulating
    char entrancePanel[32];
    PromptForEntrancePanelSerialNumber(entrancePanel, 32);

    // Fetch the gateway's serial number
    bool ok = DisplaySerialNumber(url);

    if (ok)
    {
        // All keys allocated to this controller
        ok = DisplayAllKeys(url, entrancePanel);
    }

    if (ok)
    {
        // Key updates
        ok = DisplayKeyUpdates(url, entrancePanel);
    }

    fprintf(stdout, "------------------------\n");

    return ok ? 0 : 1;
}


//
//  Prompt for the URL of the gateway
//  User can press RETURN to use the default
//
void PromptForUrl(char* url, size_t bufferSize)
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
void PromptForEntrancePanelSerialNumber(char* serialNumber, size_t bufferSize)
{
    fprintf(stdout, "Serial number for entrance panel [%s]: ", DEFAULT_ENTRANCEPANEL);
    fgets(serialNumber, bufferSize, stdin);
    if (strlen(serialNumber) <= 1) strcpy(serialNumber, DEFAULT_ENTRANCEPANEL);
    serialNumber[strcspn(serialNumber, "\n")] = 0;  // Remove trailing '\n'
    fprintf(stdout, "\n");
}
