#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "../libGatewayClient/include/gway_api.h"
#include "constants.h"

//
//  Retrieves pages of key codes for the specified entrance panel and outputs the list to stdout
//  Note that we never display more than MAX_PAGES_TO_DISPLAY pages
//
//  utcCloudSyncTicks is returned to the caller.  It is the time the Gateway last sync-ed to the cloud
//
bool DisplayAllKeys(const char* url, const char* entrancePanel, unsigned long long* utcCloudSyncTicks)
{
    GatewayReturnCodes status = GWAY_SUCCESS;

    fprintf(stdout, "Fetching all keys for %s...\n", entrancePanel);

    // Display time of last cloud sync
    status = LookupGatewayLastCloudSyncTime(url, utcCloudSyncTicks);

    if (IsSuccess(status))
    {
        time_t t;
        struct tm *tm;

        t = (time_t)((*utcCloudSyncTicks - UNIXEPOCH) / 10000000);
        tm = gmtime(&t);

        fprintf(stdout, "The Gateway last sync-ed to the cloud at %s\n", asctime(tm));           // asctime returns string ending with newline
    }

    // Allocate memory for a page of key entries from the stack
    int i;
    char keys[KEYCODE_PAGE_SIZE][KEYCODE_LENGTH + 1];           // Allocates memory for the key codes
    struct KeyEntry keyentries[KEYCODE_PAGE_SIZE];                // Allocates memory for the key entry structures
    for (i = 0 ; i < KEYCODE_PAGE_SIZE ; i++) keyentries[i].KeyCode = keys[i];

    // To pass into the function, convert to an array of pointers
    struct KeyEntry *buffer[KEYCODE_PAGE_SIZE];
    for (i = 0 ; i < KEYCODE_PAGE_SIZE ; i++) buffer[i] = &keyentries[i];

    int keyCount = 0;
    int pageNumber = 1;
    char inPageMarker[40] = "";
    char outPageMarker[40] = "";

    do
    {
        status = FetchPageOfKeys(url, entrancePanel, inPageMarker, outPageMarker, buffer, KEYCODE_LENGTH, KEYCODE_PAGE_SIZE, &keyCount);

        if (IsSuccess(status))
        {
            fprintf(stdout, "Page %d.  %d key(s) returned\n", pageNumber, keyCount);
            for (i = 0 ; i < keyCount ; i++)
            {
                fprintf(stdout, "\t%s", buffer[i]->KeyCode);
                if (buffer[i]->Tagged) fprintf(stdout, " (TAGGED)");
                fprintf(stdout, "\n");
            }
            fprintf(stdout, "\n");

            strcpy(inPageMarker, outPageMarker);
            pageNumber++;
        }
    }
    while (IsSuccess(status) &&                     // Lookup was successful
           keyCount == KEYCODE_PAGE_SIZE &&         // We got a full page of keys
           pageNumber < MAX_PAGES_TO_DISPLAY);      // Limit the display. (Also protects against endless loop)

    if (!IsSuccess(status))
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "*** Failed to fetch keys ***\n");
        fprintf(stderr, "\tError code %d - %s\n", status, ErrorDescription(status));
        return false;
    }

    return true;
}
