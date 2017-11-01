#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../libGatewayClient/include/gway_api.h"
#include "constants.h"

//
//  Retrieves pages of key codes for the specified entrance panel and outputs the list to stdout
//  Note that we never display more than MAX_PAGES_TO_DISPLAY pages
//
bool DisplayAllKeys(const char* url, const char* entrancePanel)
{
    fprintf(stdout, "------------------------\n");
    fprintf(stdout, "Fetching all keys for %s...\n", entrancePanel);

    // Allocate a buffer for a page of key codes from stack memory
    char keys[KEYCODE_PAGE_SIZE][KEYCODE_LENGTH];
    char* buffer[KEYCODE_PAGE_SIZE];
    int i;
    for (i = 0 ; i < KEYCODE_PAGE_SIZE ; i++) buffer[i] = keys[i];

    GatewayReturnCodes status = GWAY_SUCCESS;
    int keyCount = 0;
    int pageNumber = 1;
    char* lastKeycodeOnPreviousPage = "";

    do
    {
        status = FetchPageOfKeys(url, entrancePanel, lastKeycodeOnPreviousPage, buffer, KEYCODE_LENGTH, KEYCODE_PAGE_SIZE, &keyCount);

        if (IsSuccess(status))
        {
            fprintf(stdout, "Page %d.  %d key(s) returned\n", pageNumber, keyCount);
            for (i = 0 ; i < keyCount ; i++)
            {
                fprintf(stdout, "\t%s\n", buffer[i]);
            }
            fprintf(stdout, "\n");

            lastKeycodeOnPreviousPage = buffer[keyCount - 1];
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
