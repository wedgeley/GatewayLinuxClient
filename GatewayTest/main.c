#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../libGatewayClient/include/gway_api.h"
#include "displaySerialNumber.h"
#include "displayApartments.h"
#include "displayAllKeycodes.h"
#include "displayKeyUpdates.h"

const char* DEFAULT_URL = "http://192.168.2.2:8080";
const char* DEFAULT_ENTRANCEPANEL = "ENTRANCE001";

// Function prototypes
void PromptForUrl(char* url, size_t bufferSize);
void PromptForEntrancePanelSerialNumber(char* serialNumber, size_t bufferSize);
void ListenForUpdates();
void UpdateAvailable();

// Gateway address and our serial number
char url[80];
char entrancePanel[32];

int main()
{
    // Prompt for the URL of the gateway
    PromptForUrl(url, sizeof(url));

    // Prompt for the serial number of the entrance panel that we're emulating
    PromptForEntrancePanelSerialNumber(entrancePanel, sizeof(entrancePanel));

    // Fetch the gateway's serial number
    bool ok = DisplaySerialNumber(url);

    // This is the last time the Gateway sync-ed with the cloud
    unsigned long long utcCloudSyncTicks;

    if (ok)
    {
        // All apartments for this controller
        fprintf(stdout, "------------------------\n");
        ok = DisplayApartments(url, entrancePanel);
    }

    if (ok)
    {
        // All keys allocated to this controller
        fprintf(stdout, "------------------------\n");
        ok = DisplayAllKeys(url, entrancePanel, &utcCloudSyncTicks);
    }

    if (ok)
    {
        // Key updates
        fprintf(stdout, "------------------------\n");
        ok = DisplayKeyUpdates(url, entrancePanel, utcCloudSyncTicks);
    }

    if (ok)
    {
        // Listen for notifications that updates are available
        fprintf(stdout, "------------------------\n");
        ListenForUpdates();
    }

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


//
//  Listens for key update notifications until the user presses ENTER
//
void ListenForUpdates()
{
    // Listen for notifications that updates are available
    RegisterForUpdateNotification(&UpdateAvailable);

    // Keep running until the user presses return
    fprintf(stdout, "\n\nListening for updates (Press ENTER to exit)... ");
    getc(stdin);

    // Exiting.  Stop listening for updates
    fprintf(stdout, "Exiting.  Waiting for listener to stop...\n");
    CancelUpdateNotification();
}


//
//  Function to be called when we are notified that an update is received
//
void UpdateAvailable()
{
    fprintf(stdout, "\nKey updates are available...\n");
    DisplayKeyUpdates(url, entrancePanel, 0);
}
