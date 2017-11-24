#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "../libGatewayClient/include/gway_api.h"
#include "constants.h"

// Keep track of which updates we last received
unsigned long long lastUpdateTicksUtc = 0;


//
//  Retrieves pages of key updates for the specified entrance panel and outputs the list to stdout
//  Note that we never display more than MAX_PAGES_TO_DISPLAY pages
//
bool DisplayKeyUpdates(const char* url, const char* entrancePanel, unsigned long long utcCloudSyncTicks)
{
    fprintf(stdout, "Fetching key updates for %s...\n", entrancePanel);

    if (lastUpdateTicksUtc == 0 && utcCloudSyncTicks > 0)
    {
        // First time in.  Use the supplied sync time so we are not fetching all updates from when time began
        lastUpdateTicksUtc = utcCloudSyncTicks;
    }

    // Allocate memory for a page of key updates from the stack
    int i;
    char keys[KEYCODE_PAGE_SIZE][KEYCODE_LENGTH + 1];           // Allocates memory for the key codes
    struct KeyUpdate updates[KEYCODE_PAGE_SIZE];                // Allocates memory for the update structures
    for (i = 0 ; i < KEYCODE_PAGE_SIZE ; i++) updates[i].KeyCode = keys[i];

    // To pass into the function, convert to an array of pointers
    struct KeyUpdate *buffer[KEYCODE_PAGE_SIZE];
    for (i = 0 ; i < KEYCODE_PAGE_SIZE ; i++) buffer[i] = &updates[i];

    GatewayReturnCodes status = GWAY_SUCCESS;
    int keyCount = 0;
    int pageNumber = 1;

    do
    {
        status = FetchKeyUpdates(url, entrancePanel, lastUpdateTicksUtc, buffer, KEYCODE_LENGTH, KEYCODE_PAGE_SIZE, &keyCount);

        if (IsSuccess(status))
        {
            fprintf(stdout, "Page %d.  %d key update(s) returned\n", pageNumber, keyCount);
            for (i = 0 ; i < keyCount ; i++)
            {
                time_t t;
                struct tm *tm;

                t = (time_t)((buffer[i]->UtcTicks - UNIXEPOCH) / 10000000);
                tm = gmtime(&t);

                if (buffer[i]->KeyCode[0] == '*')
                {
                    fprintf(stdout, "\t*** Key re-fetch requested on %s", asctime(tm));           // asctime returns string ending with newline
                }
                else if (buffer[i]->Active)
                {
                    fprintf(stdout, "\t+ Added %s on %s", buffer[i]->KeyCode, asctime(tm));       // asctime returns string ending with newline
                }
                else
                {
                    fprintf(stdout, "\t- Deleted %s on %s", buffer[i]->KeyCode, asctime(tm));     // asctime returns string ending with newline
                }
                lastUpdateTicksUtc = buffer[i]->UtcTicks;
            }
            fprintf(stdout, "\n");

            pageNumber++;
        }
    }
    while (IsSuccess(status) &&                     // Lookup was successful
           keyCount == KEYCODE_PAGE_SIZE &&         // We got a full page of keys
           pageNumber < MAX_PAGES_TO_DISPLAY);      // Limit the display. (Also protects against endless loop)

    if (!IsSuccess(status))
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "*** Failed to fetch key updates ***\n");
        fprintf(stderr, "\tError code %d - %s\n", status, ErrorDescription(status));
        return false;
    }

    return true;
}
